<?php
/**
 * @file
 * Provide election type argument handler.
 */

/**
 * Argument handler to accept an election type.
 */
class election_views_handler_argument_type extends views_handler_argument_numeric {

  /**
   * Overrides views_handler_title_query().
   *
   * Get the name of the election type.
   */
  public function title_query() {
    $names = array();
    $election_types = election_types();
    foreach ($this->value as $machine_name) {
      if (isset($election_types[$machine_name])) {
        $names[] = $election_types[$machine_name]['name'];
      }
    }
    return $names;
  }

}
