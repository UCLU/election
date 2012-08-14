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
  $variables['view_mode'] = $variables['elements']['#view_mode'];

  $variables['teaser'] = $variables['view_mode'] == 'teaser';
  $variables['details'] = $variables['view_mode'] == 'details';
  $variables['page'] = ($variables['view_mode'] == 'full' || $variables['view_mode'] == 'details');

  $variables['election'] = $variables['elements']['#election'];
  $variables['candidate'] = $variables['elements']['#candidate'];
  $variables['post'] = $variables['elements']['#post'];

  $election = $variables['election'];
  $candidate = $variables['candidate'];
  $post = $variables['post'];
  $election_type = _election_type_get_info($election->type);

  if (empty($post)) {
    // Handle gracefully when the candidate's post is deleted prematurely.
    $post = entity_get_controller('election_post')->create(array(
      'type' => $election_type['post machine name'],
    ));
  }

  $uri = election_candidate_uri($candidate);
  $variables['url'] = url($uri['path'], $uri['options']);

  $election_uri = entity_uri('election', $election);
  $variables['election_url'] = url($election_uri['path'], $election_uri['options']);
  $variables['election_title'] = check_plain($election->title);
  $variables['election_link'] = l($election->title, $election_uri['path'], $election_uri['options']);
  $variables['election_type'] = check_plain($election_type['name']);

  $post_uri = entity_uri('election_post', $post);
  $variables['post_url'] = url($post_uri['path'], $post_uri['options']);
  $variables['post_title'] = check_plain($post->title);
  $variables['post_link'] = l($post->title, $post_uri['path'], $post_uri['options']);
  $variables['post_name'] = check_plain($election_type['post name']);

  $variables['full_name'] = t('@first_name @last_name', array('@first_name' => $candidate->first_name, '@last_name' => $candidate->last_name));

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
 * Process variables for election-candidate-details.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $candidate
 * - $election
 * - $page
 *
 * @see election-candidate.tpl.php
 */
function template_preprocess_election_candidate_details(&$variables) {

  $variables['election'] = $variables['elements']['#election'];
  $variables['candidate'] = $variables['elements']['#candidate'];
  $variables['post'] = $variables['elements']['#post'];

  $election = $variables['election'];
  $candidate = $variables['candidate'];
  $post = $variables['post'];
  $election_type = _election_type_get_info($election->type);

  if (empty($post)) {
    // Handle gracefully when the candidate's post is deleted prematurely.
    $post = entity_get_controller('election_post')->create(array(
      'type' => $election_type['post machine name'],
    ));
  }

  $uri = election_candidate_uri($candidate);
  $variables['url'] = url($uri['path'], $uri['options']);

  $election_uri = entity_uri('election', $election);
  $variables['election_url'] = url($election_uri['path'], $election_uri['options']);
  $variables['election_link'] = l($election->title, $election_uri['path'], $election_uri['options']);

  $post_uri = entity_uri('election_post', $post);
  $variables['post_url'] = url($post_uri['path'], $post_uri['options']);
  $variables['post_link'] = l($post->title, $post_uri['path'], $post_uri['options']);

  $variables['full_name'] = t('@first_name @last_name', array('@first_name' => $candidate->first_name, '@last_name' => $candidate->last_name));

  $variables['content']['election'] = array(
    '#theme' => 'container',
    '#attributes' => array(
      'class' => array('pseudo-field', 'candidate-election'),
    ),
    '#markup' => '<span class="label">' . t('Election:') . '</span>'
    . ' <span class="item">' . $variables['election_link'] . '</span>',
  );

  $variables['content']['post'] = array(
    '#theme' => 'container',
    '#attributes' => array(
      'class' => array('pseudo-field', 'candidate-post'),
    ),
    '#markup' => '<span class="label">' . drupal_ucfirst($election_type['post name'] . ':') . '</span>'
    . ' <span class="item">' . $variables['post_link'] . '</span>',
  );

  $variables['content']['full-name'] = array(
    '#theme' => 'container',
    '#attributes' => array(
      'class' => array('pseudo-field', 'candidate-full-name'),
    ),
    '#markup' => '<span class="label">' . t('Name:') . '</span>'
    . ' <span class="item">' . $variables['full_name'] . '</span>',
  );

  $variables['username'] = theme('username', array('#account' => $candidate->uid ? user_load($candidate->uid) : NULL));
  $variables['phone'] = check_plain($candidate->phone);
  $variables['mail'] = l($candidate->mail, 'mailto:' . $candidate->mail, array('absolute' => TRUE));

  $variables['content']['mail'] = array(
    '#theme' => 'container',
    '#attributes' => array(
      'class' => array('pseudo-field', 'candidate-email-address'),
    ),
    '#markup' => '<span class="label">' . t('Email address:') . '</span>'
    . ' <span class="item">' . $variables['mail'] . '</span>',
  );

  $variables['content']['phone'] = array(
    '#theme' => 'container',
    '#attributes' => array(
      'class' => array('pseudo-field', 'candidate-phone'),
    ),
    '#markup' => '<span class="label">' . t('Phone number:') . '</span>'
    . ' <span class="item">' . $variables['phone'] . '</span>',
  );

  $variables['content']['candidate-username'] = array(
    '#theme' => 'container',
    '#attributes' => array(
      'class' => array('pseudo-field', 'candidate-username'),
    ),
    '#markup' => '<span class="label">' . t('Username:') . '</span>'
    . ' <span class="item">' . $variables['username'] . '</span>',
  );

  $variables += array('content' => array());
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  $variables['classes_array'][] = drupal_html_class('election-' . $election->type . '-candidate', 'election-candidate-details');

  $variables['theme_hook_suggestions'][] = 'election__' . $election->type . '__candidate_details';

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
  return '<span class="election-candidate-ballot-item">' . l($candidate->first_name . ' ' . $candidate->last_name, election_candidate_uri_path($candidate)) . '</span>';
}
