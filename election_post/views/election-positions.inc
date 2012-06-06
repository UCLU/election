<?php
/**
 * @file
 * View listing positions in an election.
 */

$view = new view;
$view->name = 'election_positions';
$view->description = 'A list of positions per election (intended for the public / all viewers of the election).';
$view->tag = 'default';
$view->base_table = 'election_post';
$view->human_name = 'Election positions';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Positions';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view any election';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['query']['options']['slave'] = TRUE;
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'vacancy_count' => 'vacancy_count',
);
$handler->display->display_options['style_options']['default'] = 'title';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'vacancy_count' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 1;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are no positions to display';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are no positions to display.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Position';
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
$handler->display->display_options['fields']['title']['link_to_post'] = 1;
/* Field: Election post: No. vacancies */
$handler->display->display_options['fields']['vacancy_count']['id'] = 'vacancy_count';
$handler->display->display_options['fields']['vacancy_count']['table'] = 'election_post';
$handler->display->display_options['fields']['vacancy_count']['field'] = 'vacancy_count';
$handler->display->display_options['fields']['vacancy_count']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['external'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['vacancy_count']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['vacancy_count']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['trim'] = 0;
$handler->display->display_options['fields']['vacancy_count']['alter']['html'] = 0;
$handler->display->display_options['fields']['vacancy_count']['element_label_colon'] = 1;
$handler->display->display_options['fields']['vacancy_count']['element_default_classes'] = 1;
$handler->display->display_options['fields']['vacancy_count']['hide_empty'] = 0;
$handler->display->display_options['fields']['vacancy_count']['empty_zero'] = 0;
$handler->display->display_options['fields']['vacancy_count']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['vacancy_count']['format_plural'] = 0;
/* Sort criterion: Election post: Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'election_post';
$handler->display->display_options['sorts']['title']['field'] = 'title';
/* Contextual filter: Election: Election ID */
$handler->display->display_options['arguments']['election_id']['id'] = 'election_id';
$handler->display->display_options['arguments']['election_id']['table'] = 'election';
$handler->display->display_options['arguments']['election_id']['field'] = 'election_id';
$handler->display->display_options['arguments']['election_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['election_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['election_id']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['election_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['election_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['election_id']['summary_options']['count'] = FALSE;
$handler->display->display_options['arguments']['election_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['election_id']['specify_validation'] = 1;
$handler->display->display_options['arguments']['election_id']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['election_id']['break_phrase'] = 0;
$handler->display->display_options['arguments']['election_id']['not'] = 0;
/* Filter criterion: Election: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'election';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'stv';
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type']['expose']['use_operator'] = FALSE;
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['remember'] = FALSE;
$handler->display->display_options['filters']['type']['expose']['multiple'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'election/%/posts-view';
$handler->display->display_options['menu']['title'] = 'List positions';
$handler->display->display_options['menu']['description'] = 'A list of the positions candidates can stand for in this election.';

$views[$view->name] = $view;