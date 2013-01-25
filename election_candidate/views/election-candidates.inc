<?php
/**
 * @file
 * View listing candidates standing in an election, per election or per post.
 */

$view = new view();
$view->name = 'election_candidates';
$view->description = 'A list of candidates (intended for the public / all viewers of the election).';
$view->tag = 'election';
$view->base_table = 'election_candidate';
$view->human_name = 'Election candidates';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Candidates';
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
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['style_options']['row_class'] = 'election-candidate-row clearfix';
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['default_field_elements'] = FALSE;
/* Footer: Global: Text area */
$handler->display->display_options['footer']['area']['id'] = 'area';
$handler->display->display_options['footer']['area']['table'] = 'views';
$handler->display->display_options['footer']['area']['field'] = 'area';
$handler->display->display_options['footer']['area']['label'] = 'Note about random ordering';
$handler->display->display_options['footer']['area']['empty'] = FALSE;
$handler->display->display_options['footer']['area']['content'] = '<p class="election-candidate-random-order-note">If more than one candidate is standing for a position, they are listed in a random order.</p>';
$handler->display->display_options['footer']['area']['format'] = 'full_html';
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are currently no published candidate profiles for this election';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are currently no published candidate profiles for this election.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Field: Election candidate: Name */
$handler->display->display_options['fields']['fullname']['id'] = 'fullname';
$handler->display->display_options['fields']['fullname']['table'] = 'election_candidate';
$handler->display->display_options['fields']['fullname']['field'] = 'fullname';
$handler->display->display_options['fields']['fullname']['label'] = '';
$handler->display->display_options['fields']['fullname']['element_type'] = 'span';
$handler->display->display_options['fields']['fullname']['element_class'] = 'election-candidate-name';
$handler->display->display_options['fields']['fullname']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['fullname']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['fullname']['link_to_candidate'] = 1;
/* Sort criterion: Election post: Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'election_post';
$handler->display->display_options['sorts']['title']['field'] = 'title';
/* Sort criterion: Global: Random */
$handler->display->display_options['sorts']['random']['id'] = 'random';
$handler->display->display_options['sorts']['random']['table'] = 'views';
$handler->display->display_options['sorts']['random']['field'] = 'random';
/* Contextual filter: Election: Election ID */
$handler->display->display_options['arguments']['election_id']['id'] = 'election_id';
$handler->display->display_options['arguments']['election_id']['table'] = 'election';
$handler->display->display_options['arguments']['election_id']['field'] = 'election_id';
$handler->display->display_options['arguments']['election_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['election_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['election_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['election_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['election_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['election_id']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['election_id']['validate']['fail'] = 'empty';
/* Filter criterion: Election: Published or admin */
$handler->display->display_options['filters']['published_or_admin']['id'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['table'] = 'election';
$handler->display->display_options['filters']['published_or_admin']['field'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['group'] = 1;
/* Filter criterion: Election candidate: Published */
$handler->display->display_options['filters']['published']['id'] = 'published';
$handler->display->display_options['filters']['published']['table'] = 'election_candidate';
$handler->display->display_options['filters']['published']['field'] = 'published';
$handler->display->display_options['filters']['published']['value'] = '1';
/* Filter criterion: Election candidate: Status */
$handler->display->display_options['filters']['cstatus']['id'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['table'] = 'election_candidate';
$handler->display->display_options['filters']['cstatus']['field'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['operator'] = 'not in';
$handler->display->display_options['filters']['cstatus']['value'] = array(
  -2 => '-2',
);

/* Display: Candidates per election */
$handler = $view->new_display('page', 'Candidates per election', 'candidates_per_election');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'title',
    'rendered' => 0,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['row_class'] = 'election-candidate-row clearfix';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['default_field_elements'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Standing for';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['link_to_post'] = 1;
/* Field: Election candidate: Name */
$handler->display->display_options['fields']['fullname']['id'] = 'fullname';
$handler->display->display_options['fields']['fullname']['table'] = 'election_candidate';
$handler->display->display_options['fields']['fullname']['field'] = 'fullname';
$handler->display->display_options['fields']['fullname']['label'] = '';
$handler->display->display_options['fields']['fullname']['element_type'] = 'span';
$handler->display->display_options['fields']['fullname']['element_class'] = 'election-candidate-name';
$handler->display->display_options['fields']['fullname']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['fullname']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['fullname']['link_to_candidate'] = 1;
$handler->display->display_options['path'] = 'election/%/candidates-view';
$handler->display->display_options['menu']['title'] = 'List candidates';
$handler->display->display_options['menu']['description'] = 'A list of the candidates standing in this election.';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'Candidates';
$handler->display->display_options['tab_options']['weight'] = '3';

/* Display: Candidates per post */
$handler = $view->new_display('block', 'Candidates per post', 'candidates_per_post');
$handler->display->display_options['defaults']['footer'] = FALSE;
/* Footer: Global: Text area */
$handler->display->display_options['footer']['area']['id'] = 'area';
$handler->display->display_options['footer']['area']['table'] = 'views';
$handler->display->display_options['footer']['area']['field'] = 'area';
$handler->display->display_options['footer']['area']['label'] = 'Note about random ordering';
$handler->display->display_options['footer']['area']['empty'] = FALSE;
$handler->display->display_options['footer']['area']['content'] = '<p class="election-candidate-random-order-note">Candidates are listed in a random order.</p>';
$handler->display->display_options['footer']['area']['format'] = 'full_html';
$handler->display->display_options['defaults']['empty'] = FALSE;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are currently no published candidate profiles for this position';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are currently no published candidate profiles for this position.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Global: Random */
$handler->display->display_options['sorts']['random']['id'] = 'random';
$handler->display->display_options['sorts']['random']['table'] = 'views';
$handler->display->display_options['sorts']['random']['field'] = 'random';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Election post: Post ID */
$handler->display->display_options['arguments']['post_id']['id'] = 'post_id';
$handler->display->display_options['arguments']['post_id']['table'] = 'election_post';
$handler->display->display_options['arguments']['post_id']['field'] = 'post_id';
$handler->display->display_options['arguments']['post_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['post_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['post_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['post_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['post_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['post_id']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['post_id']['validate']['fail'] = 'empty';

$views[$view->name] = $view;
