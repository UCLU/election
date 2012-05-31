<?php
/**
 * @file
 * View listing all elections.
 */

$view = new view;
$view->name = 'elections';
$view->description = 'A publicly viewable list of elections and referendums';
$view->tag = 'default';
$view->base_table = 'election';
$view->human_name = 'Elections and Referendums';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Elections and Referendums';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'type' => 'type',
  'vstatus' => 'vstatus',
);
$handler->display->display_options['style_options']['default'] = 'title';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'vstatus' => array(
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
/* Field: Election: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Election: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'election';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['type']['alter']['external'] = 0;
$handler->display->display_options['fields']['type']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['type']['alter']['html'] = 0;
$handler->display->display_options['fields']['type']['element_label_colon'] = 1;
$handler->display->display_options['fields']['type']['element_default_classes'] = 1;
$handler->display->display_options['fields']['type']['hide_empty'] = 0;
$handler->display->display_options['fields']['type']['empty_zero'] = 0;
$handler->display->display_options['fields']['type']['hide_alter_empty'] = 0;
$handler->display->display_options['fields']['type']['machine_name'] = 0;
/* Field: Election: Voting status */
$handler->display->display_options['fields']['vstatus']['id'] = 'vstatus';
$handler->display->display_options['fields']['vstatus']['table'] = 'election';
$handler->display->display_options['fields']['vstatus']['field'] = 'vstatus';
$handler->display->display_options['fields']['vstatus']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['external'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['vstatus']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['vstatus']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['trim'] = 0;
$handler->display->display_options['fields']['vstatus']['alter']['html'] = 0;
$handler->display->display_options['fields']['vstatus']['element_label_colon'] = 1;
$handler->display->display_options['fields']['vstatus']['element_default_classes'] = 1;
$handler->display->display_options['fields']['vstatus']['hide_empty'] = 0;
$handler->display->display_options['fields']['vstatus']['empty_zero'] = 0;
$handler->display->display_options['fields']['vstatus']['hide_alter_empty'] = 0;
/* Field: Election: Nominations status */
$handler->display->display_options['fields']['nstatus']['id'] = 'nstatus';
$handler->display->display_options['fields']['nstatus']['table'] = 'election';
$handler->display->display_options['fields']['nstatus']['field'] = 'nstatus';
$handler->display->display_options['fields']['nstatus']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['external'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nstatus']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nstatus']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nstatus']['alter']['html'] = 0;
$handler->display->display_options['fields']['nstatus']['element_label_colon'] = 1;
$handler->display->display_options['fields']['nstatus']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nstatus']['hide_empty'] = 0;
$handler->display->display_options['fields']['nstatus']['empty_zero'] = 0;
$handler->display->display_options['fields']['nstatus']['hide_alter_empty'] = 0;
/* Sort criterion: Election: Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'election';
$handler->display->display_options['sorts']['title']['field'] = 'title';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'elections';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Elections and Referendums';

$views[$view->name] = $view;
