<?php
/**
 * @file
 * Field handler to translate an election type into its readable form.
 *
 * @see views_handler_field_node_type.inc
 */


class views_handler_field_election_type extends views_handler_field {

  /**
   * Overrides parent::option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['machine_name'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide machine_name option for election type display.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['machine_name'] = array(
      '#title' => t('Output machine name'),
      '#description' => t('Display field as the election type machine name.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['machine_name']),
      '#fieldset' => 'more',
    );
  }

  /**
   * Render the election type as a human readable name.
   *
   * The 'machine_name' option disables this.
   */
  public function render_name($data, $values) {
    if ($this->options['machine_name'] != 1 && $data !== NULL && $data !== '') {
      $type = _election_type_get_info($data);
      $name = drupal_ucfirst($type['name']);
      return t($this->sanitize_value($name));
    }
    return $this->sanitize_value($data);
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_name($value, $values);
  }

}
