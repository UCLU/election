<?php
/**
 * @file
 * Field handler to present a link to manage an election's posts.
 *
 * This works with the election_post module.
 */

class views_handler_field_election_link_posts extends views_handler_field_election_link {

  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['uid'] = 'uid';
  }

  /**
   * Overrides parent::render_link().
   */
  public function render_link($data, $values) {

    $election = new stdClass();
    $election->election_id = $this->get_value($values, 'election_id');
    $election->type = $this->get_value($values, 'type');
    $election->uid = $this->get_value($values, 'uid');

    // Exit if the election_post module is not enabled.
    if (!module_exists('election_post')) {
      return;
    }

    // Ensure user has access to edit this election.
    if (!entity_access('edit', 'election', $election)) {
      return;
    }

    $posts_name = election_get_post_name($election->type, TRUE);
    $posts_count = _election_post_count_by_election($election);

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/posts";

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Manage @posts (!count)', array('@posts' => $posts_name, '!count' => $posts_count));
    return $text;
  }
}
