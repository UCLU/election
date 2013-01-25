<?php
/**
 * @file
 * Theme and preprocess functions for the Election Candidate module.
 */

/**
 * Process variables for election-candidate.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $candidate
 * - $election
 * - $view_mode
 * - $page
 *
 * @see election-candidate.tpl.php
 */
function template_preprocess_election_candidate(&$variables) {

  $candidate = $variables['elements']['#candidate'];
  $election = $variables['elements']['#election'];
  $post = $variables['elements']['#post'];
  $view_mode = $variables['elements']['#view_mode'];

  // Detect whether the candidate is on its own page.
  $page_candidate = menu_get_object('election_candidate');
  $is_page = (!empty($page_candidate) && $page_candidate->candidate_id == $candidate->candidate_id);

  // Create some basic useful variables.
  $variables['view_mode'] = $view_mode;
  $variables['teaser'] = ($view_mode == 'teaser');
  $variables['page'] = $is_page;
  $variables['details'] = $variables['view_mode'] == 'details';
  $variables['candidate'] = $candidate;
  $variables['election'] = $election;
  $variables['post'] = $post;
  $uri = election_candidate_uri($candidate);
  $variables['candidate_url'] = url($uri['path'], $uri['options']);

  $variables['full_name'] = theme('election_candidate_full_name', array('candidate' => $candidate));
  $variables['first_name'] = check_plain($candidate->first_name);
  $variables['last_name'] = check_plain($candidate->last_name);

  $variables += array('content' => array());

  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('election_candidate', $candidate, $variables['content'], $variables);

  $variables['classes_array'][] = drupal_html_class('election-' . $election->type . '-candidate');

  $variables['theme_hook_suggestions'][] = 'election_candidate__' . $candidate->candidate_id;
  $variables['theme_hook_suggestions'][] = 'election__' . $election->type . '__candidate';
  $variables['theme_hook_suggestions'][] = 'election__' . $election->election_id . '__candidate';

}

/**
 * Process variables for theme_election_candidate_status().
 */
function template_preprocess_election_candidate_status(&$variables) {
  $candidate = $variables['candidate'];
  $variables['status'] = check_plain(_election_candidate_status_name($candidate));
  $variables['classes'] = array(
    'election-candidate-status',
    drupal_html_class('election-candidate-status-' . $variables['status']),
  );
}

/**
 * Theme function for displaying a candidate's status.
 */
function theme_election_candidate_status($variables) {
  $attributes = array();
  if ($variables['classes']) {
    $attributes['class'] = $variables['classes'];
  }
  return '<span' . drupal_attributes($attributes) . '>'
    . check_plain($variables['status'])
    . '</span>';
}

/**
 * Theme function for displaying a candidate's full name.
 */
function theme_election_candidate_full_name($variables) {
  $candidate = $variables['candidate'];
  return check_plain($candidate->first_name . ' ' . $candidate->last_name);
}

/**
 * Theme function for displaying a candidate as an item on a ballot form.
 */
function theme_election_candidate_ballot_item($variables) {
  $candidate = $variables['candidate'];
  $full_name = theme('election_candidate_full_name', array('candidate' => $candidate));
  $link = l($full_name, election_candidate_uri_path($candidate));
  return '<span class="election-candidate-ballot-item">' . $link . '</span>';
}
