<?php
/**
 * @file
 * Field handler to present a link to a post's voting form.
 */
class views_handler_field_election_post_link_vote extends views_handler_field_election_post_link {

  function render_link($data, $values) {

    $small_post = new stdClass();
    $small_post->post_id = $this->get_value($values, 'post_id');

    // ensure there are actually some candidates to vote for
    if (!_election_post_has_approved_candidates($small_post)) {
      return;
    }

    // ensure the user has not already voted
    if (election_vote_check_user_previously_voted($small_post)) {
      return;
    }

    // ensure user has access to vote for this post
    $post = election_post_load($small_post->post_id);
    if (!entity_access('vote', 'election_post', $post)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = election_post_uri_path($post) . '/vote';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Vote');
    return $text;

  }

}
