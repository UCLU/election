<?php
/**
 * @file
 * Filter by candidate status.
 */

class views_handler_filter_election_candidate_cstatus extends views_handler_filter_in_operator {

  /**
   * Overrides parent::get_value_options().
   */
  public function get_value_options() {
    $this->value_options = _election_candidate_get_cstatus_options();
    return $this->value_options;
  }

  /**
   * Overrides parent::query().
   */
  public function query() {
    $table = $this->ensure_my_table();
    if (empty($this->value)) {
      return;
    }
    $group = $this->query->set_where_group('OR');
    foreach ($this->value as $value) {
      $this->query->add_where_expression($group, "$table.cstatus = :value", array(':value' => $value));
    }
  }

}
