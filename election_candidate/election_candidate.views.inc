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

  $data['election_candidate']['table']['group']  = t('Election candidate');

  $data['election_candidate']['table']['entity type'] = 'election_candidate';

  $data['election_candidate']['table']['base'] = array(
    'field' => 'candidate_id',
    'title' => t('Election candidate'),
    'weight' => -8,
    'defaults' => array(
      'field' => 'fullname',
    ),
  );

  // Join candidates to elections.
  $data['election']['table']['join']['election_candidate'] = array(
    'left_field' => 'election_id',
    'field' => 'election_id',
  );

  // Join candidates to posts (positions).
  $data['election_post']['table']['join']['election_candidate'] = array(
    'left_field' => 'post_id',
    'field' => 'post_id',
  );

  $data['election_candidate']['candidate_id'] = array(
    'title' => t('Candidate ID'),
    'help' => t('The unique ID of the election candidate.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

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

  $data['election_candidate']['published'] = array(
    'title' => t('Published'),
    'help' => t('Whether or not the candidate is published.'),
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

  $data['election_candidate']['fullname'] = array(
    'title' => t('Name'),
    'help' => t('The full name of the candidate ($Firstname $Lastname).'),
    'field' => array(
      'handler' => 'views_handler_field_election_candidate_fullname',
      'link_to_candidate default' => TRUE,
    ),
  );

  $data['election_candidate']['view_link'] = array(
    'field' => array(
      'title' => t('Profile link'),
      'help' => t("Provide a simple link to the candidate's profile page."),
      'handler' => 'views_handler_field_election_candidate_link',
    ),
  );

  return $data;

}
