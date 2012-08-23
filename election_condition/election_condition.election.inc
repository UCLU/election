<?php
/**
 * @file
 * Election hooks for the Election Condition module.
 */

/**
 * Implements hook_election_vote_access().
 *
 * @return mixed
 *   FALSE on permission denied, NULL otherwise.
 */
function election_condition_election_vote_access($op, $post = NULL, $account = NULL) {

  if (empty($account)) {
    $account = $GLOBALS['user'];
  }

  switch ($op) {

    case 'vote':
      // Get the conditions associated with this post.
      $conditions = election_condition_get_conditions($post);
      if (!count($conditions)) {
        return NULL;
      }

      // If any condition does not exist or return TRUE, return FALSE.
      foreach ($conditions as $condition) {
        foreach ($condition->callbacks as $callback) {
          if (!function_exists($callback) || $callback($account) !== TRUE) {
            return FALSE;
          }
        }
      }
      break;

  }

  return FALSE;

}
