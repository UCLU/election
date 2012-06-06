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
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view any election';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['query']['options']['slave'] = TRUE;
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'type' => 'type',
  'vopen_time' => 'vopen_time',
  'vclose_time' => 'vclose_time',
  'view_election' => 'view_election',
  'posts_link' => 'view_election',
  'candidates_link' => 'view_election',
  'nominate_link' => 'view_election',
  'edit_election' => 'view_election',
  'delete_election' => 'view_election',
  'created' => 'created',
);
$handler->display->display_options['style_options']['default'] = 'created';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'vopen_time' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
  'vclose_time' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
  'view_election' => array(
    'align' => '',
    'separator' => ' · ',
    'empty_column' => 1,
  ),
  'posts_link' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'candidates_link' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nominate_link' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'edit_election' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'delete_election' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'created' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
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
$handler->display->display_options['empty']['area']['label'] = 'There are no elections to display';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are no elections to display.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Field: Election: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Election: Created date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'election';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Created';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['created']['alter']['external'] = 0;
$handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['element_label_colon'] = 1;
$handler->display->display_options['fields']['created']['element_default_classes'] = 1;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
$handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['created']['date_format'] = 'custom';
$handler->display->display_options['fields']['created']['custom_date_format'] = 'jS F Y';
/* Field: Election: Voting opening date */
$handler->display->display_options['fields']['vopen_time']['id'] = 'vopen_time';
$handler->display->display_options['fields']['vopen_time']['table'] = 'election';
$handler->display->display_options['fields']['vopen_time']['field'] = 'vopen_time';
$handler->display->display_options['fields']['vopen_time']['label'] = 'Opening date';
$handler->display->display_options['fields']['vopen_time']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['external'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['vopen_time']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['vopen_time']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['trim'] = 0;
$handler->display->display_options['fields']['vopen_time']['alter']['html'] = 0;
$handler->display->display_options['fields']['vopen_time']['element_label_colon'] = 1;
$handler->display->display_options['fields']['vopen_time']['element_default_classes'] = 1;
$handler->display->display_options['fields']['vopen_time']['hide_empty'] = 0;
$handler->display->display_options['fields']['vopen_time']['empty_zero'] = 0;
$handler->display->display_options['fields']['vopen_time']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['vopen_time']['date_format'] = 'short';
/* Field: Election: Voting closing date */
$handler->display->display_options['fields']['vclose_time']['id'] = 'vclose_time';
$handler->display->display_options['fields']['vclose_time']['table'] = 'election';
$handler->display->display_options['fields']['vclose_time']['field'] = 'vclose_time';
$handler->display->display_options['fields']['vclose_time']['label'] = 'Closing date';
$handler->display->display_options['fields']['vclose_time']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['external'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['vclose_time']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['vclose_time']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['trim'] = 0;
$handler->display->display_options['fields']['vclose_time']['alter']['html'] = 0;
$handler->display->display_options['fields']['vclose_time']['element_label_colon'] = 1;
$handler->display->display_options['fields']['vclose_time']['element_default_classes'] = 1;
$handler->display->display_options['fields']['vclose_time']['hide_empty'] = 0;
$handler->display->display_options['fields']['vclose_time']['empty_zero'] = 0;
$handler->display->display_options['fields']['vclose_time']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['vclose_time']['date_format'] = 'short';
/* Field: Election: Link */
$handler->display->display_options['fields']['view_election']['id'] = 'view_election';
$handler->display->display_options['fields']['view_election']['table'] = 'election';
$handler->display->display_options['fields']['view_election']['field'] = 'view_election';
$handler->display->display_options['fields']['view_election']['label'] = 'Actions';
$handler->display->display_options['fields']['view_election']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['external'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['view_election']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['view_election']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['trim'] = 0;
$handler->display->display_options['fields']['view_election']['alter']['html'] = 0;
$handler->display->display_options['fields']['view_election']['element_label_colon'] = 1;
$handler->display->display_options['fields']['view_election']['element_default_classes'] = 1;
$handler->display->display_options['fields']['view_election']['hide_empty'] = 0;
$handler->display->display_options['fields']['view_election']['empty_zero'] = 0;
$handler->display->display_options['fields']['view_election']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['view_election']['text'] = 'View details';
/* Field: Election: Posts link */
$handler->display->display_options['fields']['posts_link']['id'] = 'posts_link';
$handler->display->display_options['fields']['posts_link']['table'] = 'election';
$handler->display->display_options['fields']['posts_link']['field'] = 'posts_link';
$handler->display->display_options['fields']['posts_link']['label'] = '';
$handler->display->display_options['fields']['posts_link']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['external'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['posts_link']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['posts_link']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['trim'] = 0;
$handler->display->display_options['fields']['posts_link']['alter']['html'] = 0;
$handler->display->display_options['fields']['posts_link']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['posts_link']['element_default_classes'] = 1;
$handler->display->display_options['fields']['posts_link']['hide_empty'] = 0;
$handler->display->display_options['fields']['posts_link']['empty_zero'] = 0;
$handler->display->display_options['fields']['posts_link']['hide_alter_empty'] = 1;
/* Field: Election: Candidates link */
$handler->display->display_options['fields']['candidates_link']['id'] = 'candidates_link';
$handler->display->display_options['fields']['candidates_link']['table'] = 'election';
$handler->display->display_options['fields']['candidates_link']['field'] = 'candidates_link';
$handler->display->display_options['fields']['candidates_link']['label'] = '';
$handler->display->display_options['fields']['candidates_link']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['external'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['candidates_link']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['candidates_link']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['trim'] = 0;
$handler->display->display_options['fields']['candidates_link']['alter']['html'] = 0;
$handler->display->display_options['fields']['candidates_link']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['candidates_link']['element_default_classes'] = 1;
$handler->display->display_options['fields']['candidates_link']['hide_empty'] = 0;
$handler->display->display_options['fields']['candidates_link']['empty_zero'] = 0;
$handler->display->display_options['fields']['candidates_link']['hide_alter_empty'] = 1;
/* Field: Election: Nomination form link */
$handler->display->display_options['fields']['nominate_link']['id'] = 'nominate_link';
$handler->display->display_options['fields']['nominate_link']['table'] = 'election';
$handler->display->display_options['fields']['nominate_link']['field'] = 'nominate_link';
$handler->display->display_options['fields']['nominate_link']['label'] = '';
$handler->display->display_options['fields']['nominate_link']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['external'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['nominate_link']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['nominate_link']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['trim'] = 0;
$handler->display->display_options['fields']['nominate_link']['alter']['html'] = 0;
$handler->display->display_options['fields']['nominate_link']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['nominate_link']['element_default_classes'] = 1;
$handler->display->display_options['fields']['nominate_link']['hide_empty'] = 0;
$handler->display->display_options['fields']['nominate_link']['empty_zero'] = 0;
$handler->display->display_options['fields']['nominate_link']['hide_alter_empty'] = 1;
/* Field: Election: Edit link */
$handler->display->display_options['fields']['edit_election']['id'] = 'edit_election';
$handler->display->display_options['fields']['edit_election']['table'] = 'election';
$handler->display->display_options['fields']['edit_election']['field'] = 'edit_election';
$handler->display->display_options['fields']['edit_election']['label'] = '';
$handler->display->display_options['fields']['edit_election']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['external'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_election']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_election']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_election']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_election']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['edit_election']['element_default_classes'] = 1;
$handler->display->display_options['fields']['edit_election']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_election']['empty_zero'] = 0;
$handler->display->display_options['fields']['edit_election']['hide_alter_empty'] = 1;
/* Field: Election: Delete link */
$handler->display->display_options['fields']['delete_election']['id'] = 'delete_election';
$handler->display->display_options['fields']['delete_election']['table'] = 'election';
$handler->display->display_options['fields']['delete_election']['field'] = 'delete_election';
$handler->display->display_options['fields']['delete_election']['label'] = '';
$handler->display->display_options['fields']['delete_election']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['external'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['delete_election']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['delete_election']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['trim'] = 0;
$handler->display->display_options['fields']['delete_election']['alter']['html'] = 0;
$handler->display->display_options['fields']['delete_election']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['delete_election']['element_default_classes'] = 1;
$handler->display->display_options['fields']['delete_election']['hide_empty'] = 0;
$handler->display->display_options['fields']['delete_election']['empty_zero'] = 0;
$handler->display->display_options['fields']['delete_election']['hide_alter_empty'] = 1;
/* Sort criterion: Election: Created date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'election';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
$handler->display->display_options['sorts']['created']['expose']['label'] = 'Created date';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'elections';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Elections and Referendums';


$views[$view->name] = $view;
