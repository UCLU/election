<?php
/**
 * @file
 * Filter by nominations status
 */
class views_handler_filter_election_nstatus extends views_handler_filter_boolean_operator {

  function query() {
    $table = $this->ensure_my_table();
    $vars = array(
      ':openstatus' => ELECTION_STATUS_OPEN,
      ':closedstatus' => ELECTION_STATUS_CLOSED,
      ':scheduledstatus' => ELECTION_STATUS_SCHEDULED,
      ':rtime' => REQUEST_TIME,
    );
    if (empty($this->value)) {
      $expression = "$table.nstatus = :closedstatus OR ($table.nstatus = :scheduledstatus AND ($table.nopen_time >= :rtime OR $table.nclose_time <= :rtime)) OR $table.type = 'referendum'";
    }
    else {
      $expression = "$table.nstatus = :openstatus OR ($table.nstatus = :scheduledstatus AND $table.nopen_time < :rtime AND $table.nclose_time > :rtime) OR $table.type = 'referendum'";
    }
    $this->query->add_where_expression(
      $this->options['group'],
      $expression,
      $vars
    );
  }

}
