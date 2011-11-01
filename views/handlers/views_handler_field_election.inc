<?php
/**
 * @file
 * Contains the basic 'election' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a election.
 * Definition terms:
 * - link_to_election default: Should this field have the checkbox "link to election" enabled by default.
 */
class views_handler_field_election extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_election'])) {
      $this->additional_fields['election_id'] = array('table' => 'election', 'field' => 'election_id');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_election'] = array('default' => isset($this->definition['link_to_election default']) ? $this->definition['link_to_election default'] : FALSE);
    return $options;
  }

  /**
   * Provide link to election option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_election'] = array(
      '#title' => t('Link this field to the election'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_election']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the election.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_election'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = "election/" . $this->get_value($values, 'election_id');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
