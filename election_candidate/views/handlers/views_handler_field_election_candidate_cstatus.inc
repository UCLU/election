<?php
/**
 * @file
 * Handler for the election candidate status (cstatus) field.
 */

class views_handler_field_election_candidate_cstatus extends views_handler_field {

  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['cstatus'] = 'cstatus';
  }

  /**
   * Overrides parent::query().
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    // Create dummy election object
    $candidate = new stdClass();
    foreach ($values as $key => $value) {
      $candidate->{str_replace('election_candidate_', '', $key)} = $value;
    }
    return _election_candidate_format_status($candidate);
  }

}
