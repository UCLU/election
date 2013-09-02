<?php
/**
 * @file
 * Election hooks for the Election Condition module.
 */

/**
 * Implements hook_election_vote_before_grant().
 */
function election_condition_election_vote_before_grant($post, $account) {

  // Call all the callbacks for all the conditions. All the callbacks must exist
  // and return TRUE; otherwise, voting access will be denied.
  foreach ($post->conditions as $condition) {
    foreach ($condition['callbacks'] as $callback) {
      if (!function_exists($callback) || $callback($post, $account) !== TRUE) {
        return FALSE;
      }
    }
  }

}
