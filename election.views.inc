<?php
/**
 * @file
 * Views integration for the Election module.
 */

/**
 * Implements hook_views_default_views().
 */
function election_views_default_views() {
  $views_dir = drupal_get_path('module', 'election') . '/views';
  $views_files = scandir($views_dir);
  $views = array();
  foreach ($views_files as $filename) {
    if (substr($filename, -4) == '.inc') {
      include_once $views_dir . '/' . $filename;
    }
  }
  return $views;
}

/**
 * Implements hook_views_query_substitutions().
 */
function election_views_query_substitutions() {
  return array(
    '***EDIT_ANY_ELECTION***' => intval(user_access('edit any election')),
  );
}

/**
 * Implements hook_views_data().
 */
function election_views_data() {

  $data = array();

  $data['election']['table']['group']  = t('Election');

  $data['election']['table']['entity type'] = 'election';

  $data['election']['table']['base'] = array(
    'field' => 'election_id',
    'title' => t('Election'),
    'weight' => -10,
    'defaults' => array(
      'field' => 'title',
    ),
  );

  $data['election']['election_id'] = array(
    'title' => t('Election ID'),
    'help' => t('The unique ID of the election.'),
    'field' => array(
      'handler' => 'views_handler_field_election',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_election_electionid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'election_id',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the election.'),
    'field' => array(
      'handler' => 'views_handler_field_election',
      'click sortable' => TRUE,
      'link_to_election default' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['election']['published'] = array(
    'title' => t('Published'),
    'help' => t('Whether or not the election is published.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'published-notpublished' => array(t('Published'), t('Not published')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election']['published_or_admin'] = array(
    'title' => t('Published or admin'),
    'help' => t('Filters out unpublished content if the current user cannot view it.'),
    'filter' => array(
      'field' => 'status',
      'handler' => 'views_handler_filter_election_published',
      'label' => t('Published or admin'),
    ),
  );

  $data['election']['vstatus'] = array(
    'title' => t('Voting status'),
    'help' => t('The voting status for this election.'),
    'field' => array(
      'handler' => 'views_handler_field_election_vstatus',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'label' => t('Voting is open'),
      'handler' => 'views_handler_filter_election_vstatus',
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election']['nstatus'] = array(
    'title' => t('Nominations status'),
    'help' => t('Whether nominations are open for this election.'),
    'field' => array(
      'handler' => 'views_handler_field_election_nstatus',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'label' => t('Nominations are open'),
      'handler' => 'views_handler_filter_election_nstatus',
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election']['type'] = array(
    'title' => t('Type'),
    'help' => t('The election type (for example, "STV Election" or "Referendum").'),
    'field' => array(
      'handler' => 'views_handler_field_election_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_election_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date when the election was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['election']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date when the election settings were last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['election']['vopen_time'] = array(
    'title' => t('Voting opening date'),
    'help' => t('The date when voting opens for this election.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['election']['vclose_time'] = array(
    'title' => t('Voting closing date'),
    'help' => t('The date when voting closes for this election.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['election']['nopen_time'] = array(
    'title' => t('Nominations opening date'),
    'help' => t('The date when nominations open for this election.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['election']['nclose_time'] = array(
    'title' => t('Nominations closing date'),
    'help' => t('The date when nominations close for this election.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['election']['view_election'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the election.'),
      'handler' => 'views_handler_field_election_link',
    ),
  );

  $data['election']['edit_election'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the election.'),
      'handler' => 'views_handler_field_election_link_edit',
    ),
  );

  $data['election']['delete_election'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the election.'),
      'handler' => 'views_handler_field_election_link_delete',
    ),
  );

  $data['election']['posts_link'] = array(
    'field' => array(
      'title' => t('Posts link'),
      'help' => t('A link to the list of posts/positions/motions for the election.'),
      'handler' => 'views_handler_field_election_link_posts',
    ),
  );

  $data['election']['candidates_link'] = array(
    'field' => array(
      'title' => t('Candidates link'),
      'help' => t('A link to the candidates page, where relevant.'),
      'handler' => 'views_handler_field_election_link_candidates',
    ),
  );

  $data['election']['nominate_link'] = array(
    'field' => array(
      'title' => t('Nomination form link'),
      'help' => t('A link to the nomination form, where relevant.'),
      'handler' => 'views_handler_field_election_link_nominate',
    ),
  );

  return $data;

}
