<?php
/**
 * @file
 * Theme and preprocess functions for the Election module.
 */

/**
 * Theme function for displaying a piece of information which isn't a field.
 */
function theme_election_info_item($variables) {
  $label = $variables['label'];
  $value = $variables['value'];
  $html = $variables['html'];
  $classes = array('election-info');
  if (!empty($label)) {
    $classes[] = 'election-info-' . strtolower(preg_replace('/[^A-Z0-9]+/i', '-', $label));
  }
  $container = array(
    '#theme' => 'container',
    '#attributes' => array('class' => $classes),
  );
  if (!empty($variables['classes'])) {
    $container['#attributes']['class'] += (array) $variables['classes'];
  }
  $label = '<span class="label">' . check_plain($label) . ':</span>';
  $value = '<span class="value">' . ($html ? $value : check_plain($value)) . '</span>';
  $container['#children'] = $label . ' ' . $value;
  return drupal_render($container);
}

/**
 * Determine the election's status, for theme_election_status().
 */
function template_preprocess_election_status(&$variables) {

  $election = $variables['election'];
  $status_prefix = $variables['status_prefix'];

  $status = $election->{$status_prefix . 'status'};
  $open_time = $election->{$status_prefix . 'open_time'};
  $close_time = $election->{$status_prefix . 'close_time'};

  $variables['open'] = _election_is_open($status, $open_time, $close_time);
  $variables['scheduled'] = ($status == ELECTION_STATUS_SCHEDULED);

  $type = (empty($election->type_info)) ? _election_type_get_info($election->type) : $election->type_info;
  // Nominations aren't relevant to elections that don't have candidates.
  if ($status_prefix == 'n' && empty($type['has candidates'])) {
    $variables['status'] = t('N/A');
  }
  elseif ($variables['scheduled'] && REQUEST_TIME < $open_time) {
    $variables['status'] = t('Open from: %date', array('%date' => date('g.ia, j M Y', $open_time)));
  }
  elseif ($variables['scheduled'] && REQUEST_TIME < $close_time) {
    $variables['status'] = t('Scheduled (now open)');
  }
  elseif ($variables['open']) {
    $variables['status'] = t('Open');
  }
  else {
    $variables['status'] = t('Closed');
  }

}

/**
 * Display an election's voting or nominating status.
 *
 * Variables:
 *   $election      - An election object.
 *   $status_prefix - Type of status: 'v' = voting (default), 'n' = nominations.
 *   $status        - A string describing the status (sanitized).
 *   $open          - A Boolean indicating whether the status is open.
 *   $scheduled     - A Boolean indicating whether the status is scheduled.
 */
function theme_election_status($variables) {

  $classes = array(
    'election-status-' . ($variables['open'] ? 'open' : 'closed'),
  );

  if ($variables['scheduled']) {
    $classes[] = 'election-status-scheduled';
  }

  return theme('container', array(
    'element' => array(
      '#children' => $variables['status'],
      '#attributes' => array('class' => $classes),
    ),
  ));

}

/**
 * Process variables for election.tpl.php.
 *
 * The $variables['elements'] array contains the following arguments:
 * - #election
 * - #view_mode
 *
 * @see election.tpl.php
 */
function template_preprocess_election(&$variables) {

  $variables['view_mode'] = $variables['elements']['#view_mode'];
  // Provide a distinct $teaser boolean.
  $variables['teaser'] = $variables['view_mode'] == 'teaser';
  $variables['election'] = $variables['elements']['#election'];
  $election = $variables['election'];

  $uri = election_uri($election);
  $variables['election_url'] = url($uri['path'], $uri['options']);
  $variables['title']     = check_plain($election->title);
  $variables['page']      = $variables['view_mode'] == 'full';

  $variables['vstatus'] = theme('election_status', array('election' => $election));
  $variables['nstatus'] = theme('election_status', array('election' => $election, 'status_prefix' => 'n'));

  // Flatten the election object's member fields.
  $variables = array_merge((array) $election, $variables);

  // Helpful $content variable for templates.
  $variables += array('content' => array());
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('election', $election, $variables['content'], $variables);

  $variables['classes_array'][] = drupal_html_class('election-type-' . $election->type);

  $variables['theme_hook_suggestions'][] = 'election__' . $election->type;
  $variables['theme_hook_suggestions'][] = 'election__' . $election->election_id;

}
