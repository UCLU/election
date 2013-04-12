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
  $conditions = election_condition_get_conditions_by_post($post);

  // Call all the callbacks for all the conditions. All the callbacks must exist
  // and return TRUE; otherwise, voting access will be denied.
  foreach ($conditions as $condition) {
    foreach ($condition['callbacks'] as $callback) {
      if (!function_exists($callback) || $callback($post, $account) !== TRUE) {
        return FALSE;
      }
    }
  }

}
