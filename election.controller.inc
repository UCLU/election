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
        // @TODO
      }
      
    } catch (Exception $e) {
      
      $transaction->rollback();
      watchdog_exception('election', $e, NULL, WATCHDOG_ERROR);
      return FALSE;
      
    }
    
  }
  
}