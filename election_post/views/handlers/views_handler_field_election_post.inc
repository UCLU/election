<?php
/**
 * @file
 * Contains the basic 'election post' field handler.
 */

/**
 * Field handler simply to allow linking to a election post.
 */
class views_handler_field_election_post extends views_handler_field {

  /**
   * Overrides parent::init().
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_post'])) {
      $this->additional_fields['post_id'] = array('table' => 'election_post', 'field' => 'post_id');
    }
  }

  /**
   * Overrides parent::option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_post'] = array('default' => isset($this->definition['link_to_post default']) ? $this->definition['link_to_post default'] : FALSE);
    return $options;
  }

  /**
   * Provide 'link to post' option.
   */
  public function options_form(&$form, &$form_state) {
    $form['link_to_post'] = array(
      '#title' => t('Link this field to the election post'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_post']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the election post.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  public function render_link($data, $values) {
    if (empty($this->options['link_to_post'])) {
      return $data;
    }
    if ($data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = election_post_uri_path((object) array(
        'post_id' => $this->get_value($values, 'post_id'),
      ));
    }
    else {
      $this->options['alter']['make_link'] = FALSE;
    }
    return $data;
  }

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
