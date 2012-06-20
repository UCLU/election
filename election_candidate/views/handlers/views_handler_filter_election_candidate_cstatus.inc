<?php
/**
 * @file
 * Filter by candidate status (=approved)
 */
class views_handler_filter_election_candidate_cstatus extends views_handler_filter_in_operator {

  public function get_value_options() {
    $this->value_options = array(
      ELECTION_CANDIDATE_APPROVED => t('Approved'),
      ELECTION_CANDIDATE_PENDING => t('Pending'),
      ELECTION_CANDIDATE_WITHDRAWN => t('Withdrawn'),
      ELECTION_CANDIDATE_REJECTED => t('Rejected'),
    );
    return $this->value_options;
  }

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
