<?php
/**
 * @file
 * Contains a field handler for displaying the user's eligibility to vote for a specified election_post.
 */

class views_handler_field_election_vote_eligibility extends views_handler_field {

  /**
   * Overrides parent::init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->field = 'vstatus_inheritance';
    $this->real_field = 'vstatus_inheritance';
    $this->additional_fields['*'] = 'post_id';
    $this->additional_fields['vstatus'] = array('table' => 'election', 'field' => 'vstatus');
    $this->additional_fields['vopen_time'] = array('table' => 'election', 'field' => 'vopen_time');
    $this->additional_fields['vclose_time'] = array('table' => 'election', 'field' => 'vclose_time');
    $this->additional_fields['election_type'] = array('table' => 'election', 'field' => 'type');
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $post = election_post_load($this->get_value($values, 'post_id'));
    $election = (object) array(
      'type' => $this->get_value($values, 'election_type'),
      'vstatus' => $this->get_value($values, 'vstatus'),
      'vopen_time' => $this->get_value($values, 'vopen_time'),
      'vclose_time' => $this->get_value($values, 'vclose_time'),
    );
    return _election_vote_format_eligibility($post, $election);
  }

}
