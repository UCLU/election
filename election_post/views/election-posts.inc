<?php
/**
 * @file
 * View listing election posts.
 */

$view = new view();
$view->name = 'election_posts';
$view->description = 'A list of posts (positions / questions) per election.';
$view->tag = 'election';
$view->base_table = 'election_post';
$view->human_name = 'Election posts';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Posts';
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view published elections';
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
  'eligibility' => 'eligibility',
  'view_link' => 'view_link',
  'vote_link' => 'view_link',
  'nominate_link' => 'view_link',
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
  'eligibility' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'view_link' => array(
    'align' => '',
    'separator' => ' · ',
    'empty_column' => 0,
  ),
  'vote_link' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nominate_link' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['empty_table'] = TRUE;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are currently no posts in this election';
$handler->display->display_options['empty']['area']['content'] = 'There are currently no posts in this election.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['link_to_post'] = 1;
/* Field: Election post: Eligibility */
$handler->display->display_options['fields']['eligibility']['id'] = 'eligibility';
$handler->display->display_options['fields']['eligibility']['table'] = 'election_post';
$handler->display->display_options['fields']['eligibility']['field'] = 'eligibility';
$handler->display->display_options['fields']['eligibility']['label'] = 'Your eligibility';
$handler->display->display_options['fields']['eligibility']['element_label_colon'] = FALSE;
/* Field: Election post: Link */
$handler->display->display_options['fields']['view_link']['id'] = 'view_link';
$handler->display->display_options['fields']['view_link']['table'] = 'election_post';
$handler->display->display_options['fields']['view_link']['field'] = 'view_link';
$handler->display->display_options['fields']['view_link']['label'] = 'Actions';
$handler->display->display_options['fields']['view_link']['element_wrapper_type'] = 'span';
$handler->display->display_options['fields']['view_link']['element_wrapper_class'] = 'election-action-link election-action-link-view';
$handler->display->display_options['fields']['view_link']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['view_link']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['view_link']['text'] = 'View details';
/* Field: Election post: Vote link */
$handler->display->display_options['fields']['vote_link']['id'] = 'vote_link';
$handler->display->display_options['fields']['vote_link']['table'] = 'election_post';
$handler->display->display_options['fields']['vote_link']['field'] = 'vote_link';
$handler->display->display_options['fields']['vote_link']['label'] = '';
$handler->display->display_options['fields']['vote_link']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['vote_link']['element_wrapper_type'] = 'span';
$handler->display->display_options['fields']['vote_link']['element_wrapper_class'] = 'election-action-link election-action-link-vote';
$handler->display->display_options['fields']['vote_link']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['vote_link']['hide_empty'] = TRUE;
/* Field: Election post: Nomination form link */
$handler->display->display_options['fields']['nominate_link']['id'] = 'nominate_link';
$handler->display->display_options['fields']['nominate_link']['table'] = 'election_post';
$handler->display->display_options['fields']['nominate_link']['field'] = 'nominate_link';
$handler->display->display_options['fields']['nominate_link']['label'] = '';
$handler->display->display_options['fields']['nominate_link']['element_label_colon'] = FALSE;
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
$handler->display->display_options['arguments']['election_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['election_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['election_id']['summary_options']['count'] = FALSE;
$handler->display->display_options['arguments']['election_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['election_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['election_id']['validate']['type'] = 'numeric';
/* Filter criterion: Election: Published or admin */
$handler->display->display_options['filters']['published_or_admin']['id'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['table'] = 'election';
$handler->display->display_options['filters']['published_or_admin']['field'] = 'published_or_admin';

/* Display: Embed */
$handler = $view->new_display('embed', 'Embed', 'embed');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

$views[$view->name] = $view;
