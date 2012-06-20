<?php
/**
 * @file
 * Menu callbacks for pages in the Election module.
 */

/**
 * Menu callback for an election, path: election/%election.
 *
 * @param stdClass $election
 *   An election object.
 * @param string $view_mode
 *   View mode, e.g. 'full', 'teaser'...
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
