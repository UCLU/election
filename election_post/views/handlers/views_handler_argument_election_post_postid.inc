<?php
/**
 * @file
 * Provide election post ID argument handler.
 */

/**
 * Argument handler to accept a post ID.
 */
class views_handler_argument_election_post_postid extends views_handler_argument_numeric {

  /**
   * Overrides views_handler_title_query().
   *
   * Get the title of the post.
   *
   * @return array
   */
  function title_query() {
    $titles = array();
    $result = db_query("SELECT title FROM {election_post} WHERE post_id IN (:ids)", array(':ids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }

}

