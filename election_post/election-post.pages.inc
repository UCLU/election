<?php
/**
 * @file
 * Page callbacks for the Election Post module.
 */

/**
 * Page callback for 'election/%election/posts/list'.
 */
function election_post_list_page(stdClass $election) {

  $posts_name = _election_get_posts_name($election->type, TRUE);

  switch ($election->type) {
    case 'referendum':
      $view = views_embed_view('election_posts', 'block_referendum', $election->election_id);
      break;

    case 'stv':
      $view = views_embed_view('election_posts', 'block_stv', $election->election_id);
      break;

    default:
      $view = views_embed_view('election_posts', 'default', $election->election_id);
      break;

  }

  drupal_set_title(drupal_ucfirst($posts_name), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  return array(
    '#markup' => $view,
  );

}
