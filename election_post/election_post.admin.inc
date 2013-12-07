<?php
/**
 * @file
 * Administrative page callbacks for election posts.
 */

/**
 * Page callback for managing election posts.
 */
function election_post_manage_page(stdClass $election) {
  drupal_set_title(t('Manage @posts', array(
    '@posts' => election_get_post_name($election->type, TRUE),
  )), PASS_THROUGH);
  election_set_breadcrumb($election);
  $view = views_get_view('election_posts_admin');
  $view->override_path = current_path();
  return $view->preview('embed', array($election->election_id));
}
