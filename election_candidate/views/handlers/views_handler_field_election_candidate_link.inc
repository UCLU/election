<?php
/**
 * @file
 * Field handler to present a link to the candidate's profile page.
 */
class views_handler_field_election_candidate_link extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['candidate_id'] = 'candidate_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $value = $this->get_value($values, 'candidate_id');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function render_link($data, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = election_candidate_uri_path((object) array('candidate_id' => $data));
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view profile');
    return $text;
  }
}
