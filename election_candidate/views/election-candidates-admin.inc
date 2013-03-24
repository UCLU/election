<?php
/**
 * @file
 * View listing candidates in an election, for administrative use.
 */

$view = new view();
$view->name = 'election_candidates_admin';
$view->description = 'An administrative list of election candidates.';
$view->tag = 'election';
$view->base_table = 'election_candidate';
$view->human_name = 'Election candidates admin';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Manage candidates';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['slave'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '25';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'created' => 'created',
  'title' => 'title',
  'first_name' => 'first_name',
  'last_name' => 'last_name',
  'name' => 'name',
  'cstatus' => 'cstatus',
  'published' => 'published',
  'view_link' => 'view_link',
  'edit_link' => 'view_link',
);
$handler->display->display_options['style_options']['default'] = 'created';
$handler->display->display_options['style_options']['info'] = array(
  'created' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'first_name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'last_name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'cstatus' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'published' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'view_link' => array(
    'align' => 'views-align-center',
    'separator' => ' &middot; ',
    'empty_column' => 0,
  ),
  'edit_link' => array(
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
$handler->display->display_options['empty']['area']['label'] = 'No candidates found';
$handler->display->display_options['empty']['area']['empty'] = TRUE;
$handler->display->display_options['empty']['area']['content'] = 'No candidates found.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
/* Relationship: Election candidate: User */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'election_candidate';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
/* Field: Election candidate: Created date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'election_candidate';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Nominated';
$handler->display->display_options['fields']['created']['date_format'] = 'short';
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Position';
$handler->display->display_options['fields']['title']['alter']['max_length'] = '30';
$handler->display->display_options['fields']['title']['alter']['trim'] = TRUE;
$handler->display->display_options['fields']['title']['link_to_post'] = 0;
/* Field: Election candidate: First name */
$handler->display->display_options['fields']['first_name']['id'] = 'first_name';
$handler->display->display_options['fields']['first_name']['table'] = 'election_candidate';
$handler->display->display_options['fields']['first_name']['field'] = 'first_name';
/* Field: Election candidate: Last name */
$handler->display->display_options['fields']['last_name']['id'] = 'last_name';
$handler->display->display_options['fields']['last_name']['table'] = 'election_candidate';
$handler->display->display_options['fields']['last_name']['field'] = 'last_name';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid';
$handler->display->display_options['fields']['name']['label'] = 'User';
/* Field: Election candidate: Status */
$handler->display->display_options['fields']['cstatus']['id'] = 'cstatus';
$handler->display->display_options['fields']['cstatus']['table'] = 'election_candidate';
$handler->display->display_options['fields']['cstatus']['field'] = 'cstatus';
/* Field: Election candidate: Published */
$handler->display->display_options['fields']['published']['id'] = 'published';
$handler->display->display_options['fields']['published']['table'] = 'election_candidate';
$handler->display->display_options['fields']['published']['field'] = 'published';
$handler->display->display_options['fields']['published']['not'] = 0;
/* Field: Election candidate: Profile link */
$handler->display->display_options['fields']['view_link']['id'] = 'view_link';
$handler->display->display_options['fields']['view_link']['table'] = 'election_candidate';
$handler->display->display_options['fields']['view_link']['field'] = 'view_link';
$handler->display->display_options['fields']['view_link']['label'] = 'Actions';
$handler->display->display_options['fields']['view_link']['text'] = 'view';
/* Field: Election candidate: Edit link */
$handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
$handler->display->display_options['fields']['edit_link']['table'] = 'election_candidate';
$handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
$handler->display->display_options['fields']['edit_link']['text'] = 'edit';
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

/* Display: Embed */
$handler = $view->new_display('embed', 'Embed', 'embed');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

$views[$view->name] = $view;
