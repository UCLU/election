<?php
/**
 * @file
 * Provide election ID argument handler.
 */

/**
 * Argument handler to accept an election ID.
 */
class views_handler_argument_election_electionid extends views_handler_argument_numeric {

  /**
   * Overrides views_handler_title_query().
   *
   * Get the title of the election.
   *
   * @return array
   */
  public function title_query() {
    $titles = array();
    $result = db_query("SELECT title FROM {election} WHERE election_id IN (:ids)", array(':ids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }

}

