<?php
/**
 * @file
 * Menu callbacks for pages in the Election module.
 */

/**
 * Menu callback for an election, path: election/%election.
 *
 * @param stdClass $election
 * @param string $view_mode
 *
 * @return array
 *   Render array.
 */
function election_page_view($election, $view_mode = 'full') {

  if ($view_mode == 'full') {
    drupal_set_title($election->title);
    drupal_set_breadcrumb(_election_build_breadcrumb());
    $uri = election_uri($election);
    drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
    drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);
  }

  return election_view($election, $view_mode);

}
