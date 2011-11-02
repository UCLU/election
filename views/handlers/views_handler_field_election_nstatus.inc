<?php
/**
 * @file
 * Handler for the election nominations status (nstatus) field.
 */

class views_handler_field_election_nstatus extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['nstatus'] = 'nstatus';
    $this->additional_fields['nopen_time'] = 'nopen_time';
    $this->additional_fields['nclose_time'] = 'nclose_time';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Create dummy election object
    $election = new stdClass;
    foreach ($values as $key => $value) {
      $election->{str_replace('election_', '', $key)} = $value;
    }
    return _election_format_status($election, 'n');
  }

}
