<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Act before voting access is granted for a user.
 *
 * @see election_condition.election.inc
 *
 * @param stdClass $post
 *   An election post object.
 * @param stdClass $account
 *   A Drupal user account object.
 *
 * @return mixed
 *   Return FALSE to deny voting access. Other return values have no effect.
 */
function hook_election_vote_before_grant($post, $account) {
  // Deny access to the user with the UID 37491.
  if ($account->uid == 37491) {
    return FALSE;
  }
}

/**
 * Save votes on submission of the voting form, for this election type.
 *
 * This hook runs inside the same database transaction that saves 'ballots' to
 * the {election_ballot} table. To roll back the transaction, return FALSE or
 * throw an exception.
 *
 * @param int $ballot_id
 *   The {ballot}.ballot_id of the ballot being saved.
 * @param stdClass $post
 *   The election post object.
 * @param array $vote_form
 *   The Form API array for the voting form.
 * @param array $vote_form_state
 *   The Form API 'form state' array for the voting form.
 *
 * @return bool
 *   Return TRUE on success, or FALSE on failure. Votes and ballots are not
 *   saved unless this implementation returns TRUE.
 */
function hook_election_vote_ELECTION_TYPE_save($ballot_id, $post, $vote_form, $vote_form_state) {
  // Get the vote value out of the form.
  $answer = $vote_form_state['values']['answer'];

  // Save a vote corresponding to this ballot ID.
  db_insert('election_vote')
    ->fields(array(
      'election_id' => $post->election_id,
      'ballot_id' => $ballot_id,
      'post_id' => $post->post_id,
      'answer' => $answer,
    ))
    ->execute();

  return TRUE;
}
