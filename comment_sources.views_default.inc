<?php

/**
 * @file
 * Default views for this module
 */

/**
 * Implements hook_views_default_views().
 */
function comment_sources_views_default_views() {
  $view = new view;
  $view->name = 'review_comments';
  $view->description = 'Contains a block page to to review and approve Drupal comments';
  $view->tag = 'default';
  $view->base_table = 'comment';
  $view->human_name = 'Review Comments';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = t('Review comments');
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = t('No unapproved comments');
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Relationship: Comment: Content */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'comment';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Field: Comment: Title */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'comment';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['label'] = '';
  $handler->display->display_options['fields']['subject']['link_to_comment'] = 1;
  /* Field: Comment: Updated date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'comment';
  $handler->display->display_options['fields']['timestamp']['field'] = 'changed';
  $handler->display->display_options['fields']['timestamp']['label'] = '';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'time ago';
  /* Sort criterion: Comment: Updated date */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'comment';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'changed';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'nid';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = t('Type');
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['reduce'] = 0;
  /* Filter criterion: Comment: Approved */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'comment';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'subject' => 'subject',
    'comment' => 'comment',
    'title' => 'title',
    'timestamp' => 'timestamp',
    'approve_comment' => 'approve_comment',
    'delete_comment' => 'delete_comment',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'subject' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'comment' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'approve_comment' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_comment' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 1;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Comment: Title */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'comment';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['label'] = t('Comment Title');
  $handler->display->display_options['fields']['subject']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['external'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['subject']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['subject']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['subject']['alter']['html'] = 0;
  $handler->display->display_options['fields']['subject']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['subject']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['subject']['hide_empty'] = 0;
  $handler->display->display_options['fields']['subject']['empty_zero'] = 0;
  $handler->display->display_options['fields']['subject']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['subject']['link_to_comment'] = 1;
  $handler->display->display_options['fields']['subject']['link_to_node'] = 0;
  /* Field: Comment: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'field_data_comment_body';
  $handler->display->display_options['fields']['comment']['field'] = 'comment_body';
  $handler->display->display_options['fields']['comment']['label'] = t('Comment Body');
  $handler->display->display_options['fields']['comment']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['external'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['max_length'] = '200';
  $handler->display->display_options['fields']['comment']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['comment']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['comment']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['comment']['alter']['strip_tags'] = 1;
  $handler->display->display_options['fields']['comment']['alter']['trim'] = 1;
  $handler->display->display_options['fields']['comment']['alter']['html'] = 0;
  $handler->display->display_options['fields']['comment']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['comment']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['comment']['hide_empty'] = 0;
  $handler->display->display_options['fields']['comment']['empty_zero'] = 0;
  $handler->display->display_options['fields']['comment']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['comment']['field_api_classes'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = t('Page Title');
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Comment: Updated date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'comment';
  $handler->display->display_options['fields']['timestamp']['field'] = 'changed';
  $handler->display->display_options['fields']['timestamp']['label'] = t('Updated Date');
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['external'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'time ago';
  /* Field: Comment: Approve link */
  $handler->display->display_options['fields']['approve_comment']['id'] = 'approve_comment';
  $handler->display->display_options['fields']['approve_comment']['table'] = 'comment';
  $handler->display->display_options['fields']['approve_comment']['field'] = 'approve_comment';
  $handler->display->display_options['fields']['approve_comment']['label'] = t('Approve');
  $handler->display->display_options['fields']['approve_comment']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['external'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['approve_comment']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['approve_comment']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['approve_comment']['alter']['html'] = 0;
  $handler->display->display_options['fields']['approve_comment']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['approve_comment']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['approve_comment']['hide_empty'] = 0;
  $handler->display->display_options['fields']['approve_comment']['empty_zero'] = 0;
  $handler->display->display_options['fields']['approve_comment']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['approve_comment']['link_to_node'] = 0;
  /* Field: Comment: Delete link */
  $handler->display->display_options['fields']['delete_comment']['id'] = 'delete_comment';
  $handler->display->display_options['fields']['delete_comment']['table'] = 'comment';
  $handler->display->display_options['fields']['delete_comment']['field'] = 'delete_comment';
  $handler->display->display_options['fields']['delete_comment']['label'] = t('Delete');
  $handler->display->display_options['fields']['delete_comment']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['external'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delete_comment']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delete_comment']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delete_comment']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['delete_comment']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delete_comment']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delete_comment']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delete_comment']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['delete_comment']['link_to_node'] = 0;
  $handler->display->display_options['path'] = 'comments/review';

  $views[$view->name] = $view;
  return $views;
}
