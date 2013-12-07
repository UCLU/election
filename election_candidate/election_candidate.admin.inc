<?php
/**
 * @file
 * Administrative page callbacks for election candidates and nominations
 * (Election module).
 */

/**
 * Page callback for managing election candidates.
 */
function election_candidate_manage_page(stdClass $election) {
  drupal_set_title(t('Manage candidates'), PASS_THROUGH);
  election_set_breadcrumb($election);
  $view = views_get_view('election_candidates_admin');
  // Explicitly set the path, so that Views Bulk Operations could work if
  // configured (VBO needs help setting the form's "action" attribute).
  $view->override_path = current_path();
  return $view->preview('embed', array($election->election_id));
}

/**
 * Page callback for editing an election candidate.
 */
function election_candidate_page_edit($candidate) {

  drupal_set_title(t('Edit candidate: %first-name %last-name', array('%first-name' => $candidate->first_name, '%last-name' => $candidate->last_name)), PASS_THROUGH);

  $post = election_post_load($candidate->post_id);
  if ($post) {
    $election = $post->election;
    election_post_set_breadcrumb($election, $post);
  }
  else {
    $election = election_load($candidate->election_id);
    election_set_breadcrumb($election);
  }

  module_load_include('inc', 'election_candidate', 'election_candidate.forms');

  return drupal_get_form('election_candidate_form', $candidate, $post);
}

/**
 * Page callback for adding an election candidate.
 */
function election_candidate_page_add($post) {

  if (!is_object($post)) {
    return MENU_NOT_FOUND;
  }

  $election = $post->election;
  election_post_set_breadcrumb($election, $post);

  $candidate_type = !empty($post->settings['candidate_type']) ? $post->settings['candidate_type'] : 'candidate';
  $candidate = entity_get_controller('election_candidate')->create(array(
    'type' => $candidate_type,
    'election_id' => $election->election_id,
    'post_id' => $post->post_id,
    'is_new' => TRUE,
  ));

  drupal_set_title(
    t('Add new candidate to %post', array('%post' => $post->title)),
    PASS_THROUGH
  );

  module_load_include('inc', 'election_candidate', 'election_candidate.forms');

  return drupal_get_form('election_candidate_form', $candidate, $post);

}

/**
 * Page callback to display total numbers of candidates per post.
 */
function election_candidate_totals_page(stdClass $election) {

  drupal_set_title(t('Candidate totals'), PASS_THROUGH);
  election_set_breadcrumb($election);

  // Get the total number of candidates for each status.
  $query = db_select('election_candidate')
    ->condition('election_id', $election->election_id);
  $query->addExpression("IFNULL(cstatus, 'all')");
  $query->addExpression('COUNT(DISTINCT candidate_id)');
  $query->groupBy('cstatus WITH ROLLUP');
  $totals = $query->execute()->fetchAllKeyed();

  $output = array(
    'total' => array(
      '#markup' => '<p>' . t('Total number of candidates: !totalAll', array('!totalAll' => empty($totals['all']) ? 0 : number_format($totals['all']))) . '</p>',
    ),
  );

  if (!empty($totals['all'])) {
    // Get the total number of candidates for each status, per post.
    $query = db_select('election_post', 'ep');
    $query->join('election_candidate', 'ec', 'ep.post_id = ec.post_id');
    $query->addExpression('COUNT(DISTINCT candidate_id)', 'num');
    $query->fields('ep', array('post_id', 'title'))
      ->fields('ec', array('cstatus'))
      ->condition('ep.election_id', $election->election_id)
      ->groupBy('ep.post_id')
      ->groupBy('ec.cstatus')
      ->orderBy('ep.title');
    $result = $query->execute();
    $cstatuses = _election_candidate_get_cstatus_options();
    $header = array_merge(
      array(drupal_ucfirst(election_get_post_name($election->type))),
      $cstatuses
    );
    $rows = array();
    $rows['total'] = array(t('TOTAL'));
    foreach (array_keys($cstatuses) as $cstatus) {
      $rows['total'][] = isset($totals[$cstatus]) ? $totals[$cstatus] : '0';
    }
    $counts = $result->fetchAll(PDO::FETCH_GROUP);
    foreach ($counts as $post_id => $subtotals) {
      $row = array();
      $row[] = check_plain($subtotals[0]->title);
      foreach (array_keys($cstatuses) as $cstatus) {
        $num = 0;
        foreach ($subtotals as $subtotal) {
          if ($subtotal->cstatus == $cstatus) {
            $num = $subtotal->num;
            break;
          }
        }
        $row[] = $num;
      }
      $rows[] = $row;
    }
    $output['by_position'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }

  return $output;
}
