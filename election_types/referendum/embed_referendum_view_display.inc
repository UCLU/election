<?php
/**
 * @file
 * Referendum-specific alterations to the 'election_posts' default View.
 *
 * The original election_posts View is provided by the Election Post module.
 *
 * This is part of an implementation of hook_views_default_views_alter().
 *
 * @see election_referendum_views_default_views_alter()
 */

/* Display: Referendum questions */
$handler = $view->new_display('embed', 'Referendum questions', 'embed_referendum');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Questions';
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['defaults']['empty'] = FALSE;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are currently no questions in this referendum';
$handler->display->display_options['empty']['area']['empty'] = TRUE;
$handler->display->display_options['empty']['area']['content'] = 'There are currently no questions in this referendum.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Question';
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
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Election: Published or admin */
$handler->display->display_options['filters']['published_or_admin']['id'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['table'] = 'election';
$handler->display->display_options['filters']['published_or_admin']['field'] = 'published_or_admin';
/* Filter criterion: Election: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'election';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'referendum';
