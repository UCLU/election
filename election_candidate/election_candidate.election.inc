<?php
/**
 * @file
 * Election hooks for the Election Candidate module.
 */

/**
 * Implements hook_election_vote_before_grant().
 */
function election_candidate_election_vote_before_grant($post, $account) {
  // Prevent users from voting when there aren't enough candidates.
  if (!election_candidate_post_has_enough($post)) {
    return FALSE;
  }
}
