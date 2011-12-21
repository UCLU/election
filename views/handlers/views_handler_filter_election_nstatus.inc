<?php
/**
 * @file
 * Filter by nominations status
 */
class views_handler_filter_election_nstatus extends views_handler_filter {

  function admin_summary() { }
  function operator_form(&$form, &$form_state) { }
  function can_expose() { return FALSE; }

  function query() {
    $table = $this->ensure_my_table();
    $this->query->add_where_expression(
      $this->options['group'],
      "$table.nstatus = :openstatus OR ($table.nstatus = :scheduledstatus AND $table.nopen_time < :rtime AND $table.nclose_time > :rtime)",
      array(
        ':openstatus' => ELECTION_STATUS_OPEN,
        ':scheduledstatus' => ELECTION_STATUS_SCHEDULED,
        ':rtime' => REQUEST_TIME,
      )
    );
  }

}
