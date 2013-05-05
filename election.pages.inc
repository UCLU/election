<?php
/**
 * @file
 * Menu callbacks for the Election module.
 */

/**
 * Page callback for listing elections.
 */
function election_page_list() {
  drupal_set_title(t('Elections'), PASS_THROUGH);
  $view = views_get_view('elections');
  $view->override_path = current_path();
  return $view->preview('embed');
}
