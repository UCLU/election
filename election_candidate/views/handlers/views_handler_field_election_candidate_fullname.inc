<?php
/**
 * @file
 * Handler for the election candidate status (fullname) field.
 */

class views_handler_field_election_candidate_fullname extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['first_name'] = 'first_name';
    $this->additional_fields['last_name'] = 'last_name';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $first_name = $this->get_value($values, 'first_name');
    $last_name = $this->get_value($values, 'last_name');
    return check_plain($first_name . ' ' . $last_name);
  }

}
