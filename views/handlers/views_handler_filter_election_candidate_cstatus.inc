<?php
/**
 * @file
 * Filter by candidate status (=approved)
 */
class views_handler_filter_election_candidate_cstatus extends views_handler_filter_boolean_operator {

  function query() {
    $table = $this->ensure_my_table();
    $vars = array(
      ':approvedstatus' => ELECTION_CANDIDATE_APPROVED,
    );
    if (empty($this->value)) {
      $expression = "$table.cstatus <> :approvedstatus";
    }
    else {
      $expression = "$table.cstatus = :approvedstatus";
    }
    $this->query->add_where_expression(
      $this->options['group'],
      $expression,
      $vars
    );
  }

}
