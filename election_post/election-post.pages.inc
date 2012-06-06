<?php
/**
 * @file
 * Page callbacks for the Election Post module.
 */

/**
 * Page callback for election/%election/posts/list
 */
function election_post_list_page(stdClass $election) {

  $posts_name = _election_get_posts_name($election->type, TRUE);

  switch($posts_name) {
    case 'motions':
      $view = views_embed_view('election_motions', 'page', $election->election_id);
      break;
    case 'positions':
      $view = views_embed_view('election_positions', 'page', $election->election_id);
      break;
    default:
      $view = views_embed_view('election_' . $posts_name, 'page', $election->election_id);
      break;
  }

  drupal_set_title(drupal_ucfirst($posts_name), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  return array(
    '#markup' => $view,
  );

}
