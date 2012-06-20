<?php
/**
 * @file
 * Field handler to present a link to manage an election's posts. This depends on
 * the election_post module.
 */
class views_handler_field_election_link_posts extends views_handler_field_election_link {

  public function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['uid'] = 'uid';
  }

  public function render_link($data, $values) {

    $election = new stdClass();
    $election->election_id = $this->get_value($values, 'election_id');
    $election->type = $this->get_value($values, 'type');
    $election->uid = $this->get_value($values, 'uid');

    // exit if the election_post module is not enabled.
    if (!module_exists('election_post')) {
      return;
    }

    // ensure user has access to edit this election.
    if (!entity_access('edit', 'election', $election)) {
      return;
    }

    $posts_name = _election_get_posts_name($election->type, TRUE);

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/posts";

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Manage @posts_name', array('@posts_name' => $posts_name));
    return $text;
  }
}
