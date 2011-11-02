<?php
/**
 * @file
 * Menu callbacks for pages in the Election module.
 *
 * @todo
 * Theming and Views
 */

/**
 * Menu callback for an election, path: election/%election.
 *
 * @param stdClass $election
 * @param string $view_mode
 *   Optional.
 *
 * @return array
 *   Render array.
 */
function election_page_view($election, $view_mode = 'full') {

  // If there is a menu link to this node, the link becomes the last part
  // of the active trail, and the link name becomes the page title.
  // Thus, we must explicitly set the page title to be the election title.
  drupal_set_title($election->title);

  $uri = entity_uri('election', $election);
  // Set the election path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);

  return election_view($election, 'full');

}

/**
 * Menu callback for an election post, path: election/%election/posts/%election_post.
 *
 * @param stdClass $election
 * @param stdClass $post
 * @param string $view_mode
 *   Optional.
 *
 * @return array
 *   Render array.
 */
function election_post_page_view($election, $post, $view_mode = 'full') {

  drupal_set_breadcrumb(
    _election_build_breadcrumb(
      $election,
      l(drupal_ucfirst(_election_get_posts_name($election->type, TRUE)), election_uri_path($election) . '/posts')
    )
  );

  return election_post_view($election, $post, $view_mode);

}