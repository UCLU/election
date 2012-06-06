<?php
/**
 * @file
 * Field handler to present a link to a post's voting form.
 */
class views_handler_field_election_post_link_vote extends views_handler_field_election_post_link {

  function construct() {
    parent::construct();
  }

  function render_link($data, $values) {
    // ensure user has access to vote for this post
    $post = election_post_load($this->get_value($values, 'post_id'));
    if (!entity_access('vote', 'election_post', $post)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = election_post_uri_path($post) . '/vote';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('vote');
    return $text;
  }

}
