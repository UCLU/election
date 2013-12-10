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
$handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
$handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '25';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'views_bulk_operations' => 'views_bulk_operations',
  'changed' => 'changed',
  'title' => 'title',
  'first_name' => 'first_name',
  'last_name' => 'last_name',
  'name' => 'name',
  'cstatus' => 'cstatus',
  'published' => 'published',
  'view_link' => 'view_link',
  'edit_link' => 'view_link',
);
$handler->display->display_options['style_options']['default'] = 'changed';
$handler->display->display_options['style_options']['info'] = array(
  'views_bulk_operations' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'changed' => array(
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
$handler->display->display_options['style_options']['sticky'] = TRUE;
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
/* Field: Bulk operations: Election candidate */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'election_candidate';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_script_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_modify_action' => array(
    'selected' => 1,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 1,
    'label' => 'Change candidate status',
    'settings' => array(
      'show_all_tokens' => 1,
      'display_values' => array(
        'cstatus' => 'cstatus',
      ),
    ),
  ),
  'action::views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'url' => '',
    ),
  ),
  'action::election_candidate_publish_action' => array(
    'selected' => 1,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_send_email_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::election_candidate_unpublish_action' => array(
    'selected' => 1,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
/* Field: Election candidate: Updated date */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'election_candidate';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['label'] = 'Updated';
$handler->display->display_options['fields']['changed']['date_format'] = 'short';
/* Field: Election post: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'election_post';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Position';
$handler->display->display_options['fields']['title']['alter']['max_length'] = '40';
$handler->display->display_options['fields']['title']['alter']['trim'] = TRUE;
$handler->display->display_options['fields']['title']['link_to_post'] = 1;
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
/* Filter criterion: Election post: Title */
$handler->display->display_options['filters']['title']['id'] = 'title';
$handler->display->display_options['filters']['title']['table'] = 'election_post';
$handler->display->display_options['filters']['title']['field'] = 'title';
$handler->display->display_options['filters']['title']['operator'] = 'allwords';
$handler->display->display_options['filters']['title']['group'] = 1;
$handler->display->display_options['filters']['title']['exposed'] = TRUE;
$handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['label'] = 'Position';
$handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
$handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Election candidate: First name */
$handler->display->display_options['filters']['first_name']['id'] = 'first_name';
$handler->display->display_options['filters']['first_name']['table'] = 'election_candidate';
$handler->display->display_options['filters']['first_name']['field'] = 'first_name';
$handler->display->display_options['filters']['first_name']['operator'] = 'allwords';
$handler->display->display_options['filters']['first_name']['group'] = 1;
$handler->display->display_options['filters']['first_name']['exposed'] = TRUE;
$handler->display->display_options['filters']['first_name']['expose']['operator_id'] = 'first_name_op';
$handler->display->display_options['filters']['first_name']['expose']['label'] = 'First name';
$handler->display->display_options['filters']['first_name']['expose']['operator'] = 'first_name_op';
$handler->display->display_options['filters']['first_name']['expose']['identifier'] = 'first_name';
$handler->display->display_options['filters']['first_name']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Election candidate: Last name */
$handler->display->display_options['filters']['last_name']['id'] = 'last_name';
$handler->display->display_options['filters']['last_name']['table'] = 'election_candidate';
$handler->display->display_options['filters']['last_name']['field'] = 'last_name';
$handler->display->display_options['filters']['last_name']['operator'] = 'allwords';
$handler->display->display_options['filters']['last_name']['group'] = 1;
$handler->display->display_options['filters']['last_name']['exposed'] = TRUE;
$handler->display->display_options['filters']['last_name']['expose']['operator_id'] = 'last_name_op';
$handler->display->display_options['filters']['last_name']['expose']['label'] = 'Last name';
$handler->display->display_options['filters']['last_name']['expose']['operator'] = 'last_name_op';
$handler->display->display_options['filters']['last_name']['expose']['identifier'] = 'last_name';
$handler->display->display_options['filters']['last_name']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Election candidate: Published */
$handler->display->display_options['filters']['published']['id'] = 'published';
$handler->display->display_options['filters']['published']['table'] = 'election_candidate';
$handler->display->display_options['filters']['published']['field'] = 'published';
$handler->display->display_options['filters']['published']['value'] = 'All';
$handler->display->display_options['filters']['published']['group'] = 1;
$handler->display->display_options['filters']['published']['exposed'] = TRUE;
$handler->display->display_options['filters']['published']['expose']['operator_id'] = '';
$handler->display->display_options['filters']['published']['expose']['label'] = 'Published';
$handler->display->display_options['filters']['published']['expose']['operator'] = 'published_op';
$handler->display->display_options['filters']['published']['expose']['identifier'] = 'published';
$handler->display->display_options['filters']['published']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: Election candidate: Status */
$handler->display->display_options['filters']['cstatus']['id'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['table'] = 'election_candidate';
$handler->display->display_options['filters']['cstatus']['field'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['group'] = 1;
$handler->display->display_options['filters']['cstatus']['exposed'] = TRUE;
$handler->display->display_options['filters']['cstatus']['expose']['operator_id'] = 'cstatus_op';
$handler->display->display_options['filters']['cstatus']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['cstatus']['expose']['operator'] = 'cstatus_op';
$handler->display->display_options['filters']['cstatus']['expose']['identifier'] = 'cstatus';
$handler->display->display_options['filters']['cstatus']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);

/* Display: Embed */
$handler = $view->new_display('embed', 'Embed', 'embed');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

$views[$view->name] = $view;
