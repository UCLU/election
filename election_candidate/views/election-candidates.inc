<?php
/**
 * @file
 * View listing the candidates standing in an election.
 */

$view = new view;
$view->name = 'election_candidates';
$view->description = 'A list of candidates per election (intended for the public / all viewers of the election).';
$view->tag = 'default';
$view->base_table = 'election_candidate';
$view->human_name = 'Election candidates';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Candidates';
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
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'title',
    'rendered' => 0,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['row_plugin'] = 'fields';
/* Footer: Global: Text area */
$handler->display->display_options['footer']['area']['id'] = 'area';
$handler->display->display_options['footer']['area']['table'] = 'views';
$handler->display->display_options['footer']['area']['field'] = 'area';
$handler->display->display_options['footer']['area']['label'] = 'Note about random ordering';
$handler->display->display_options['footer']['area']['empty'] = FALSE;
$handler->display->display_options['footer']['area']['content'] = '<p class="election-candidate-random-order-note">If more than one candidate is standing for a position, they are listed in a random order.</p>';
$handler->display->display_options['footer']['area']['format'] = 'full_html';
$handler->display->display_options['footer']['area']['tokenize'] = 0;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are no candidates to display';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are no candidates to display.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
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
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title']['hide_empty'] = 1;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['title']['link_to_post'] = 1;
/* Field: Election candidate: Candidate ID */
$handler->display->display_options['fields']['candidate_id']['id'] = 'candidate_id';
$handler->display->display_options['fields']['candidate_id']['table'] = 'election_candidate';
$handler->display->display_options['fields']['candidate_id']['field'] = 'candidate_id';
$handler->display->display_options['fields']['candidate_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['external'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['candidate_id']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['candidate_id']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['candidate_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['candidate_id']['element_label_colon'] = 1;
$handler->display->display_options['fields']['candidate_id']['element_default_classes'] = 1;
$handler->display->display_options['fields']['candidate_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['candidate_id']['empty_zero'] = 0;
$handler->display->display_options['fields']['candidate_id']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['candidate_id']['format_plural'] = 0;
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
$handler->display->display_options['arguments']['election_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['election_id']['exception']['value'] = '';
$handler->display->display_options['arguments']['election_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['election_id']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['election_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['election_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['election_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['election_id']['specify_validation'] = 1;
$handler->display->display_options['arguments']['election_id']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['election_id']['break_phrase'] = 0;
$handler->display->display_options['arguments']['election_id']['not'] = 0;
/* Filter criterion: Election candidate: Status */
$handler->display->display_options['filters']['cstatus']['id'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['table'] = 'election_candidate';
$handler->display->display_options['filters']['cstatus']['field'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['value'] = array(
  1 => '1',
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'election/%/candidates/list';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'List candidates';
$handler->display->display_options['menu']['description'] = 'A list of the candidates standing in this election.';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'main-menu';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'Candidates';
$handler->display->display_options['tab_options']['weight'] = '3';

$views[$view->name] = $view;
