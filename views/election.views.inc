<?php
/**
 * @file
 * Views integration for the Election module.
 *
 * @todo this is totally unfinished and doesn't work at all.
 */


function election_views_data_alter(&$data) {

  // ----------------------------------------------------------------
  // election table -- basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['election']['table']['group']  = t('Elections');

  // Advertise this table as a possible base table
  $data['election']['table']['base'] = array(
    'field' => 'election_id',
    'title' => t('Election'),
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
      'handler' => 'views_handler_field', // @todo create a handler
      'click sortable' => TRUE, // @todo understand this
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
      'handler' => 'views_handler_field', // @todo create a handler
      'click sortable' => TRUE, // @todo understand this
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

  // ----------------------------------------------------------------
  // election table -- fields hidden from Views

  // anonymous
  $data['election']['anonymous'] = array();

  return $data;

}
