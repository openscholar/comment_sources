<?php

/**
 * @file
 * Administration form for the comment_sources module
 */

module_load_include('api.php','comment_sources');

/**
 * Menu callback; Displays the administration settings for comment sources.
 */
function comment_sources_admin_settings_form() {

  $form = array();
  $node_types = node_type_get_types();

  $sources = comment_sources_enabled_sources();
  $options = comment_sources_node_options($sources);
  $default_values = comment_sources_load($sources);
  foreach ($node_types as $key => $value) {
    $form["comment_sources_$key"] = array(
      '#type' => 'radios',
      '#title' => $value->name,
      '#options' => $options,
      '#default_value' => isset($default_values[$key]) ? $default_values[$key] : 'none',
      );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'comment_sources_save_admin_form';
  return $form;
}

/**
 * Get the list of options that can be selected for each node type, in a format
 * that can be passed as '#options' to a radio button form element
 */
function comment_sources_node_options($sources)
{
  foreach ($sources as $key => $value) {
    $node_options[$key] = $value['description'];
  }
  $node_options['none'] = t("No Comments");
  return $node_options;
}

/**
 * Save the comment source selections by node type
 */
function comment_sources_save_admin_form($form, $form_state) {

  // Create an array for each comment source with the list of selected node types
  $selected_sources = array();
  if(isset($form_state['values'])) {
    foreach ($form_state['values'] as $key => $value) {
      $key = str_replace('comment_sources_','',$key);
      $selected_sources[$key] = $value;
    }
  }

  comment_sources_save(comment_sources_enabled_sources(),$selected_sources);
}


