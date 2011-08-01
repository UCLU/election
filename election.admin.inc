<?php
/**
 * @file
 * Administration page callbacks for the Election module.
 *
 * @todo Implement all of this with Views.
 */


/**
 * Page callback for editing an election post
 */
function election_post_page_edit($election, $post) {
  $title = election_post_page_title($election, $post);
  drupal_set_title(t('Edit: %title', array('%title' => $title)), PASS_THROUGH);

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, 'Posts', l($post->title, election_post_uri_path($post)), 'Edit')
  );

  return drupal_get_form('election_post_form', $election, $post);
}

function election_post_page_add($election) {

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, 'Posts', 'Add new')
  );

  global $user;

  if (!is_object($election)) {
    return MENU_NOT_FOUND;
  }

  $post = entity_get_controller('election_post')->create($election);

  $post_name = $election->type;
  $type = _election_type_load($election->type);
  if (!empty($type->{'post name'})) {
    $post_name = $type->{'post name'};
  }

  drupal_set_title(
    t('Add new @post_name to %election', array('%election' => $election->title, '@post_name' => $post_name)),
    PASS_THROUGH
  );

  return drupal_get_form('election_post_form', $election, $post);

}

/**
 * Page callback for editing an election
 */
function election_page_edit($election) {

  $type = _election_type_load($election->type);
  $title = t(
    'Edit @type: %title',
    array(
      '@type' => $type->name,
      '%title' => $election->title,
    )
  );

  drupal_set_title($title, PASS_THROUGH);
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, 'Edit')
  );

  return drupal_get_form('election_form', $election);
}


/**
 * Main election list (called via drupal_get_form).
 *
 * @return array
 * Drupal form array.
 */
function election_list($form, $form_state) {

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'e.title'),
    'type' => array('data' => t('Type'), 'field' => 'e.type'),
    'anonymity' => array('data' => t('Anonymity'), 'field' => 'e.anonymous'),
    'status' => array('data' => t('Voting status'), 'field' => 'e.status'),
    'open' => array('data' => t('Voting opens'), 'field' => 'e.open_time'),
    'close' => array('data' => t('Voting closes'), 'field' => 'e.close_time'),
    'changed' => array('data' => t('Last updated'), 'field' => 'e.changed', 'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  // Build the election query, adding a pager and sorter.
  $query = db_select('election', 'e')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('e', array('election_id'))
    ->limit(50)
    ->orderByHeader($header);

  // Get an array of election IDs
  $election_ids = $query->execute()->fetchCol();

  $elections = election_load_multiple($election_ids);

  $destination = drupal_get_destination();

  $options = array();
  foreach ($elections as $id => $election) {

    $type = _election_type_load($election->type);

    $options[$election->election_id] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $election->title,
          '#href' => election_uri_path($election),
        ),
      ),
      'type' => check_plain($type->name),
      'anonymity' => $election->anonymous? t('Anonymous') : t('Tracked'),
      'status' => _election_format_status($election->status, $election->open_time, $election->close_time),
      'open' => '',
      'close' => '',
      'changed' => format_date($election->changed, 'short'),
      'operations' => array(),
    );

    foreach (array('open', 'close') as $event) {
      if ($election->status != ELECTION_STATUS_SCHEDULED) {
        $options[$election->election_id][$event] = t('N/A');
        continue;
      }
      $time = $election->{$event . '_time'};
      if (is_numeric($time)) {
        $options[$election->election_id][$event] = format_date($time, 'short');
      }
      elseif (empty($election->open_time)) {
        $options[$election->election_id][$event] = t('Not set');
      }
    }

    $post_name_plural = $election->type;
    if (!empty($type->{'post name plural'})) {
      $post_name_plural = $type->{'post name plural'};
    }
    else if (!empty($type->{'post key'})) {
      $post_name_plural = $election->type;
    }

    $operations = array();
    if (election_access('update', $election)) {
      $operations['posts'] = l(
        t('!post_name_plural list', array('!post_name_plural' => $post_name_plural)),
        election_uri_path($election) . '/posts'
      );
    }

    if (election_access('update', $election)) {
      $operations['edit'] = l(
        t('Edit'),
        election_uri_path($election) . '/edit',
        array('query' => $destination)
      );
    }

    if (election_access('delete', $election)) {
      $operations['delete'] = l(
        t('Delete'),
        election_uri_path($election) . '/delete',
        array('query' => $destination)
      );
    }

    if (count($operations)) {
      // Render an unordered list of operations links.
      $options[$election->election_id]['operations'] = array(
        'data' => array(
          '#theme' => 'item_list',
          '#items' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }

  }

  if (FALSE) { // don't bother with tableselect yet
    $form['elections'] = array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#empty' => t('No elections available.'),
    );
  }
  else {
    $form['elections'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#empty' => t('No elections available.'),
    );
  }

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

function _election_is_open($status, $open_time, $close_time) {

  if ($status == ELECTION_STATUS_OPEN) {
    return TRUE;
  }
  if ($status == ELECTION_STATUS_CLOSED) {
    return FALSE;
  }

  return (
    $open_time && REQUEST_TIME >= $open_time
    && (!$close_time || REQUEST_TIME < $close_time)
  );

}

function _election_format_status($status_number, $open, $close) {

  $is_open = _election_is_open($status_number, $open, $close);

  $open_status = t('<span class="election-status-open">Open</span>');
  $closed_status = t('<span class="election-status-closed">Closed</span>');

  $status = $is_open? $open_status : $closed_status;

  if ($status_number == ELECTION_STATUS_SCHEDULED) {
    return t('Scheduled (!status)', array(
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

function election_page_type($type_mn) {
  $type = _election_type_load($type_mn);
  return $type->name . ': ' . $type->description;
}

function election_page_add($type_mn) {

  global $user;

  if (!$type = _election_type_load($type_mn)) {
    return MENU_NOT_FOUND;
  }

  $election = entity_get_controller('election')->create($type_mn);

  drupal_set_title(
    t('Add new @type', array('@type' => $type->name)),
    PASS_THROUGH
  );

  return drupal_get_form('election_form', $election);

}


function election_form($form, &$form_state, $election) {

  $form['#id'] = 'election-form';
  $form['#election'] = $election;

  $form_state['election'] = $election;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($election->title)? $election->title : '',
    '#required' => TRUE,
  );

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Voting status'),
    '#default_value' => isset($election->status)? $election->status : ELECTION_STATUS_CLOSED,
    '#options' => array(
      ELECTION_STATUS_CLOSED => t('Closed'),
      ELECTION_STATUS_OPEN => t('Open'),
      ELECTION_STATUS_SCHEDULED => t('Scheduled'),
    ),
  );

  $form['voting_schedule'] = array(
    '#title' => t('Voting schedule'),
    '#type' => 'fieldset',
    '#description' => 'Set the opening and closing times for voting.',
    '#states' => array(
      'visible' => array(
        ':input[name=status]' => array('value' => (string) ELECTION_STATUS_SCHEDULED),
      ),
    ),
  );

  $open_default = isset($election->open_time)? $election->open_time : strtotime('+1 Friday 10am');
  $close_default = isset($election->close_time)? $election->close_time : strtotime('+2 Friday 10am');

  $date_format = variable_get('date_format_long', 'Y-m-d H:i');

  $form['voting_schedule']['open_time'] = array(
    '#title' => t('Opening time'),
    '#type' => module_exists('date_popup')? 'date_popup' : 'date_text',
    '#default_value' => $open_default? date('Y-m-d H:i:s', $open_default) : NULL,
    '#date_format' => $date_format,
    '#date_increment' => 5,
    '#date_year_range' => '-0:+3',
  );

  $form['voting_schedule']['close_time'] = array(
    '#title' => t('Closing time'),
    '#type' => module_exists('date_popup')? 'date_popup' : 'date_text',
    '#default_value' => $close_default? date('Y-m-d H:i:s', $close_default) : NULL,
    '#date_format' => $date_format,
    '#date_increment' => 5,
    '#date_year_range' => '-0:+3',
  );

  $form['anonymous'] = array(
    '#title' => t('Voter anonymity'),
    '#type' => 'select',
    '#default_value' => isset($election->anonymous)? $election->anonymous : 1,
    '#options' => array(
       1 => 'Anonymous: voters can never be identified.',
       0 => 'Tracked: voters can be identified if necessary.',
    ),
    '#required' => TRUE,
    '#description' => 'If this is set to Tracked, it is technically possible (although not easy) to link votes with voters. This may be useful for testing purposes. If this is set to Anonymous during voting, it is impossible to make this link. Tracking won\'t work retrospectively.',
    '#weight' => 80,
  );

  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('election_form_submit'),
  );

  if (!empty($election->election_id)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('election_form_delete_submit'),
      '#access' => election_access('delete', $election),
    );
  }

  $form['#validate'][] = 'election_form_validate';

  // Add custom fields from the Field system.
  field_attach_form('election', $election, $form, $form_state);

  return $form;

}

function election_form_validate($form, &$form_state) {

  // Check that the opening and closing times are set if voting is scheduled.
  if ($form_state['values']['status'] == ELECTION_STATUS_SCHEDULED
      && (empty($form_state['values']['open_time']) || empty($form_state['values']['close_time']))) {
    form_set_error('voting_schedule', t('You must set the opening and closing times when the voting status is !scheduled.', array('!scheduled' => t('Scheduled'))));
  }

  // Check that the closing time is after the opening time.
  elseif ($form_state['values']['status'] == ELECTION_STATUS_SCHEDULED
        && strtotime($form_state['values']['close_time']) <= strtotime($form_state['values']['open_time'])) {
    form_set_error('voting_schedule', t('The closing time must be after the opening time.'));
  }

  // Check other Fields.
  field_attach_form_validate('election', $form_state['election'], $form, $form_state);

}

function election_form_submit($form, &$form_state) {
  global $user;
  $election = &$form_state['election'];

  if (empty($election->uid)) {
    $election->uid = $user->uid;
  }

  $election->title = $form_state['values']['title'];

  $election->status = $form_state['values']['status'];

  $election->open_time = strtotime($form_state['values']['open_time']);
  $election->close_time = strtotime($form_state['values']['close_time']);

  $election->anonymous = $form_state['values']['anonymous'];

  field_attach_submit('election', $election, $form, $form_state);

  // Save the election.
  election_save($election);

  // Notify the user that the election was saved.
  $types = election_types();
  $type = $types[$election->type];
  drupal_set_message(
    t('The @type "@title" was saved.', array('@type' => $type->name, '@title' => $election->title))
  );

  $form_state['redirect'] = 'admin/election/list';
}

function election_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $election = $form['#election'];
  $form_state['redirect'] = array(
    election_uri_path($election) . '/delete',
    array('query' => $destination),
  );
}

function election_delete_confirm($form, &$form_state, $election) {
  $form['#election'] = $election;
  $form['election_id'] = array(
    '#type' => 'value',
    '#value' => $election->election_id,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete %title?', array('%title' => $election->title)),
    election_uri_path($election),
    t('This action cannot be undone'),
    t('Delete'),
    t('Cancel')
  );
}

function election_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $election = election_load($form_state['values']['election_id']);
    $delete = election_delete($form_state['values']['election_id']);
    if ($delete) {
      watchdog('election', 'Deleted election "%title" (type: @type)', array('@type' => $election->type, '%title' => $election->title));
    }

    $type = _election_type_load($election->type);

    if ($delete) {
      drupal_set_message(
        t('The @type %title has been deleted.', array('@type' => $type->name, '%title' => $election->title))
      );
    }
    else {
      drupal_set_message(
        t('Failed to delete the @type %title.', array('@type' => $type->name, '%title' => $election->title)),
        'error'
      );
    }

  }
  $form_state['redirect'] = 'admin/election/list';
}



function election_post_form($form, &$form_state, $election, $post) {

  $form['#id'] = 'election-post-form';
  $form['#post'] = $post;

  $form_state['post'] = $post;
  $form_state['election'] = $election;

  $type = _election_type_load($election->type);
  $post_name = empty($type->{'post name'})? t('Post') : $type->{'post name'};

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('@post_type_name title', array('@post_type_name' => $post_name)),
    '#default_value' => isset($post->title)? $post->title : '',
    '#required' => TRUE,
  );

  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('election_post_form_submit'),
  );

  if (!empty($election->election_id)) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('election_post_form_delete_submit'),
      '#access' => election_access('update', $election), // deleting a post is really 'updating' an election
    );
  }

  $form['#validate'][] = 'election_post_form_validate';

  // Add custom fields from the Field system.
  field_attach_form('election_post', $post, $form, $form_state);

  return $form;
}


function election_post_form_validate($form, &$form_state) {

  // Run posts form checks here.

  // Check other Fields.
  field_attach_form_validate('election_post', $form_state['post'], $form, $form_state);

}


function election_post_form_submit($form, &$form_state) {

  $post = &$form_state['post'];

  // election object won't be changed, no need to pass by reference
  $election = $form_state['election'];

  if (empty($post->uid)) {
    global $user;
    $post->uid = $user->uid;
  }

  $post->title = $form_state['values']['title'];

  field_attach_submit('election_post', $post, $form, $form_state);

  // Save the post.
  election_post_save($post);

  $post_name = '';
  if ($type = _election_type_load($election->type)) {
    $post_name = $type->{'post key'};
    if (!empty($type->{'post name'})) {
      $post_name = $type->{'post name'};
    }
  }

  // Notify the user that the post was saved.
  drupal_set_message(
    t('The @post_name "@title" was saved.', array('@post_name' => $post_name, '@title' => $post->title))
  );

  $form_state['redirect'] = election_uri_path($election) . '/posts';

}
