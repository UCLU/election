<?php
/**
 * @file
 * STV-specific alterations to the 'election_posts' default View.
 *
 * The original election_posts View is provided by the Election Post module.
 *
 * This is part of an implementation of hook_views_default_views_alter().
 *
 * @see election_stv_views_default_views_alter()
 */

/* Display: STV election positions */
$handler = $view->new_display('block', 'STV election positions', 'block_stv');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Positions';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'eligibility' => 'eligibility',
  'view_link' => 'view_link',
  'nominate_link' => 'view_link',
  'vote_link' => 'view_link',
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
  'nominate_link' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'vote_link' => array(
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
$handler->display->display_options['defaults']['empty'] = FALSE;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are currently no positions in this election';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are currently no positions in this election.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
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
/* Field: Election post: Eligibility */
$handler->display->display_options['fields']['eligibility']['id'] = 'eligibility';
$handler->display->display_options['fields']['eligibility']['table'] = 'election_post';
$handler->display->display_options['fields']['eligibility']['field'] = 'eligibility';
$handler->display->display_options['fields']['eligibility']['label'] = 'Your eligibility';
$handler->display->display_options['fields']['eligibility']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['external'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['eligibility']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['eligibility']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['trim'] = 0;
$handler->display->display_options['fields']['eligibility']['alter']['html'] = 0;
$handler->display->display_options['fields']['eligibility']['element_label_colon'] = 0;
$handler->display->display_options['fields']['eligibility']['element_default_classes'] = 1;
$handler->display->display_options['fields']['eligibility']['hide_empty'] = 0;
$handler->display->display_options['fields']['eligibility']['empty_zero'] = 0;
$handler->display->display_options['fields']['eligibility']['hide_alter_empty'] = 1;
/* Field: Election post: Link */
$handler->display->display_options['fields']['view_link']['id'] = 'view_link';
$handler->display->display_options['fields']['view_link']['table'] = 'election_post';
$handler->display->display_options['fields']['view_link']['field'] = 'view_link';
$handler->display->display_options['fields']['view_link']['label'] = 'Actions';
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
$handler->display->display_options['fields']['view_link']['element_label_colon'] = 1;
$handler->display->display_options['fields']['view_link']['element_wrapper_type'] = 'span';
$handler->display->display_options['fields']['view_link']['element_wrapper_class'] = 'election-action-link election-action-link-view';
$handler->display->display_options['fields']['view_link']['element_default_classes'] = 0;
$handler->display->display_options['fields']['view_link']['hide_empty'] = 1;
$handler->display->display_options['fields']['view_link']['empty_zero'] = 0;
$handler->display->display_options['fields']['view_link']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['view_link']['text'] = 'View details';
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
$handler->display->display_options['fields']['nominate_link']['element_wrapper_type'] = 'span';
$handler->display->display_options['fields']['nominate_link']['element_wrapper_class'] = 'election-action-link election-action-link-nominate';
$handler->display->display_options['fields']['nominate_link']['element_default_classes'] = 0;
$handler->display->display_options['fields']['nominate_link']['hide_empty'] = 0;
$handler->display->display_options['fields']['nominate_link']['empty_zero'] = 0;
$handler->display->display_options['fields']['nominate_link']['hide_alter_empty'] = 1;
/* Field: Election post: Vote link */
$handler->display->display_options['fields']['vote_link']['id'] = 'vote_link';
$handler->display->display_options['fields']['vote_link']['table'] = 'election_post';
$handler->display->display_options['fields']['vote_link']['field'] = 'vote_link';
$handler->display->display_options['fields']['vote_link']['label'] = '';
$handler->display->display_options['fields']['vote_link']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['external'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['replace_spaces'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['trim_whitespace'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['vote_link']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['vote_link']['alter']['more_link'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['trim'] = 0;
$handler->display->display_options['fields']['vote_link']['alter']['html'] = 0;
$handler->display->display_options['fields']['vote_link']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['vote_link']['element_wrapper_type'] = 'span';
$handler->display->display_options['fields']['vote_link']['element_wrapper_class'] = 'election-action-link election-action-link-vote';
$handler->display->display_options['fields']['vote_link']['element_default_classes'] = 0;
$handler->display->display_options['fields']['vote_link']['hide_empty'] = 1;
$handler->display->display_options['fields']['vote_link']['empty_zero'] = 0;
$handler->display->display_options['fields']['vote_link']['hide_alter_empty'] = 1;
$handler->display->display_options['fields']['vote_link']['text'] = 'Vote';
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Election: Published or admin */
$handler->display->display_options['filters']['published_or_admin']['id'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['table'] = 'election';
$handler->display->display_options['filters']['published_or_admin']['field'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['group'] = 1;
/* Filter criterion: Election: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'election';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'stv';
$handler->display->display_options['filters']['type']['group'] = 1;
