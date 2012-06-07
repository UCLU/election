<?php
/**
 * @file
 * View listing the candidates standing for an election post.
 */

$view = new view;
$view->name = 'candidates_post';
$view->description = 'A list of candidates standing for a particular election position.';
$view->tag = 'default';
$view->base_table = 'election_candidate';
$view->human_name = 'Candidates';
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
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are currently no approved candidates for this position';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are currently no approved candidates for this position.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
/* Field: Election candidate: Name */
$handler->display->display_options['fields']['fullname']['id'] = 'fullname';
$handler->display->display_options['fields']['fullname']['table'] = 'election_candidate';
$handler->display->display_options['fields']['fullname']['field'] = 'fullname';
$handler->display->display_options['fields']['fullname']['label'] = '';
$handler->display->display_options['fields']['fullname']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['external'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['fullname']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['fullname']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['trim'] = 0;
$handler->display->display_options['fields']['fullname']['alter']['html'] = 0;
$handler->display->display_options['fields']['fullname']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['fullname']['element_default_classes'] = 1;
$handler->display->display_options['fields']['fullname']['hide_empty'] = 0;
$handler->display->display_options['fields']['fullname']['empty_zero'] = 0;
$handler->display->display_options['fields']['fullname']['hide_alter_empty'] = 1;
/* Field: Election candidate: Profile link */
$handler->display->display_options['fields']['view_link']['id'] = 'view_link';
$handler->display->display_options['fields']['view_link']['table'] = 'election_candidate';
$handler->display->display_options['fields']['view_link']['field'] = 'view_link';
$handler->display->display_options['fields']['view_link']['label'] = '';
$handler->display->display_options['fields']['view_link']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['external'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['view_link']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['view_link']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['trim'] = 0;
$handler->display->display_options['fields']['view_link']['alter']['html'] = 0;
$handler->display->display_options['fields']['view_link']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['view_link']['element_default_classes'] = 1;
$handler->display->display_options['fields']['view_link']['hide_empty'] = 0;
$handler->display->display_options['fields']['view_link']['empty_zero'] = 0;
$handler->display->display_options['fields']['view_link']['hide_alter_empty'] = 1;
/* Contextual filter: Election post: Post ID */
$handler->display->display_options['arguments']['post_id']['id'] = 'post_id';
$handler->display->display_options['arguments']['post_id']['table'] = 'election_post';
$handler->display->display_options['arguments']['post_id']['field'] = 'post_id';
$handler->display->display_options['arguments']['post_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['post_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['post_id']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['post_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['post_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['post_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['post_id']['specify_validation'] = 1;
$handler->display->display_options['arguments']['post_id']['validate']['type'] = 'numeric';
$handler->display->display_options['arguments']['post_id']['break_phrase'] = 0;
$handler->display->display_options['arguments']['post_id']['not'] = 0;
/* Filter criterion: Election candidate: Status */
$handler->display->display_options['filters']['cstatus']['id'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['table'] = 'election_candidate';
$handler->display->display_options['filters']['cstatus']['field'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['value'] = array(
  1 => '1',
);

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'candidates_post_block');
$handler->display->display_options['block_description'] = 'Election candidates (per position)';

$views[$view->name] = $view;
