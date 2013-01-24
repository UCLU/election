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

  $post = $variables['elements']['#post'];
  $election = $post->election;
  $view_mode = $variables['elements']['#view_mode'];

  // Create some basic useful variables.
  $variables['view_mode'] = $view_mode;
  $variables['teaser'] = ($view_mode == 'teaser');
  $variables['page'] = ($view_mode == 'full');
  $variables['post'] = $post;
  $variables['election'] = $election;
  $variables['title'] = check_plain($post->title);
  $uri = election_post_uri($post);
  $variables['post_url'] = url($uri['path'], $uri['options']);

  // Flatten the post object's member fields.
  $variables = array_merge((array) $post, $variables);

  // Helpful $content variable for templates.
  $variables += array('content' => array());

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
