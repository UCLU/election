<?php
/**
 * @file
 * Views handler to filter election candidates by 'published' status.
 */

class views_handler_filter_election_candidate_published extends views_handler_filter {

  public function admin_summary() {
    return;
  }

  public function operator_form(&$form, &$form_state) {
    return;
  }

  /**
   * Overrides parent::can_expose().
   *
   * It makes no sense to expose this filter.
   */
  public function can_expose() {
    return FALSE;
  }

  public function query() {
    $join = new views_join();
    $join->construct('election', 'election_candidate', 'election_id', 'election_id');
    $candidate_table = $this->ensure_my_table();
    $election_table = $this->query->add_relationship('election', $join, $this->table_alias, $this->relationship);
    $this->query->add_where_expression($this->options['group'], "$candidate_table.published = 1 OR ($election_table.uid = ***CURRENT_USER*** AND ***CURRENT_USER*** <> 0) OR ***EDIT_ANY_ELECTION*** = 1");
  }
}
