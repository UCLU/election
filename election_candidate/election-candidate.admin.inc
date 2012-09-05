<?php
/**
 * @file
 * Administrative page callbacks for election candidates and nominations
 * (Election module).
 */

/**
 * Form builder function for the main administrative election candidate list.
 */
function election_candidate_manage_form($form, $form_state, stdClass $election) {

  drupal_set_title(t('Candidates'), PASS_THROUGH);
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election)
  );

  $post_name = election_get_post_name($election->type);

  // Build the sortable table header.
  $header = array(
    'created' => array('data' => t('Nominated'), 'field' => 'ec.created', 'sort' => 'desc'),
    'post_title' => array('data' => drupal_ucfirst($post_name), 'field' => 'ep.title'),
    'first_name' => array('data' => t('First name'), 'field' => 'ec.first_name'),
    'last_name' => array('data' => t('Last name'), 'field' => 'ec.last_name'),
    'username' => array('data' => t('Username'), 'field' => 'ec.username'),
    'cstatus' => array('data' => t('Status'), 'field' => 'ec.cstatus'),
    'operations' => array('data' => t('Operations')),
  );

  // Build the query, adding a pager and sorter.
  $query = db_select('election_candidate', 'ec')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ec', array('candidate_id'));
  $query->leftJoin('election_post', 'ep', 'ec.post_id=ep.post_id');
  $query->condition('ec.election_id', $election->election_id)
    ->limit(50)
    ->orderByHeader($header);
  $result = $query->execute();

  // Get array of candidates.
  $candidate_ids = $result->fetchCol();
  $candidates = election_candidate_load_multiple($candidate_ids);

  $destination = drupal_get_destination();

  $default_post_title = '<em class="deleted-post">' . t('Deleted?') . '</em>';

  $options = array();
  foreach ($candidates as $id => $candidate) {

    $post_title = $default_post_title;
    if (!empty($candidate->post_id) && ($post = election_post_load($candidate->post_id))) {
      $post_title = check_plain($post->title);
    }

    $account = user_load($candidate->uid);

    $options[$candidate->candidate_id] = array(
      'created' => format_date($candidate->created, 'short'),
      'post_title' => $post_title,
      'first_name' => check_plain($candidate->first_name),
      'last_name' => check_plain($candidate->last_name),
      'username' => $account ? theme('username', array('account' => $account)) : t('Anonymous'),
      'cstatus' => _election_candidate_format_status($candidate),
    );

    $operations = array();
    $candidate_uri_path = election_candidate_uri_path($candidate);

    if (election_candidate_access('view', $candidate)) {
      $operations['view'] = l(
        t('view'),
        $candidate_uri_path
      );
    }

    if (election_candidate_access('view details', $candidate)) {
      $operations['view_details'] = l(
        t('details'),
        $candidate_uri_path . '/details'
      );
    }

    if (election_candidate_access('update', $candidate)) {
      $operations['edit'] = l(
        t('edit'),
        $candidate_uri_path . '/edit',
        array('query' => $destination)
      );
    }

    if (count($operations)) {
      // Render an unordered list of operations links.
      $options[$candidate->candidate_id]['operations'] = array(
        'data' => array(
          '#theme' => 'item_list',
          '#items' => $operations,
          '#attributes' => array('class' => array('operations')),
        ),
      );
    }

  }

  $form['election_candidates'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No candidates available.'),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Display total numbers of candidates per post.
 */
function election_candidate_totals_page(stdClass $election) {

  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  $result = db_query('SELECT COUNT(DISTINCT candidate_id) AS num_candidates, COUNT(DISTINCT IF (cstatus=0, candidate_id, NULL)) AS num_pending, COUNT(DISTINCT IF (cstatus=1, candidate_id, NULL)) AS num_approved, COUNT(DISTINCT IF (cstatus=-1, candidate_id, NULL)) AS num_withdrawn, COUNT(DISTINCT IF (cstatus=-2, candidate_id, NULL)) AS num_rejected FROM {election_candidate} WHERE election_id = :election_id', array(
  ':election_id' => $election->election_id));
  $total_all = $result->fetch();

  $output = array(
    'total' => array(
      '#markup' => '<p>' . t('Total number of candidates: !totalAll', array('!totalAll' => $total_all->num_candidates)) . '</p>',
    ),
  );

  if ($total_all->num_candidates > 0) {
    $result = db_query(
      'SELECT ep.title, COUNT(DISTINCT ec.candidate_id) AS num_candidates, COUNT(DISTINCT IF (ec.cstatus=0, ec.candidate_id, NULL)) AS num_pending, COUNT(DISTINCT IF (ec.cstatus=1, ec.candidate_id, NULL)) AS num_approved, COUNT(DISTINCT IF (ec.cstatus=-1, ec.candidate_id, NULL)) AS num_withdrawn, COUNT(DISTINCT IF (ec.cstatus=-2, ec.candidate_id, NULL)) AS num_rejected FROM {election_post} ep LEFT JOIN {election_candidate} ec USING (post_id) WHERE ep.election_id = :election_id GROUP BY ep.post_id ORDER BY ep.title',
      array(
        ':election_id' => $election->election_id,
      )
    );
    $totals = $result->fetchAll();
    $header = array(
      t('Position'),
      t('Pending'),
      t('Approved'),
      t('Rejected'),
      t('Withdrawn'),
    );
    $rows = array();
    $rows[] = array(
      t('TOTAL'),
      $total_all->num_pending,
      $total_all->num_approved,
      $total_all->num_rejected,
      $total_all->num_withdrawn,
    );
    foreach ($totals as $total) {
      $rows[] = array(
        check_plain($total->title),
        $total->num_pending,
        $total->num_approved,
        $total->num_rejected,
        $total->num_withdrawn,
      );
    }
    $output['by_position'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }

  return $output;
}
