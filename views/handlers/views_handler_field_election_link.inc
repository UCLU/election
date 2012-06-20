<?php
/**
 * @file
 * Field handler to present a link to the election.
 */
class views_handler_field_election_link extends views_handler_field {
  public function construct() {
    parent::construct();
    $this->additional_fields['election_id'] = 'election_id';
  }

  public function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  public function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  public function render($values) {
    $value = $this->get_value($values, 'election_id');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  public function render_link($data, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/$data";
    $text = !empty($this->options['text']) ? $this->options['text'] : t('View');
    return $text;
  }
}
