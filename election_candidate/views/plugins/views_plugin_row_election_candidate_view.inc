<?php
/**
 * @file
 * Row style plugin for running election_candidate_view() on a candidate in a View.
 */

/**
 * @ingroup views_row_plugins
 */
class views_plugin_row_election_candidate_view extends views_plugin_row {

  // Basic properties that let the row style follow relationships.
  // from Views module
  var $base_table = 'election_candidate';
  var $base_field = 'candidate_id';

  // Stores the entities loaded with pre_render.
  var $entities = array();

  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);
    require_once drupal_get_path('module', 'election_candidate') . '/election-candidate.pages.inc';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['view_mode'] = array('default' => 'teaser');
    $options['links'] = array('default' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options_form_summary_options();
    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $this->options['view_mode'],
     );
    $form['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display links'),
      '#default_value' => $this->options['links'],
    );
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  function options_form_summary_options() {
    $entity_info = entity_get_info('election_candidate');
    $options = array();
    if (!empty($entity_info['view modes'])) {
      foreach ($entity_info['view modes'] as $mode => $settings) {
        $options[$mode] = $settings['label'];
      }
    }
    if (empty($options)) {
      $options = array(
        'teaser' => t('Teaser'),
        'full' => t('Full content')
      );
    }
    return $options;
  }

  function summary_title() {
    $options = $this->options_form_summary_options();
    return check_plain($options[$this->options['view_mode']]);
  }

  function pre_render($values) {
    $candidate_ids = array();
    foreach ($values as $row) {
      $candidate_ids[] = $row->{$this->field_alias};
    }
    $this->entities = election_candidate_load_multiple($candidate_ids);
  }

  function render($row) {
    $candidate = $this->entities[$row->{$this->field_alias}];
    $candidate->view = $this->view;

    $build = election_candidate_view($candidate, NULL, $this->options['view_mode']);

    return drupal_render($build);
  }
}
