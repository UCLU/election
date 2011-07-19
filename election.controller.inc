<?php
class ElectionController extends DrupalDefaultEntityController {
  
  public function create($type = '') {
    
    return (object) array(
      'election_id' => '',
      'type' => $type,
      'title' => '',
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
        $op = 'insert'
      } else {
        drupal_write_record('election', $election, 'election_id');
        $op = 'update';
      }
      
    } catch (Exception $e) {
      
      $transaction->rollback();
      watchdog_exception('election', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
      
    }
    
  }
  
}