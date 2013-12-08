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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election_candidate']['cstatus'] = array(
    'title' => t('Status'),
    'help' => t('The status of the candidate.'),
    'field' => array(
      'handler' => 'election_candidate_views_handler_field_cstatus',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'label' => t('Candidate status'),
      'handler' => 'election_candidate_views_handler_filter_cstatus',
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

  $data['election_candidate']['published_or_admin'] = array(
    'title' => t('Published or admin'),
    'help' => t('Filters out unpublished candidates if the current user may not view them.'),
    'filter' => array(
      'field' => 'published',
      'handler' => 'election_candidate_views_handler_filter_published',
    ),
  );

  $data['election_candidate']['uid'] = array(
    'title' => t('User UID'),
    'help' => t('The UID of the user associated with the candidate.'),
    'relationship' => array(
      'title' => t('User'),
      'help' => t('Relate candidates to users.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('user'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  $data['election_candidate']['fullname'] = array(
    'title' => t('Name'),
    'help' => t('The full name of the candidate ($Firstname $Lastname).'),
    'field' => array(
      'handler' => 'election_candidate_views_handler_field_full_name',
      'link_to_candidate default' => TRUE,
    ),
  );

  $data['election_candidate']['first_name'] = array(
    'title' => t('First name'),
    'help' => t('The first name(s) of the candidate.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election_candidate']['last_name'] = array(
    'title' => t('Last name'),
    'help' => t('The last name(s) of the candidate.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election_candidate']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date when the candidate was created (or nomination date).'),
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

  $data['election_candidate']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date when the candidate was last updated.'),
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

  $data['election_candidate']['view_link'] = array(
    'field' => array(
      'title' => t('Profile link'),
      'help' => t("Provide a simple link to the candidate's profile page."),
      'handler' => 'election_candidate_views_handler_field_link',
    ),
  );

  $data['election_candidate']['edit_link'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('A link to edit the candidate.'),
      'handler' => 'election_candidate_views_handler_field_link_edit',
    ),
  );

  $data['election_post']['nominate_link'] = array(
    'field' => array(
      'title' => t('Nomination form link'),
      'help' => t('Link to the nomination form.'),
      'handler' => 'election_candidate_views_handler_field_link_nominate',
    ),
  );

  return $data;

}
