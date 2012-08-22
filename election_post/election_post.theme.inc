<?php
/**
 * @file
 * Theme and preprocess functions for the Election Post module.
 */

/**
 * Process variables for election-post.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $election
 * - $view_mode
 * - $page
 *
 * @see election-post.tpl.php
 */
function template_preprocess_election_post(&$variables) {

  $variables['view_mode'] = $variables['elements']['#view_mode'];

  // Provide a distinct $teaser boolean.
  $variables['teaser'] = $variables['view_mode'] == 'teaser';

  $post = $variables['elements']['#post'];
  $election = $post->election;

  $variables['post'] = $post;
  $variables['election'] = $election;

  $uri = election_post_uri($post);
  $variables['election_post_url'] = url($uri['path'], $uri['options']);
  $variables['title'] = check_plain($post->title);
  $variables['page'] = $variables['view_mode'] == 'full';

  $election_uri = election_uri($election);
  $election_type = _election_type_get_info($election->type);
  $variables['election_url'] = url($election_uri['path'], $election_uri['options']);
  $variables['election_title'] = check_plain($election->title);
  $variables['election_link'] = l($election->title, $election_uri['path'], $election_uri['options']);
  $variables['election_type'] = check_plain($election_type['name']);
  $variables['election_type_has_candidates'] = (!empty($election_type['has candidates']));

  // Flatten the post object's member fields.
  $variables = array_merge((array) $post, $variables);

  // Helpful $content variable for templates.
  $variables += array('content' => array());

  $variables['content']['election'] = array(
    '#type' => 'item',
    '#title' => t('In election'),
    '#markup' => $variables['election_link'],
  );

  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  // Make the field variables available with the appropriate language.
  field_attach_preprocess('election_post', $post, $variables['content'], $variables);

  // Gather classes.
  $variables['classes_array'][] = drupal_html_class('election-post-' . $election->type);

  // Clean up name so there are no underscores.
  $variables['theme_hook_suggestions'][] = 'election_post__' . $election->type;
  $variables['theme_hook_suggestions'][] = 'election_post__' . $election->election_id;

}
