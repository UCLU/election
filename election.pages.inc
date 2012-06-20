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
  }

  return election_view($election, $view_mode);

}
