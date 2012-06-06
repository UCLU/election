<?php
/**
 * @file
 * Views integration for the Election module.
 */

/**
 * Implements hook_views_data().
 */
function election_views_data() {

  $data = array();

  // ----------------------------------------------------------------
  // election table -- basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['election']['table']['group']  = t('Election');

  // Advertise this table as a possible base table
  $data['election']['table']['base'] = array(
    'field' => 'election_id',
    'title' => t('Election'),
    'entity type' => 'election',
    'weight' => -10,
//    'access query tag' => 'election_access', // @todo work out how to adapt this
    'defaults' => array(
      'field' => 'title',
    ),
  );

  // ----------------------------------------------------------------
  // election table -- fields

  // election_id
  $data['election']['election_id'] = array(
    'title' => t('Election ID'),
    'help' => t('The unique ID of the election.'), // The help that appears on the UI,
    // Information for displaying the election_id
    'field' => array(
      'handler' => 'views_handler_field_election',
      'click sortable' => TRUE,
    ),
    // Information for accepting an election_id as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_election_electionid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'election_id',
    ),
    // Information for accepting an election_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on an election_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // title
  $data['election']['title'] = array(
    'title' => t('Title'), // The item it appears as on the UI,
    'help' => t('The title of the election.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_election',
      'click sortable' => TRUE,
      'link_to_election default' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // voting status
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

  // nominations status
  // @todo make the filter always true if it's a referendum.
  $data['election']['nstatus'] = array(
    'title' => t('Nominations status'),
    'help' => t('Whether nominations are open for this election (not relevant for referendums).'),
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

  // type
  $data['election']['type'] = array(
    'title' => t('Type'), // The item it appears as on the UI,
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

  // created
  $data['election']['created'] = array(
    'title' => t('Created date'), // The item it appears as on the UI,
    'help' => t('The date when the election was created.'), // The help that appears on the UI,
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

  // changed
  $data['election']['changed'] = array(
    'title' => t('Updated date'), // The item it appears as on the UI,
    'help' => t('The date when the election settings were last updated.'), // The help that appears on the UI,
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

  // vopen_time
  $data['election']['vopen_time'] = array(
    'title' => t('Voting opening date'), // The item it appears as on the UI,
    'help' => t('The date when voting opens for this election.'), // The help that appears on the UI,
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

  // vclose_time
  $data['election']['vclose_time'] = array(
    'title' => t('Voting closing date'), // The item it appears as on the UI,
    'help' => t('The date when voting closes for this election.'), // The help that appears on the UI,
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

  // nopen_time
  $data['election']['nopen_time'] = array(
    'title' => t('Nominations opening date'), // The item it appears as on the UI,
    'help' => t('The date when nominations open for this election (not relevant for referendums).'), // The help that appears on the UI,
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

  // nclose_time
  $data['election']['nclose_time'] = array(
    'title' => t('Nominations closing date'), // The item it appears as on the UI,
    'help' => t('The date when nominations close for this election (not relevant for referendums).'), // The help that appears on the UI,
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

  // election links
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

  return $data;

}
