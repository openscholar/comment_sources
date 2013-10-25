<?php

/**
 * @file
 * Administration form for the comment_sources module
 */

/**
 * Includes non-hook functions and helper functions.
 */
ctools_include('api', 'comment_sources');

/**
 * Includes comment_sources hook implementations for Drupal, Facebook, Disqus.
 */
ctools_include('sources', 'comment_sources');

/**
 * Menu callback; Displays the administration settings for comment sources.
 */
function comment_sources_admin_settings_form() {

  $form = array();
  $node_types = node_type_get_types();

  $sources = comment_sources_enabled_sources();
  $options = array_merge(_comment_sources_form_options($sources), array('' => t('No comments')));
  $default_values = comment_sources_type_settings();
  foreach ($node_types as $key => $value) {
    $form["comment_sources_$key"] = array(
      '#type' => 'radios',
      '#title' => check_plain($value->name),
      '#options' => $options,
      '#default_value' => isset($default_values[$key]) ? $default_values[$key] : '',
    );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'comment_sources_save_admin_form';
  return $form;
}

/**
 * Save the comment source selections by node type.
 */
function comment_sources_save_admin_form($form, $form_state) {

  // Creates an array for each comment source listing selected node types.
  $type_settings = array();
  if (isset($form_state['values'])) {
    foreach ($form_state['values'] as $key => $value) {
      $key = str_replace('comment_sources_', '', $key);
      $type_settings[$key] = $value;
    }
  }

  comment_sources_save_type_settings($type_settings);
}
