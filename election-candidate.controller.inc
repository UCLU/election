<?php
/**
 * @file
 * Entity controller for candidates (in the Election module).
 *
 * @class
 * ElectionCandidateController
 * @extends
 * DrupalDefaultEntityController
 */
class ElectionCandidateController extends DrupalDefaultEntityController {

  public function create($post) {

    return (object) array(
      'candidate_id' => '',
      'post_id' => $post->post_id,
      'uid' => '',
      'phone' => '',
      'summary' => '',
      'manifesto' => '',
      'photo' => '',
      'proposer' => array(),
      'seconders' => array(),
    );

  }

  public function save($candidate) {

    $transaction = db_transaction();

    try {

      global $user;

      // Determine whether the candidate is new.
      $candidate->is_new = empty($candidate->candidate_id);

      // Set timestamp fields.
      if (empty($candidate->created)) {
        $candidate->created = REQUEST_TIME;
      }

      $candidate->changed = REQUEST_TIME;

      field_attach_presave('election_candidate', $candidate);

      if ($candidate->is_new) {
        drupal_write_record('election_candidate', $candidate);
        $op = 'insert';
      }
      else {
        drupal_write_record('election_candidate', $candidate, 'candidate_id');
        $op = 'update';
      }

      $function = 'field_attach_' . $op;
      $function('election_candidate', $candidate);

      module_invoke_all('entity_' . $op, $candidate, 'election_candidate');

      unset($candidate->is_new);

      // Skip the slave server for this user temporarily.
      db_ignore_slave();

      return $candidate;

    }
    catch (Exception $e) {

      $transaction->rollback();
      watchdog_exception('election_candidate', $e, NULL, WATCHDOG_ERROR);
      return FALSE;

    }

  }

  public function delete($ids) {

    if (!empty($ids)) {
      $candidates = $this->load($ids, array());
      $transaction = db_transaction();

      try {
        db_delete('election_candidate')
          ->condition('candidate_id', $ids, 'IN')
          ->execute();
        foreach ($candidates as $candidate_id => $candidate) {
          field_attach_delete('election_candidate', $candidate);
        }
        db_ignore_slave();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('election', $e, NULL, WATCHDOG_ERROR);
        return FALSE;
      }

      module_invoke_all('entity_delete', $candidates, 'election_candidate');

      cache_clear_all();
      $this->resetCache();

    }

    return TRUE;

  }

}
