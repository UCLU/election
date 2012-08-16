<?php
/**
 * @file
 * Theme and preprocess functions for the Election module.
 */

/**
 * Display an election's voting or nominating status.
 *
 * Variables:
 *   $election      - An election object.
 *   $status_prefix - Type of status: 'v' = voting (default), 'n' = nominations.
 */
function theme_election_status($variables) {
  $election = $variables['election'];
  $status_prefix = $variables['status_prefix'];
  $type = (empty($election->type_info)) ? _election_type_get_info($election->type) : $election->type_info;
  // Nominations aren't relevant to elections that don't have candidates.
  if ($status_prefix == 'n' && empty($type['has candidates'])) {
    return t('N/A');
  }
  $status = $election->{$status_prefix . 'status'};
  $open_time = $election->{$status_prefix . 'open_time'};
  $close_time = $election->{$status_prefix . 'close_time'};
  $open_status = t('<span class="election-status-open">Open</span>');
  $closed_status = t('<span class="election-status-closed">Closed</span>');
  if ($status == ELECTION_STATUS_SCHEDULED) {
    if (REQUEST_TIME < $open_time) {
      return t('Open from: %date', array('%date' => date('g.ia, j M Y', $open_time)));
    }
    if (REQUEST_TIME < $close_time) {
      return t('Scheduled (now open)');
    }
  }
  if (_election_is_open($status, $open_time, $close_time)) {
    return $open_status;
  }
  return $closed_status;
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
