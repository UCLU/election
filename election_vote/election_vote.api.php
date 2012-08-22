<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Modify voting access check.
 *
 * This allows other modules to deny access to voting, or to deleting one's own
 * votes. The election_condition submodule provides an example.
 *
 * @param string $op
 *   The operation requested (either 'vote' or 'delete').
 * @param stdClass $post
 *   An election post object.
 * @param stdClass $account
 *   A Drupal user account object.
 *
 * @return
 *   Return FALSE to deny access; other return values have no effect.
 */
function hook_election_vote_access($op, $post, $account) {
  // Deny vote access to the user with the UID 37491.
  if ($op == 'vote' && $account->uid == 37491) {
    return FALSE;
  }
}
