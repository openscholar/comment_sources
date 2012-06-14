<?php

/**
 * @file
 * Public API for the comment_sources module
 *
 */

/*
 * Get a list of the supported comment sources.
 * In some cases (fb_social) a module can provide multiple comment sources. If that's the case, provide a callback
 * function to retrieve each comment source associated with that module.
 */
function comment_sources_sources() {
  $base_sources = array(
    'comment' => array(
      'description' => t('Drupal Comments'),
      ),
    'disqus' => array(
      'description' => t('Disqus Comments'),
    ),
    'fb_social' => array(
      'description' => t('Facebook Comments'),
      'children_callback' => 'comment_sources_fb_social',
    )
  );
  return $base_sources;
}

/**
 * Get a list of the currently enabled comment sources
 */
function comment_sources_enabled_sources() {
  $sources = comment_sources_sources();
  $enabled_sources = array();

  // Check to ensure the comment source modules are enabled
  foreach ($sources as $key => $value) {
    $info = system_get_info('module',$key);
    if (count($info) > 0) {
      if (isset($value['children_callback'])) {
        $enabled_sources = array_merge($enabled_sources,call_user_func($value['children_callback'],$value));
      }
      else {
        $enabled_sources[$key] = $value;
      }
    }
  }
  return $enabled_sources;
}

/**
 * fb_social has a separate comment source for each preset. Retrieve the available comment presets
 */
function comment_sources_fb_social($source) {
  $presets = function_exists('fb_social_get_presets_by_type') ? fb_social_get_presets_by_type('comments') : array();
  $enabled_sources = array();
  if (count($presets) > 0) {
    foreach ($presets as $preset) {
      $enabled_sources[join(":",array('fb_social',$preset->name))] = array(
        'description' => join(":",array($source['description'],$preset->name)),
      );
    }
  }
  return $enabled_sources;
}

/**
 * Set the comment source for a given content type, with options appropriate to that comment source
 */
function comment_sources_set_source($content_type, $source, $options = array()) {
  $enabled_sources = comment_sources_enabled_sources();
  if (!array_key_exists($source,$enabled_sources) || !array_key_exists($content_type,node_type_get_types()))
    return false;

  $selected_sources = comment_sources_load($enabled_sources);
  $selected_sources[$content_type] = $source;
  comment_sources_save($enabled_sources,$selected_sources);

  return true;
}