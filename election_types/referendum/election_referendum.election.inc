<?php
/**
 * @file
 * Election hooks for the Election Referendum module.
 */

/**
 * Implements hook_election_type_info().
 */
function election_referendum_election_type_info() {
  return array(
    'referendum' => array(
      'name' => t('referendum'),
      'description' => t('A simple referendum where people can vote in response to one or more motions.'),
      'post name' => t('motion'),
      'vote form' => 'election_referendum_vote_form',
      'save votes' => 'election_referendum_save_votes',
      'results page' => 'election_referendum_count_page',
    ),
  );
}
