<?php
/**
 * @file
 * Views integration for the Election Post module.
 */

/**
 * Implements hook_views_data().
 */
function election_post_views_data() {

  $data = array();

  $data['election_post']['table']['group']  = t('Election post');

  $data['election_post']['table']['entity type'] = 'election_post';

  $data['election_post']['table']['base'] = array(
    'field' => 'post_id',
    'title' => t('Election post'),
    'weight' => -9,
    'defaults' => array(
      'field' => 'title',
    ),
  );

  // Join posts to elections.
  $data['election']['table']['join']['election_post'] = array(
    'left_field' => 'election_id',
    'field' => 'election_id',
  );

  $data['election_post']['post_id'] = array(
    'title' => t('Post ID'),
    'help' => t('The unique ID of the election post.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_election_post_postid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'post_id',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['election_post']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the election post.'),
    'field' => array(
      'handler' => 'views_handler_field_election_post',
      'click sortable' => TRUE,
      'link_to_post default' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['election_post']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date when the post was created.'),
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

  $data['election_post']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date when the post was last updated.'),
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

  $data['election_post']['view_link'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the post.'),
      'handler' => 'views_handler_field_election_post_link',
    ),
  );

  $data['election_post']['edit_link'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('A link to edit the post.'),
      'handler' => 'views_handler_field_election_post_link_edit',
    ),
  );

  return $data;

}
