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
   */
  public function title_query() {
    $titles = array();
    $query = db_select('election')
      ->fields('election', array('title'))
      ->condition('election_id', $this->value, 'IN');
    $result = $query->execute();
    foreach ($result as $item) {
      $titles[] = check_plain($item->title);
    }
    return $titles;
  }

}
