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

  // Remove previously built content, if it exists.
  $election->content = array();

  if ($view_mode == 'teaser') {
    $election->content['title'] = array(
      '#markup' => filter_xss($election->title),
      '#weight' => -5,
    );
  }
  else {
    drupal_set_title($election->title);
  }

  field_attach_prepare_view('election', array($election->election_id => $election), $view_mode);
  entity_prepare_view('election', array($election->election_id => $election));
  $election->content += field_attach_view('election', $election, $view_mode);

  return $election->content;

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
      l(drupal_ucfirst(_election_get_posts_name($election->type, TRUE)), election_uri_path($election) . '/posts'),
      check_plain($post->title)
    )
  );

  $post->content = array();

  if ($view_mode == 'teaser') {
    $post->content['title'] = array(
      '#markup' => filter_xss($post->title),
      '#weight' => -5,
    );
  }

  field_attach_prepare_view('election_post', array($post->post_id => $post), $view_mode);
  entity_prepare_view('election_post', array($post->post_id => $post));
  $post->content += field_attach_view('election_post', $post, $view_mode);

  return $post->content;

}