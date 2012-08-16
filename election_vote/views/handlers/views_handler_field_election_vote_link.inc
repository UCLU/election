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
    $this->additional_fields['settings'] = 'settings';
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $post_id = $this->get_value($values, 'post_id');

    $small_post = new stdClass();
    $small_post->post_id = $post_id;
    $small_post->settings = $this->get_value($values, 'settings');

    // Ensure there are actually some candidates to vote for.
    if (!_election_vote_post_has_approved_candidates($small_post)) {
      return;
    }

    // Ensure the user has not already voted.
    if (election_vote_check_account_previously_voted($small_post)) {
      return;
    }

    // Ensure user has access to vote for this post.
    $post = election_post_load($post_id);
    if (!election_vote_access('vote', $post)) {
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
