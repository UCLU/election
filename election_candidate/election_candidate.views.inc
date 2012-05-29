<?php
/**
 * @file
 * Views integration for the Election Candidate module.
 */

/**
 * Implements hook_views_data().
 */
function election_candidate_views_data() {

  $data = array();

  // ----------------------------------------------------------------
  // election candidates table -- basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['election_candidate']['table']['group']  = t('Election candidate');

  // Advertise this table as a possible base table
  $data['election_candidate']['table']['base'] = array(
    'field' => 'candidate_id',
    'title' => t('Election candidate'),
    'entity type' => 'election_candidate',
    'weight' => -8,
//    'access query tag' => 'election_access', // @todo work out how to adapt this
    'defaults' => array(
      'field' => 'candidate_id',
    ),
  );

  // Join candidates to elections
  $data['election']['table']['join']['election_candidate'] = array(
    'left_field' => 'election_id',
    'field' => 'election_id',
  );
  // Join candidates to posts
  $data['election_post']['table']['join']['election_candidate'] = array(
    'left_field' => 'post_id',
    'field' => 'post_id',
  );

  // candidate_id
  $data['election_candidate']['candidate_id'] = array(
    'title' => t('Candidate ID'),
    'help' => t('The unique ID of the election candidate.'), // The help that appears on the UI,
    // Information for displaying the candidate_id
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a candidate_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a candidate_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // candidate status (cstatus)
  $data['election_candidate']['cstatus'] = array(
    'title' => t('Status'),
    'help' => t('The status of the candidate.'),
    'field' => array(
      'handler' => 'views_handler_field_election_candidate_cstatus',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'label' => t('Candidate status'),
      'handler' => 'views_handler_filter_election_candidate_cstatus',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;

}
