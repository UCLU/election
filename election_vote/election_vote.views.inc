<?php
/**
 * @file
 * Views integration for the Election Vote module.
 */

/**
 * Implements hook_views_data().
 */
function election_vote_views_data() {

  $data = array();

  $data['election_post']['vote_link'] = array(
    'field' => array(
      'title' => t('Vote link'),
      'help' => t('Provide a link to cast a vote for this post.'),
      'handler' => 'views_handler_field_election_vote_link',
    ),
  );

  $data['election_post']['eligibility'] = array(
    'title' => t('Eligibility'),
    'help' => t('Whether the current user is eligible to vote.'),
    'real field' => 'post_id',
    'field' => array(
      'handler' => 'views_handler_field_election_vote_eligibility',
      'click sortable' => FALSE,
    ),
  );

  return $data;

}
