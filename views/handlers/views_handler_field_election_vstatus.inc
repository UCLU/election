<?php
/**
 * @file
 * Handler for the election voting status (vstatus) field.
 */

class views_handler_field_election_vstatus extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['vstatus'] = 'vstatus';
    $this->additional_fields['vopen_time'] = 'vopen_time';
    $this->additional_fields['vclose_time'] = 'vclose_time';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Dummy election object
    $election = (object) $values;
    return _election_format_status($election, 'v');
  }

}
