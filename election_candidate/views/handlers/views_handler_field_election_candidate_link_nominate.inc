<?php
/**
 * @file
 * Field handler to present a link to an election post's nomination form.
 *
 * @todo fix this for performance
 */

class views_handler_field_election_candidate_link_nominate extends views_handler_field {

  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['election_id'] = 'election_id';
    $this->additional_fields['post_id'] = 'post_id';
    $this->additional_fields['nstatus_inheritance'] = array('table' => 'election_post', 'field' => 'nstatus_inheritance');
    $this->additional_fields['nstatus'] = array('table' => 'election', 'field' => 'nstatus');
    $this->additional_fields['nopen_time'] = array('table' => 'election', 'field' => 'nopen_time');
    $this->additional_fields['nclose_time'] = array('table' => 'election', 'field' => 'nclose_time');
    $this->additional_fields['election_type'] = array('table' => 'election', 'field' => 'type');
  }

  /**
   * Overrides parent::query().
   */
  public function query() {
    $this->ensure_my_table();
    $this->query->add_table('election_post');
    $this->query->add_table('election');
    $this->add_additional_fields();
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    // Build a dummy post object (to avoid loading it in full).
    $election_type = $this->get_value($values, 'election_type');
    $post = (object) array(
      'post_id' => $this->get_value($values, 'post_id'),
      'nstatus_inheritance' => $this->get_value($values, 'nstatus_inheritance'),
      'election' => (object) array(
        'election_id' => $this->get_value($values, 'election_id'),
        'nstatus' => $this->get_value($values, 'nstatus'),
        'nopen_time' => $this->get_value($values, 'nopen_time'),
        'nclose_time' => $this->get_value($values, 'nclose_time'),
        'type' => $election_type,
        'type_info' => _election_type_get_info($election_type),
      ),
    );
    return theme('election_candidate_nominate_link', array('post' => $post));
  }

}
