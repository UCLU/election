<?php
/**
 * @file
 * Filter by voting status = open.
 */
class views_handler_filter_election_vstatus extends views_handler_filter_boolean_operator {

  function query() {
    $table = $this->ensure_my_table();
    $vars = array(
      ':openstatus' => ELECTION_STATUS_OPEN,
      ':closedstatus' => ELECTION_STATUS_CLOSED,
      ':scheduledstatus' => ELECTION_STATUS_SCHEDULED,
      ':rtime' => REQUEST_TIME,
    );
    if (empty($this->value)) {
      $expression = "$table.vstatus = :closedstatus OR ($table.vstatus = :scheduledstatus AND ($table.vopen_time >= :rtime OR $table.vclose_time <= :rtime))";
    }
    else {
      $expression = "$table.vstatus = :openstatus OR ($table.vstatus = :scheduledstatus AND $table.vopen_time < :rtime AND $table.vclose_time > :rtime)";
    }
    $this->query->add_where_expression(
      $this->options['group'],
      $expression,
      $vars
    );
  }

}
