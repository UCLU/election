<?php
/**
 * @file
 * Election hooks for the Election Condition module.
 */

/**
 * Implements hook_election_vote_before_grant().
 */
function election_condition_election_vote_before_grant($post, $account) {

  // Get the conditions associated with this post.
  $conditions = election_condition_get_conditions($post);
  if (!count($conditions)) {
    return;
  }

  // Make sure that all the callbacks for all the conditions exist and return
  // TRUE. Otherwise, return FALSE to deny voting access.
  foreach ($conditions as $condition) {
    foreach ($condition->callbacks as $callback) {
      if (!function_exists($callback) || $callback($post, $account) !== TRUE) {
        return FALSE;
      }
    }
  }

}
