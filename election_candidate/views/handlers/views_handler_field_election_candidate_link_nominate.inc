<?php
/**
 * @file
 * Field handler to present a link to an election post's nomination form.
 */

class views_handler_field_election_candidate_link_nominate extends views_handler_field_election_post_link {

  /**
   * Overrides parent::render_link().
   */
  public function render_link($data, $values) {
    $post = election_post_load($this->get_value($values, 'post_id'));
    // Only show a link if the user has access to nominate for this post.
    if (!election_candidate_nominate_access($post)) {
      return;
    }
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election-post/{$post->post_id}/nomination-form";
    $this->options['alter']['alt'] = t('Nomination form for @post', array('@post' => $post->title));

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Nominate');

    return $text;
  }

}
