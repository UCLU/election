<?php
/**
 * @file
 * Theme and preprocess functions for the Election module.
 */

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

  $variables['vstatus'] = _election_format_status($election, 'v');
  $variables['nstatus'] = _election_format_status($election, 'n');

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
