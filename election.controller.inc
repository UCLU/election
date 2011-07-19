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
        $op = 'insert';
      } else {
        drupal_write_record('election', $election, 'election_id');
        $op = 'update';
      }
      
      $function = 'field_attach_' . $op;
      $function('election', $election);
      
      module_invoke_all('entity_' . $op, $election, 'election');
      
      unset($election->is_new);
      
      // Skip the slave server for this user temporarily.
      db_ignore_slave();
      
      return $election;
      
    } catch (Exception $e) {
      
      $transaction->rollback();
      watchdog_exception('election', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
      
    }
    
  }
  
}