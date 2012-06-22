<?php
/**
 * @file
 * Field handler to present a link to an election_post's voting form.
 */

class views_handler_field_election_vote_link extends views_handler_field_election_post_link {

  /**
   * Overrides parent::init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['use_ron'] = 'use_ron';
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $post_id = $this->get_value($values, 'post_id');

    $small_post = new stdClass();
    $small_post->post_id = $post_id;
    $small_post->use_ron = $this->get_value($values, 'use_ron');

    // ensure there are actually some candidates to vote for
    if (!_election_vote_post_has_approved_candidates($small_post)) {
      return;
    }

    // ensure the user has not already voted
    if (election_vote_check_user_previously_voted($small_post)) {
      return;
    }

    // ensure user has access to vote for this post
    $post = election_post_load($post_id);
    if (!election_post_access('vote', $post)) {
      return;
    }

    return $this->render_link($post, $values);
  }

  /**
   * Overrides parent::render_link().
   */
  public function render_link($post, $values) {

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = election_post_uri_path($post) . '/vote';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Vote');
    return $text;

  }

}
