<?php
/**
 * @file
 * Handler for the election candidate "Full name" field.
 */

class views_handler_field_election_candidate_fullname extends views_handler_field {

  /**
   * Overrides parent::construct().
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['first_name'] = 'first_name';
    $this->additional_fields['last_name'] = 'last_name';
    $this->additional_fields['type'] = 'type';
  }

  /**
   * Overrides parent::init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_candidate'])) {
      $this->additional_fields['candidate_id'] = array('table' => 'election_candidate', 'field' => 'candidate_id');
    }
  }

  /**
   * Overrides parent::option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_candidate'] = array('default' => isset($this->definition['link_to_candidate default']) ? $this->definition['link_to_candidate default'] : FALSE);
    return $options;
  }

  /**
   * Provide 'link to candidate' option.
   */
  public function options_form(&$form, &$form_state) {
    $form['link_to_candidate'] = array(
      '#title' => t("Link this field to the candidate's profile."),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_candidate']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the candidate.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  public function render_link($data, $values) {
    if (!empty($this->options['link_to_candidate'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = "election-candidate/" . $this->get_value($values, 'candidate_id');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  /**
   * Overrides parent::query().
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $candidate = new stdClass();
    $candidate->first_name = $this->get_value($values, 'first_name');
    $candidate->last_name = $this->get_value($values, 'last_name');
    $candidate->type = $this->get_value($values, 'type');
    $data = theme('election_candidate_full_name', array('candidate' => $candidate));
    return $this->render_link($data, $values);
  }

}
