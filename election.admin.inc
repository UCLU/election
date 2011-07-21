<?php
/**
 * @file
 * Administration page callbacks for the Election module.
 */


/**
 * Main election list (called via drupal_get_form).
 *
 * @return array
 * Drupal form array.
 */
function election_list($form, $form_state) {

  // Build the sortable table header.
  $header = array(
    'name' => array('data' => t('Name'), 'field' => 'e.name'),
    'type' => array('data' => t('Type'), 'field' => 'e.type'),
    'status' => array('data' => t('Status'), 'field' => 'e.status'),
    'open' => array('data' => t('Opening time'), 'field' => 'e.open_time'),
    'close' => array('data' => t('Closing time'), 'field' => 'e.close_time'),
    'changed' => array('data' => t('Updated'), 'field' => 'e.changed', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  // Build the election query, adding a pager and sorter.
  $query = db_select('election', 'e')->extend('PagerDefault')->extend('TableSort');

  // Get an array of election IDs
  $election_ids = $query
                  ->fields('e', array('election_id'))
                  ->limit(50)
                  ->orderByHeader($header)
                  ->execute()
                  ->fetchCol();

  $elections = election_load_multiple($election_ids);

  $destination = drupal_get_destination();

  $options = array();
  foreach ($elections as $id => $election) {
    $options[$election->election_id] = array(
      'name' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $election->name,
          '#href' => sprintf('election/%d', $id),
        ),
      ),
      'type' => check_plain(election_type_load($election->type)->name),
      'close' => array('data' => t('Voting status'), 'field' => 'e.close_time'),
      'status' => _election_format_status($election->status, $election->open_time, $election->close_time),
      'open' => $election->open_time? format_date($election->open_time, 'short') : '<em>Not set!</em>',
      'close' => $election->close_time? format_date($election->close_time, 'short') : '<em>Not set!</em>',
      'changed' => format_date($election->changed, 'short'),
      'operations' => array(),
    );

    $operations = array();
    if (entity_access('update', 'election', $election)) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => sprintf('election/%d/edit', $id),
        'query' => $destination,
      );
    }
    if (entity_access('delete', 'election', $election)) {
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => sprintf('election/%d/delete', $id),
        'query' => $destination,
      );
    }

    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$node->nid]['operations'] = array(
        'data' => array(
          '#theme' => 'item_list',
          '#items' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
    elseif (!empty($operations)) {
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$node->nid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array('query' => $link['query']),
        ),
      );
    }

  }

  if (user_access('administer elections')) {
    $form['elections'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No elections available.'),
    );
  }
  else {
    // @todo work out why this is #theme while the above is #type
    $form['elections'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No elections available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

function _election_is_open($status, $open_time, $close_time) {

  if ($status == ELECTION_STATUS_OPEN) {
    return true;
  }
  if ($status == ELECTION_STATUS_CLOSED) {
    return false;
  }

  return ($open_time && $close_time && $close_time > $open_time && REQUEST_TIME >= $open_time && REQUEST_TIME < $close_time);
}

function _election_format_status($status_number, $open, $close) {

  $is_open = _election_is_open($status_number, $open, $close);

  $open_status = t('<span class="election-status-open">open</span>');
  $closed_status = t('<span class="election-status-closed">closed</span>');

  $status = $is_open? $open_status : $closed_status;

  if ($status_number == ELECTION_STATUS_AUTO) {
    return t('auto (!status)', array(
      '!status' => $status,
    ));
  }

  return $status;
}


function election_page_type_overview() {

  $item = menu_get_item();

  $output = array();

  foreach (election_types() as $type_mn => $type) {

    $output['content'][] = array(
      'title' => 'Manage ' . $type->name,
      'href' => $item['href'] . '/' . $type_mn,
      'localized_options' => $item['localized_options'],
      'description' => $type->description,
    );

  }

  return theme('admin_block_content', $output);

}

function election_type_title($type_mn) {
  if (!$type = election_type_load($type_mn)) {
    return FALSE;
  }
  return $type->name;
}

function election_page_title($election) {
  return $election->name;
}

function election_page_view($election, $view_mode = 'full') {

  // Remove previously built content, if it exists.
  $election->content = array();

  if ($view_mode == 'teaser') {
    $election->content['title'] = array(
      '#markup' => filter_xss($election->name),
      '#weight' => -5,
    );
  }
  else {
    drupal_set_title($election->name);
  }

  $election->content['description'] = array(
    '#markup' => filter_xss($election->description),
    '#weight' => 0,
  );

  field_attach_prepare_view('election', array($election->election_id => $election), $view_mode);
  entity_prepare_view('election', array($election->election_id => $election));
  $election->content += field_attach_view('election', $election, $view_mode);

  return $election->content;
}

function election_page_type($type_mn) {
  $type = election_type_load($type_mn);
  return $type->name . ': ' . $type->description;
}

function election_page_add($type_mn) {

  global $user;

  if (!$type = election_type_load($type_mn)) {
    return MENU_NOT_FOUND;
  }

  $election = entity_get_controller('election')->create($type_mn);

  drupal_set_title(
    t('Add new @name', array('@name' => $type->name)),
    PASS_THROUGH
  );

  return drupal_get_form($type_mn . '_election_form', $election);

}