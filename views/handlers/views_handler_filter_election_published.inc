<?php
/**
 * Filter by published status
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_election_published extends views_handler_filter {
  function admin_summary() { }
  function operator_form(&$form, &$form_state) { }
  function can_expose() { return FALSE; }

  function query() {
    $table = $this->ensure_my_table();
    $this->query->add_where_expression($this->options['group'], "$table.published = 1 OR ($table.uid = ***CURRENT_USER*** AND ***CURRENT_USER*** <> 0) OR ***EDIT_ANY_ELECTION*** = 1");
  }
}
