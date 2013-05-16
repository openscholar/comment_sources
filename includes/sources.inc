<?php

/**
 * @file
 * Hook implementations and callbacks for comment_sources module.
 */

/**
 * Gets Facebook Social comment presets.
 */
function _comment_sources_preset_fb_social() {
  $presets = array();
  if (function_exists('fb_social_get_presets_by_type')) {
    $presets = fb_social_get_presets_by_type('comments');
  }
  return $presets;
}

/**
 * Gets available comment presets for fb_social.
 *
 * Loops over all comments fb_social presets, and returns an array of comment
 * sources for each, keyed by "fb_social:$preset_name".
 *
 * @param NULL $null
 *   unused @todo remove?
 *
 * @return array
 *   An array of enabled sources for fb_social.
 */
function comment_sources_fb_social($null = NULL) {
  $enabled_sources = array();

  $presets = _comment_sources_preset_fb_social();

  if (count($presets) > 0) {
    foreach ($presets as $preset) {
      $enabled_sources[implode(':', array('fb_social', $preset->name))] = array(
        'description' => $preset->name,
      );
    }
  }

  return $enabled_sources;
}

/**
 * Gets Disqus node types comment settings.
 *
 * @return array
 *   Node types using Disqus comments
 *
 * @see comment_sources_nodetypes_sources()
 */
function _comment_sources_load_disqus() {
  return variable_get('disqus_nodetypes', array());
}

/**
 * Gets Facebook preset node types comment settings.
 *
 * @return array
 *   Node types using Facebook comments
 *
 * @see comment_sources_nodetypes_sources()
 */
function _comment_sources_load_fb_social($variant = '') {
  $result = array();
  if ($variant && function_exists('fb_social_preset_load')) {
    $preset = fb_social_preset_load($variant);
    $result = $preset->settings['node_types']['types'];
  }
  return $result;
}

/**
 * Gets Drupal node types comment settings.
 *
 * @return array
 *   Node types using Drupal core comments.
 *
 * @see comment_sources_nodetypes_sources()
 */
function _comment_sources_load_comment() {
  $result = &drupal_static(__FUNCTION__);
  if (!is_null($result)) {
    return $result;
  }

  $node_types = node_type_get_types();
  $result = array();
  foreach ($node_types as $node_type) {
    $setting = variable_get("comment_" . $node_type->type, COMMENT_NODE_CLOSED);
    if ($setting == COMMENT_NODE_OPEN) {
      $result[$node_type->type] = $node_type->type;
    }
  }
  return $result;
}

/**
 * Saves the Disqus node types comment settings.
 */
function _comment_sources_save_disqus($enabled_nodetypes) {
  variable_set('disqus_nodetypes', $enabled_nodetypes);
}

/**
 * Saves the Facebook node type comments settings.
 */
function _comment_sources_save_fb_social($enabled_nodetypes, $preset_name = '') {
  if ($preset_name && function_exists('fb_social_preset_load')) {
    $preset = fb_social_preset_load($preset_name);
    $preset->settings['node_types']['types'] = $enabled_nodetypes;
    ctools_export_crud_save($preset->table, $preset);
  }
}

/**
 * Saves the Drupal comments node types comment settings.
 */
function _comment_sources_save_comment($enabled_nodetypes) {
  $node_types = node_type_get_types();
  foreach ($node_types as $node_type) {
    if ($enabled_nodetypes[$node_type->type]) {
      $setting = COMMENT_SOURCES_DRUPAL_ON;
    }
    else {
      $setting = COMMENT_SOURCES_DRUPAL_OFF;
    }
    variable_set("comment_$node_type->type", $setting);
  }
}

/**
 * Gets the most recent Drupal comments for a content type.
 */
function _comment_sources_recent_comments_comment($count) {
  // Additionally order by cid to ensure that comments with the same timestamp
  // are returned in the exact order posted.
  $comments = db_select('comment', 'c')->innerJoin('node', 'n', 'n.nid = c.nid')->addTag('node_access')->fields('c')->condition('c.status', COMMENT_PUBLISHED)->condition('n.status', NODE_PUBLISHED)->orderBy('c.created', 'DESC')->orderBy('c.cid', 'DESC')->range(0, $count)->execute()->fetchAll();

  return $comments ? $comments : array();
}

/**
 * Gets the most recent Disqus comments for a content type.
 */
function _comment_sources_recent_comments_disqus($count) {
  $posts = array();

  $user_api_key = variable_get('disqus_userapikey', '');
  if (empty($user_api_key)) {
    watchdog('comment_sources', "User API key required to display recent Disqus comments", array(), WATCHDOG_ERROR);
  }
  else {
    try {
      $disqus = disqus($user_api_key);
      $forums = $disqus->get_forum_list();
      if (count($forums) != 1) {
        watchdog('comment_sources', "More than one Disqus forum returned. Cannot display recent Disqus comments", array(), WATCHDOG_ERROR);
      }
      else {
        $forum_id = $forums[0]->id;
        $forum_api_key = $disqus->get_forum_api_key($forum_id);
        if (empty($forum_api_key)) {
          watchdog('comment_sources', "Could not retrieve forum key to display recent Disqus comments", array(), WATCHDOG_ERROR);
        }
        else {
          $posts = $disqus->get_forum_posts($forum_id, array(
              'forum_api_key' => $forum_api_key,
              'limit' => $count,
            ));
        }
      }
    }
    catch(DisqusException$d) {
      watchdog('comment_sources', "Error trying to retrieve comments from Disqus", array(), WATCHDOG_ERROR);
    }
  }
  return $posts;
}

/**
 * Gets the most recent Facebook comments for a content type.
 *
 * @todo
 */
function _comment_sources_recent_comments_fb_social($content_type, $count) {
  return array();
}

