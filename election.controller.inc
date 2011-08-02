<?php
/**
 * @file
 * Entity controller for the Election module.
 *
 * @class
 * ElectionController
 * @extends
 * DrupalDefaultEntityController
 */
class ElectionController extends DrupalDefaultEntityController {

  public function create($type = 0) {

    return (object) array(
      'election_id' => '',
      'type' => $type,
      'title' => '',
      'status' => NULL,
      'open_time' => NULL,
      'close_time' => NULL,
    );

  }

  public function save($election) {

    $transaction = db_transaction();

    try {

      global $user;

      // Determine whether the election is new.
      $election->is_new = empty($election->election_id);

      // Set timestamp fields.
      if (empty($election->created)) {
        $election->created = REQUEST_TIME;
      }

      $election->changed = REQUEST_TIME;
      $update_election = TRUE;

      field_attach_presave('election', $election);

      if ($election->is_new) {
        drupal_write_record('election', $election);
        $op = 'insert';
      }
      else {
        drupal_write_record('election', $election, 'election_id');
        $op = 'update';
      }

      $function = 'field_attach_' . $op;
      $function('election', $election);

      module_invoke_all('entity_' . $op, $election, 'election');

      if (function_exists('pathauto_election_update_alias')) {
        pathauto_election_update_alias($election, $op);
      }

      unset($election->is_new);

      // Skip the slave server for this user temporarily.
      db_ignore_slave();

      return $election;

    }
    catch (Exception $e) {

      $transaction->rollback();
      watchdog_exception('election', $e, NULL, WATCHDOG_ERROR);
      return FALSE;

    }

  }

  public function delete($ids) {

    if (!empty($ids)) {
      $elections = $this->load($ids, array());
      $transaction = db_transaction();

      try {
        db_delete('election')
          ->condition('election_id', $ids, 'IN')
          ->execute();
        foreach ($elections as $election_id => $election) {

          field_attach_delete('election', $election);

          module_invoke_all('entity_delete', $election, 'election');

          if (function_exists('pathauto_entity_path_delete_all')) {
            pathauto_entity_path_delete_all('election', $election);
          }

        }
        db_ignore_slave();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('election', $e, NULL, WATCHDOG_ERROR);
        return FALSE;
      }

// @todo this doesn't make sense
//      module_invoke_all('entity_delete', $election, 'election');

      cache_clear_all();
      $this->resetCache();

    }

    return TRUE;

  }

}