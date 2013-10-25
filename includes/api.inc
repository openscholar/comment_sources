<?php

/**
 * @file
 * Hook functions and helpers for comment_sources module.
 */

/**
 * Gets list of available comment sources.
 *
 * In some cases (fb_social) a module can provide multiple comment sources.
 * If that's the case, provide a callback function to retrieve each comment
 * source associated with that module.
 *
 * @return array
 *   An array of available comment sources, keyed by machine name.
 */
function _comment_sources_sources() {
  // Drupal content style guide: use "sentence caps" for headings and actions.
  // @see http://drupal.org/style-guide/content#headings
  $base_sources = array(
    'comment' => array(
      'description' => t('Drupal comments'),
      'supports_recent_comments' => TRUE,
    ),
    'disqus' => array(
      'description' => t('Disqus comments'),
      'supports_recent_comments' => TRUE,
    ),
    'fb_social' => array(
      'description' => t('Facebook comments'),
      'supports_recent_comments' => FALSE,
      'children_callback' => 'comment_sources_fb_social',
    ),
  );

  // Gets comment sources provided by other modules.
  $other_sources = module_invoke_all("comment_sources");
  $sources = array_merge_recursive($base_sources, $other_sources);
  // Allows other modules to alter the $sources array.
  drupal_alter('comment_sources', $sources);

  return $sources;
}

/**
 * Gets list of the currently enabled comment sources.
 *
 * @return array
 *   An array of enabled comment sources, keyed by machine name.
 */
function comment_sources_enabled_sources() {
  $enabled_sources = &drupal_static(__FUNCTION__);
  if (!is_null($enabled_sources)) {
    return $enabled_sources;
  }

  $sources = _comment_sources_sources();
  $enabled_sources = array();

  // Checks to ensure the comment source modules are enabled.
  foreach ($sources as $key => $value) {
    if (module_exists($key)) {
      if (isset($value['children_callback'])) {
        $enabled_sources = array_merge($enabled_sources, call_user_func($value['children_callback'], $value));
      }
      else {
        $enabled_sources[$key] = $value;
      }
    }
  }
  return $enabled_sources;
}

/**
 * Checks if the given content source supports retrieving recent comments.
 *
 * @param string $source
 *   A machine name of a comment source
 *
 * @return bool
 *   Returns TRUE only if the source is available and provides a value for
 *   'supports_recent_comments' that casts to TRUE.
 */
function comment_sources_supports_recent_comments($source) {
  $sources = _comment_sources_sources();
  if (isset($sources[$source])) {
    $result = (bool) $sources[$source]['supports_recent_comments'];
  }
  else {
    $result = FALSE;
  }

  return $result;
}

/**
 * Updates database when a 3rd party comment is added.
 *
 * Sets has_comments to 1 and status to 1 for the given node ID and source.
 *
 * Triggered by client-side javascript on comment creation.
 */
function comment_sources_record_comment_ajax($nid = FALSE, $source = FALSE) {
  if (!$nid && !$source) {
    // Gets the query data for database operations.
    $nid    = filter_input(INPUT_GET, 'nid', FILTER_VALIDATE_INT);
    $source = $_GET['source'];
  }

  // Does nothing if either the source or nid are invalid.
  if (empty($source) || empty($nid)) {
    return;
  }

  // Finds any existing record for this node and this comment source.
  $record = db_select('comment_sources_comments', 'c')
    ->condition('c.nid', $nid)
    ->condition('c.source', $source)
    ->fields('c', array('nid'))
    ->execute();

  // Updates or adds a record for this node and source.
  // Sets has_comments to TRUE (1) and status to ACTIVE (1).
  $data = array(
    'nid' => $nid,
    'source' => $source,
    'status' => COMMENT_SOURCES_ACTIVE,
    'has_comments' => TRUE,
  );

  // Creates or updates record.
  if ($record->rowCount() > 0) {
    drupal_write_record('comment_sources_comments', $data, array('nid', 'source'));
  }
  else {
    drupal_write_record('comment_sources_comments', $data);
  }

  // Additionally, sets any other comment sources to INACTIVE.
  $record = db_update('comment_sources_comments')
    ->fields(array('status' => COMMENT_SOURCES_INACTIVE))
    ->condition('nid', $nid)
    ->condition('source', $source, '!=')
    ->execute();
}

/**
 * Sets the comment source for this content type, with appropriate options.
 */
function comment_sources_set_type_source($source, $type) {
  // Returns FALSE if the specified node type is invalid.
  if (!array_key_exists($type, node_type_get_types())) {
    return FALSE;
  }

  // Gets a keyed array mapping nodetypes => current comment source.
  $type_settings = comment_sources_type_settings();

  // If a source is specified, validates and prepares to save.
  if (!empty($source)) {
    // Returns FALSE if the specified source is invalid.
    $enabled_sources = comment_sources_enabled_sources();
    if (!array_key_exists($source, $enabled_sources)) {
      return FALSE;
    }
    $type_settings[$type] = $source;
  }

  // Otherwise, unsets this content type, disabling comments for the node type.
  else {
    unset($type_settings[$type]);
  }

  // Saves changes to static variable and invokes save callbacks.
  comment_sources_save_type_settings($type_settings);

  return TRUE;
}

/**
 * Gets the comment source for this content type or node, or FALSE if invalid.
 *
 * @param mixed $node_info
 *   Accepts a node object, node type string or nid.
 * @param bool $trim_preset
 *   TRUE if the returned comment source should trim the "plugin" portion from
 *   the end of the string. i.e.
 *   FALSE => 'fb_social:someplugin'
 *   TRUE => 'fb_social'
 * @param string $default_source
 *   Which source should be returned if none found.
 *
 * @return string
 *   The plugin string, with or without the plugin suffix.
 *
 * @return mixed
 *   One of the available source strings, or FALSE if passed invalid param.
 */
function comment_sources_get_source($node_info, $trim_preset = FALSE, $default_source = '') {
  // If this looks like a nodetype string, return the nodetype's source.
  if (is_string($node_info) && !is_numeric($node_info)) {
    $type = $node_info;
    return comment_sources_get_source_type($type, $trim_preset, $default_source);
  }
  // Otherwise, attempt to load the node if this looks like a node ID.
  if (is_numeric($node_info)) {
    $node_info = node_load($node_info);
  }
  // If we have a node object...
  if (is_object($node_info) && isset($node_info->type)) {
    $node = $node_info;
    // Returns the default source for this node type if the node is new.
    if (!isset($node->nid) || isset($node->is_new)) {
      return comment_sources_get_source_type($node->type, $trim_preset, $default_source);
    }
    // Otherwise, finds the existing stored comment source for this node.
    return comment_sources_get_source_node($node, $trim_preset, $default_source);
  }
  // Returns FALSE if the $node_info param was invalid.
  return FALSE;
}

/**
 * Checks db if this comment source has any comments for this node.
 *
 * @param mixed $node
 *   Accepts a node object or a node ID
 * @param string $source
 *   A machine name of a comment source.
 *
 * @return bool
 *   TRUE only if the node is found and stored with has_comments = 1.
 *
 * Uses drupal_static("comment_sources_has_comments") to store:
 * @code
 * $has_comments = array(
 *   'source1' => array(
 *     '123' => 0,
 *     '234' => 1,
 *     ...
 *   ),
 *   'source2' => array(...),
 * );
 * @endcode
 */
function comment_sources_has_comments($node, $source) {
  // Handles and validates $node param.
  if (is_object($node) && $node->nid) {
    $nid = $node->nid;
  }
  if (is_numeric($node)) {
    $nid = $node;
  }
  if (!$nid) {
    return FALSE;
  }

  // Stores info in a drupal_static variable.
  $has_comments = &drupal_static(__FUNCTION__, array());
  // Quickly returns the already indexed information.
  if (isset($has_comments[$source]) && isset($has_comments[$source][$nid])) {
    return $has_comments[$source][$nid];
  }

  // Otherwise, this function is being invoked for the first time with these
  // params. Builds and returns the information.
  if (!isset($has_comments[$source])) {
    $has_comments[$source] = array();
  }

  // Default value is FALSE if we don't find any information in the database.
  $has_comments[$source][$nid] = FALSE;
  // Gets the "has_comments" value from the database.
  $record = db_select('comment_sources_comments', 'c')
    ->condition('c.nid', $nid)
    ->condition('c.source', $source)
    ->fields('c', array('has_comments'))
    ->execute();
  if ($record->rowCount() == 1) {
    $has_comments[$source][$nid] = ($record->fetchField(0) == 1);
  }

  return $has_comments[$source][$nid];
}

/**
 * Saves a node type's current comment source.
 *
 * @param array $type_settings
 *   The new comment sources values, keyed by node type, with some extraneous
 *   form information.
 */
function comment_sources_save_type_settings($type_settings) {
  // Flips the $nodetypes_sources array to be keyed by comment source,
  // while also only using valid nodetypes as keys for the new array.
  $sources_nodetypes = array();
  $enabled_sources = comment_sources_enabled_sources();
  $nodetypes = array_keys(_node_types_build()->types);
  // For each array key-value pair like "type" -> "source:preset"
  foreach ($type_settings as $nodetype => $source) {
    // Only flips valid sources & nodetypes pairs.
    if ($source && in_array($nodetype, $nodetypes)) {
      // Strips the preset off the end of the source ID string.
      $source = preg_replace('/:.*$/', '', $source);
      // Adds to the flipped array.
      $sources_nodetypes[$source][$nodetype] = $nodetype;
    }
  }

  // Gets array containing all sources settings.
  // Contains save_function callback from each source info.
  $sources = _comment_sources_sources();

  // For each enabled source...
  foreach (array_keys($enabled_sources) as $source) {
    // Checks for any preset at the end of the string, like "source:preset".
    preg_match('/:(.*$)/', $source, $matches);
    // If there is a match, $matches[1] will be the "preset" from example above.
    if (isset($matches[1]) && $matches[1]) {
      $source = preg_replace('/:.*$/', '', $source);
      $preset = $matches[1];
    }
    // Prepares the nodetype save callback function.
    if (isset($sources[$source]['save_function'])) {
      $save_nodetype_callback = $sources[$source]['save_function'];
    }
    else {
      $save_nodetype_callback = "_comment_sources_save_" . $source;
    }
    // Invokes the nodetype save callback for this source, if it exists.
    if (function_exists($save_nodetype_callback)) {
      $enabled_nodetypes = isset($sources_nodetypes[$source]) ? $sources_nodetypes[$source] : array();
      $params = array();
      $params[] = $enabled_nodetypes;
      // Handles comment sources like "$module:$preset".
      if (isset($preset) && $preset) {
        $params[] = $matches[1];
      }
      call_user_func_array($save_nodetype_callback, $params);
    }
  }
}

/**
 * Builds list of comment source options for a form element.
 */
function _comment_sources_form_options() {
  // Gets all enabled comment sources.
  $sources = comment_sources_enabled_sources();
  // Adds a machine name keyed element for each enabled source.
  $node_options = array();
  foreach ($sources as $key => $value) {
    $node_options[$key] = $value['description'];
  }

  return $node_options;
}

/**
 * Returns an array of current source settings for each node type.
 *
 * Where there are multiple comment sources provided by the same module, the
 * source names will be of the form 'module_name:source_name'.
 *
 * @param string $default_source
 *   What to return for a nodetype if no source is returned.
 *
 * @return array
 *   An array containing comment sources keyed by node type
 */
function comment_sources_type_settings($default_source = '') {
  // Returns the stored information if available.
  $type_settings = &drupal_static(__FUNCTION__, array());
  if (!empty($type_settings)) {
    return $type_settings;
  }

  // Otherwise, builds the info array.
  $enabled_sources = comment_sources_enabled_sources();
  // For each enabled source...
  foreach (array_keys($enabled_sources) as $source) {
    // Handles sources like "$module:$plugin" (i.e. fb_social)
    preg_match('/:(.*$)/', $source, $matches);
    $nodetypes_callback = "_comment_sources_load_" . preg_replace('/:.*$/', '', $source);
    // Gets the nodetypes settings from the source-specific callback.
    if (function_exists($nodetypes_callback)) {
      $params = array();
      if (count($matches) > 1) {
        $params[] = $matches[1];
      }
      $nodetypes = call_user_func_array($nodetypes_callback, $params);
      // @todo what if two comment sources both think they are enabled for the
      // same node type?
      foreach ($nodetypes as $nodetype) {
        $type_settings[$nodetype] = $source;
      }
    }
  }

  // Adds the default comment source to any nodetype missing a source.
  foreach (node_type_get_types() as $node_type) {
    if (!array_key_exists($node_type->type, $type_settings)) {
      $type_settings[$node_type->type] = $default_source;
    }
  }

  return $type_settings;
}

/**
 * Gets the most recent comments for a content source and content type.
 *
 * Wrapper function which calls _comment_sources_recent_comments_$source
 * if the function extists.
 *
 * @param string $source
 *   A machine name of a comment source.
 * @param int $count
 *   The maximum number of comments to return.
 *
 * @return array
 *   An array of recent comments.
 */
function comment_sources_recent_comments($source, $count = 10) {
  $recent_comments = array();
  $function_name = "_comment_sources_recent_comments_" . $source;
  if (function_exists($function_name)) {
    $recent_comments = call_user_func_array($function_name, array($count));
  }
  return $recent_comments;
}

/**
 * Records the comment source and status (1 or 0) for the given node.
 *
 * @param mixed $node
 *   A node object or node ID.
 * @param string $source
 *   A machine name of an available comment source.
 * @param int $status
 *   Whether comments are enabled or disabled.
 *
 * @return bool
 *   TRUE if the node settings were successfully updated.
 */
function comment_sources_record_node_settings($node, $source, $status) {
  // Handles and validates $node param.
  if (is_object($node) && $node->nid) {
    $nid = $node->nid;
  }
  if (is_numeric($node)) {
    $nid = $node;
  }
  if (!$nid) {
    return FALSE;
  }

  $success = _comment_sources_record_node_settings($nid, $source, $status);
  if ($success) {
    drupal_static_reset('comment_sources_node_sources');
  }
  // Returns TRUE if the drupal_write_record() succeeded.
  return ($success !== FALSE);
}

/**
 * Records the comment source and status.
 */
function _comment_sources_record_node_settings($nid, $source, $status) {
  // Finds any existing record of this node and this source.
  $record = db_select('comment_sources_comments', 'c')
    ->condition('c.nid', $nid)
    ->condition('c.source', $source)
    ->fields('c', array('nid'))
    ->execute();

  // Makes the data array to write to the database.
  $data = array(
    'nid' => $nid,
    'source' => $source,
    'status' => $status,
  );

  // Updates the existing record or adds a new record.
  if ($record->rowCount() > 0) {
    // Ensures only one comment source can be "ON" at a time.
    if ($status = COMMENT_SOURCES_ACTIVE) {
      // If any other sources are active and have comments, alter those records.
      $update = db_update('comment_sources_comments')
        ->fields(array(
          'status' => COMMENT_SOURCES_INACTIVE,
        ))
        ->condition('nid', $nid)
        ->condition('source', $source, '!=')
        ->execute();
    }
    // Sets the given source status to ON, now that no others are on.
    $success = drupal_write_record('comment_sources_comments', $data, array('nid', 'source'));
  }
  else {
    $success = drupal_write_record('comment_sources_comments', $data);
  }
  return $success;
}

/**
 * Gets the comment source for this node type.
 *
 * @param string $type
 *   The machine name for this node type
 * @param bool $trim_preset
 *   TRUE if the returned comment source should trim the "plugin" portion from
 *   the end of the string. i.e.
 *   FALSE => 'fb_social:someplugin'
 *   TRUE => 'fb_social'
 * @param string $default_source
 *   Which source should be returned if none found.
 *
 * @return string
 *   The plugin string, with or without the plugin suffix.
 */
function comment_sources_get_source_type($type, $trim_preset = FALSE, $default_source = '') {
  // Handles node ID as input.
  if (!is_string($type) && is_numeric($type)) {
    $node = node_load($type);
    $type = $node->type;
  }

  // Loads the (drupal_static) comment source settings for each content type.
  $type_settings = comment_sources_type_settings();

  // Assigns the stored comment source for this type.
  if (isset($type_settings[$type])) {
    $source = $type_settings[$type];
  }
  // Otherwise, assigns the (optional param) default source.
  else {
    $source = $default_source;
  }

  // Optionally removes the plugin from the source key.
  if ($trim_preset) {
    $source = preg_replace('/:.*$/', '', $source);
  }

  return $source;
}

/**
 * Gets the comment source for this node.
 *
 * @param object $node
 *   The loaded node object to check for comment source.
 * @param bool $trim_preset
 *   TRUE if the returned comment source should trim the "plugin" portion from
 *   the end of the string. i.e.
 *   FALSE => 'fb_social:someplugin'
 *   TRUE => 'fb_social'
 * @param string $default_source
 *   Which source should be returned if none found.
 *
 * @return string
 *   The plugin string, with or without the plugin suffix.
 */
function comment_sources_get_source_node($node, $trim_preset = FALSE, $default_source = '') {
  // Stores info in a drupal_static variable.
  $node_sources = &drupal_static('comment_sources_node_sources');

  // Initializes this node's info array if it does not exist.
  if (!isset($node_sources[$node->nid])) {
    // Finds the first-recorded ACTIVE comment source for this node.
    $results = db_select('comment_sources_comments', 'c')
      ->orderBy('cid', 'ASC')
      ->condition('nid', $node->nid)
      ->condition('status', COMMENT_SOURCES_ACTIVE)
      ->fields('c', array('source'))
      ->range(0, 1)
      ->execute()
      ->fetchAllAssoc('cid');
    if ($results) {
      foreach ($results as $cid => $record) {
        $source = $record->source;
      }
    }
    // Otherwise, defaults to this node type's setting if no result was found.
    else {
      $source = comment_sources_get_source_type($node->type, $trim_preset, $default_source);
    }

    // Stores this value in drupal static array.
    $node_sources[$node->nid] = $source;
  }

  $source = $node_sources[$node->nid];
  // Optionally removes the plugin from the source key.
  if ($trim_preset) {
    $source = preg_replace('/:.*$/', '', $source);
  }

  return $source;
}

/**
 * Returns the status (1 or 0) of the active comment source for the given node.
 */
function comment_sources_get_status($node) {
  if (is_object($node) && $node->nid) {
    $nid = $node->nid;
  }
  elseif (is_numeric($node)) {
    $nid = $node;
  }
  // Returns INACTIVE if the node has been overridden to INACTIVE.
  $results = db_select('comment_sources_comments', 'c')
    ->orderBy('cid', 'ASC')
    ->condition('nid', $nid)
    ->fields('c', array('source', 'status'))
    ->execute()
    ->fetchAllAssoc('cid');
  // Loops over all records for this node in the comment_sources_comments table.
  if (count($results)) {
    foreach ($results as $key => $record) {
      // If any of the records are set to active...
      if ($record->status == COMMENT_SOURCES_ACTIVE) {
        // Returns INACTIVE if the source is '' (No comments).
        if ($record->source == '') {
          return COMMENT_SOURCES_INACTIVE;
        }
        // Otherwise, this is an active, overridden source.
        // Returns ACTIVE.
        else {
          return COMMENT_SOURCES_ACTIVE;
        }
      }
    }
  }

  // Gets the current comment source for this node type.
  // Returns INACTIVE if no node record was found and current source is
  // '' (No comments).
  $source = comment_sources_get_source_type($nid);
  if (!$source) {
    return COMMENT_SOURCES_INACTIVE;
  }
  // Returns ACTIVE if no node record was found and a source is set.
  return COMMENT_SOURCES_ACTIVE;
}

/**
 * Returns TRUE if the given node is set to the given comment source.
 */
function comment_sources_should_display_comments($node, $source) {
  // First, returns FALSE if source is '' (No comments).
  if (!$source) {
    return FALSE;
  }

  // Otherwise, checks if the given source is active for this node.
  $node_source = comment_sources_get_source_node($node, TRUE);
  if ($source != $node_source) {
    return FALSE;
  }

  return TRUE;
}
