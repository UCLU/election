<?php
/**
 * @file
 * Filter by eligibility.
 *
 * @todo
 */
class views_handler_filter_election_post_eligibility extends views_handler_filter_in_operator {

  function get_value_options() {
    $this->value_options = array(
      1 => t('Eligible'),
      2 => t('Already voted')
      3 => t('Eligible, but voting is currently unavailable.'),
      4 => t('Ineligible'),
    );
    return $this->value_options;
  }

  function query() {
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
