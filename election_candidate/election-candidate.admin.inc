<?php
/**
 * @file
 * Administrative page callbacks for election candidates and nominations
 * (Election module).
 */

/**
 * Form builder function for the main administrative election candidate list.
 *
 * @param stdClass $form
 *   The Drupal Form API form definition.
 * @param array $form_state
 * @param stdClass $election
 *
 * @return array
 *   Render array.
 */
function election_candidate_manage_form($form, $form_state, $election) {

  drupal_set_title(t('Candidates'), PASS_THROUGH);
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election)
  );

  $post_name = _election_get_posts_name($election->type);

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

  // Get array of candidates
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
    $header = array(t('Position'), t('Pending'), t('Approved'), t('Rejected'), t('Withdrawn'));
    $rows = array();
    $rows[] = array(t('TOTAL'), $total_all->num_pending, $total_all->num_approved, $total_all->num_rejected, $total_all->num_withdrawn);
    foreach ($totals as $total) {
      $rows[] = array(check_plain($total->title), $total->num_pending, $total->num_approved, $total->num_rejected, $total->num_withdrawn);
    }
    $output['by_position'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }

  return $output;
}

/**
 * Form builder function for the form at "election/%election/candidates/download".
 *
 * @param stdClass $form
 *   The Drupal Form API form definition.
 * @param array $form_state
 * @param stdClass $election
 */
function election_candidate_download_form($form, &$form_state, stdClass $election) {

  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  $candidate_field_instances = field_info_instances('election_candidate', 'election_candidate');

  $post_name = _election_get_posts_name($election->type);

  $properties = array(
    'election_id' => t('Election ID'),
    'election_title' => array(
      'label' => t('Election title'),
      'default' => TRUE,
    ),
    'candidate_id' => t('Candidate ID'),
    'first_name' => array(
      'label' => t('First name'),
      'default' => TRUE,
    ),
    'last_name' => array(
      'label' => t('Last name'),
      'default' => TRUE,
    ),
    'post_id' => t('@post_name ID', array('@post_name' => drupal_ucfirst($post_name))),
    'post_title' => array(
      'label' => t('@post_name title', array('@post_name' => drupal_ucfirst($post_name))),
      'default' => TRUE,
    ),
    'cstatus' => array(
      'label' => t('Candidate status'),
      'default' => TRUE,
    ),
    'mail' => array(
      'label' => t('Email address'),
      'default' => TRUE,
    ),
    'phone' => array(
      'label' => t('Phone number'),
      'default' => TRUE,
    ),
    'uid' => t('User ID'),
    'username' => array(
      'label' => t('Username'),
      'default' => TRUE,
    ),
    'created' => array(
      'label' => t('Created date'),
      'default' => TRUE,
      'type' => 'timestamp',
    ),
    'changed' => array(
      'label' => t('Last modified date'),
      'default' => TRUE,
      'type' => 'timestamp',
    ),
    'profile_link' => array(
      'label' => t('Profile link'),
      'default' => TRUE,
    ),
  );
  foreach ($candidate_field_instances as $field_name => $cfi) {
    $field = field_info_field($field_name);
    $properties[$field_name] = array(
      'label' => $cfi['label'],
      'type' => $field['type'],
      'default' => FALSE,
    );
  }

  $property_options = array();
  $default_properties = array();
  foreach ($properties as $property_key => $property) {
    $label = is_string($property) ? $property : $property['label'];
    $property_options[$property_key] = $label;
    if (isset($property['default']) && $property['default'] === TRUE) {
      $default_properties[] = $property_key;
    }
  }

  $form_state['#election'] = $election;
  $form_state['#properties'] = $properties;
  $form_state['#property_options'] = $property_options;

  $form['include_properties'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Properties to include'),
    '#options' => $property_options,
    '#default_value' => $default_properties,
  );

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['download'] = array(
    '#type' => 'submit',
    '#value' => t('Download candidate spreadsheet'),
  );

  return $form;

}

/**
 * Submit callback for the form election_candidate_download_form.
 *
 * @param stdClass $form
 *   The Drupal Form API form definition.
 * @param array $form_state
 */
function election_candidate_download_form_submit($form, &$form_state) {

  $election = $form_state['#election'];

  $file_name = 'candidates_election' . $election->election_id . '_' . date('Y-m-d_His', REQUEST_TIME) . '.csv';

  $include_properties = $form_state['values']['include_properties'];
  foreach ($include_properties as $key => $value) {
    if ($value === 0) {
      unset($include_properties[$key]);
    }
  }

  $properties = array_intersect_key($form_state['#properties'], $include_properties);

  $separator = ',';
  $enclosure = '"';
  $empty = '';

  $outstream = fopen('php://output', 'w');

  $candidate_ids = db_select('election_candidate')
    ->fields('election_candidate', array('candidate_id'))
    ->condition('election_id', $election->election_id)
    ->orderBy('created', 'DESC')
    ->execute()
    ->fetchCol();
  $candidates = election_candidate_load_multiple($candidate_ids);

  drupal_add_http_header('Content-Type', 'text/csv; charset=UTF-8');
  drupal_add_http_header('Cache-Control', 'no-store, max-age=0, must-revalidate');
  drupal_add_http_header('Pragma', FALSE);
  drupal_add_http_header('Content-Description', 'File Transfer');
  drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $file_name . '"');

  $property_headers = array();
  foreach ($properties as $property) {
    $property_headers[] = is_string($property) ? $property : $property['label'];
  }

  fputcsv($outstream, $property_headers, $separator, $enclosure);

  foreach ($candidates as $candidate) {

    $row = array();
    foreach ($properties as $property_key => $property) {
      $value = _election_candidate_download_format_property($candidate, $property_key, $property);
      $row[$property_key] = strlen($value) ? $value : $empty;
    }

    fputcsv($outstream, $row, $separator, $enclosure);
    fpassthru($outstream);
  }

  fpassthru($outstream);
  fclose($outstream);

  exit();

}

/**
 * Format a candidate property.
 *
 * @param stdClass $candidate
 */
function _election_candidate_download_format_property(stdClass $candidate, $property_key, $property) {
  $value = isset($candidate->{$property_key}) ? $candidate->{$property_key} : NULL;
  switch ($property_key) {
    case 'profile_link':
      return url(election_candidate_uri_path($candidate), array('absolute' => TRUE));
      return $election->title;
      break;

    case 'election_title':
      $election = election_load($candidate->election_id);
      return $election->title;
      break;

    case 'post_title':
      if (empty($candidate->post_id)) {
        return;
      }
      $post = election_post_load($candidate->post_id);
      return $post->title;
      break;

    case 'cstatus':
      return _election_candidate_format_status($candidate);
      break;

  }
  $type = 'string';
  if (is_array($property) && !empty($property['type'])) {
    $type = $property['type'];
  }
  switch ($type) {
    case 'timestamp':
      return date('Y-m-d H:i:s', $value);
      break;

    case 'string':
      if (is_string($value)) {
        return check_plain($value);
      }
      break;

    case 'file':
    case 'image':
      if (is_array($value) && isset($value['und'][0]['uri'])) {
        return file_create_url($value['und'][0]['uri']);
      }
      break;

    default:
      if (is_array($value)) {
        if (isset($value['und'][0]['safe_value'])) {
          return $value['und'][0]['safe_value'];
        }
        elseif (isset($value['und'][0]['value'])) {
          return check_plain($value['und'][0]['value']);
        }
      }
  }
}
