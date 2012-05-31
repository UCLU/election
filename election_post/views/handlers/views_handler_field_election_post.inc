<?php
/**
 * @file
 * Contains the basic 'election post' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a election post.
 * Definition terms:
 * - link_to_post default: Should this field have the checkbox "link to post" enabled by default.
 */
class views_handler_field_election_post extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_post'])) {
      $this->additional_fields['election_id'] = array('table' => 'election_post', 'field' => 'election_id');
      $this->additional_fields['post_id'] = array('table' => 'election_post', 'field' => 'post_id');
      $this->additional_fields['type'] = array('table' => 'election_post', 'field' => 'type');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_post'] = array('default' => isset($this->definition['link_to_post default']) ? $this->definition['link_to_post default'] : FALSE);
    return $options;
  }

  /**
   * Provide link to election option
   */
  function options_form(&$form, &$form_state) {
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
  function render_link($data, $values) {
    $post = (object) array(
      'election_id' => $this->get_value($values, 'election_id'),
      'post_id' => $this->get_value($values, 'post_id'),
      'type' => $this->get_value($values, 'type'),
    );
    if (!empty($this->options['link_to_post'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = election_post_uri_path($post);
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
