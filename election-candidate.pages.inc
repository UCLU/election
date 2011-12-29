<?php
/**
 * @file
 * Page callbacks and form functions for election candidates and nominations
 * (Election module).
 */

/**
 * Page callback for election/%election/candidates/%election_candidate
 */
function election_candidate_page_view($election, $candidate) {

  drupal_set_title(
    t(
      'Candidate: @first-name @last-name',
      array(
        '@first-name' => $candidate->first_name,
        '@last-name' => $candidate->last_name,
      )
    ),
    PASS_THROUGH
  );

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_CANDIDATES_')
  );

  $uri = entity_uri('election_candidate', $candidate, $election);
  // Set the candidate path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);

  $view_mode = entity_access('edit', 'election_candidate', $candidate)? 'details' : 'full';

  return election_candidate_view($candidate, $election, $view_mode);

}

/**
 * Generate a table of a user's previous nominations in an election.
 */
function election_candidate_display_previous_nominations(stdClass $election, array $previous_nominations) {
  if (empty($previous_nominations)) {
    return;
  }
  $caption = t('Your previous nominations in this election');
  $header = array(
    t('Nomination date'),
    drupal_ucfirst(_election_get_posts_name($election->type)),
    t('Candidate name'),
    t('Status'),
  );
  $rows = array();
  foreach ($previous_nominations as $nomination) {
    $post = election_post_load($nomination->post_id);
    $post_title = empty($post)? '?' : check_plain($post->title);
    $rows[] = array(
      format_date($nomination->created, 'short'),
      $post_title,
      check_plain($nomination->first_name . ' ' . $nomination->last_name),
      _election_candidate_format_status($nomination)
    );
  }
  return array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#caption' => $caption,
  );
}

/**
 * Page callback for election/%election/nomination-form
 */
function election_page_nomination_form(stdClass $election) {

  // @todo this should not be necessary
  drupal_set_title(t('Nomination form'), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  if ($election->type == 'referendum') {
    return MENU_NOT_FOUND;
  }

  $nopen = _election_is_open($election->nstatus, $election->nopen_time, $election->nclose_time);
  if (!$nopen && !user_access('bypass nomination schedule')) {
    return array(
      '#markup' => t('Nominations are currently closed.'),
    );
  }

  $previous_nominations = (array) election_candidate_user_nominations($election);

  $available_post_ids = (array) election_candidate_available_posts($election, $previous_nominations);

  // End with an error message if there are no available posts.
  if (empty($available_post_ids)) {
    $posts_name = _election_get_posts_name($election->type, TRUE);
    return array(
      election_candidate_display_previous_nominations($election, $previous_nominations),
      array(
        '#markup' => t(
          'There are no more @posts-name available for nomination in this election.',
          array('@posts-name' => $posts_name)
        ),
      ),
    );
  }
  $available_posts = (array) election_post_load_multiple($available_post_ids);

  return drupal_get_form('election_candidate_nomination_form', $election, $previous_nominations, $available_posts);

}

/*
 * Election nominations form function. This creates election_candidate entities.
 */
function election_candidate_nomination_form($form, &$form_state, stdClass $election, array $previous_nominations, array $available_posts) {

  global $user;

  $candidate = entity_get_controller('election_candidate')->create();
  $candidate->is_new = TRUE;

  $form['#id'] = 'election-nomination-form';

  if (!isset($form_state['seconder_count'])) {
    $form_state['seconder_count'] = 1;
  }

  $form_state['candidate'] = $candidate;
  $form_state['election'] = $election;

  if (count($previous_nominations)) {
    $form['previous_nominations'] = array(
      '#type' => 'fieldset',
      '#title' => t('Your previous nominations'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => -50,
    );
    $previous_nominations_display = election_candidate_display_previous_nominations($election, $previous_nominations);
    unset($previous_nominations_display['#caption']);
    $form['previous_nominations']['display'] = $previous_nominations_display;
  }

  $bypass = user_access('bypass nomination access control');
  $post_options = array();
  foreach ($available_posts as $post) {
    if ($bypass || $post->nstatus_inheritance == ELECTION_POST_STATUS_INHERIT) {
      $post_options[$post->post_id] = $post->title;
    }
  }
  $post_name = _election_get_posts_name($election->type);
  $form['post_fieldset'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#description' => t(
      'Select the @post_name you wish to stand for. If you cannot find a @post_name in this list, that may be because you have already submitted a nomination.',
      array('@post_name' => $post_name)
    ),
    '#title' => check_plain(drupal_ucfirst($post_name)),
  );
  $form['post_fieldset']['post'] = array(
    '#type' => 'select',
    '#options' => $post_options,
    '#title' => check_plain(drupal_ucfirst($post_name)),
    '#empty_option' => t('- Select @post_name -', array('@post_name' => $post_name)),
    '#required' => TRUE,
  );

  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Candidate details'),
    '#collapsible' => FALSE,
  );

  $form['details']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name(s)'),
    '#max_length' => 100,
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['details']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name(s)'),
    '#max_length' => 100,
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['details']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#default_value' => empty($user->mail)? NULL: $user->mail,
    '#size' => 30,
    '#required' => TRUE,
  );

  $form['details']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact telephone number'),
    '#description' => t('Please enter a valid phone number so that we can contact you about your nomination.'),
    '#size' => 12,
    '#required' => TRUE,
  );

  // Add custom fields from the Field system.
  field_attach_form('election_candidate', $candidate, $form, $form_state);

  $form['#validate'][] = 'election_candidate_form_validate';

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit your nomination'),
    '#submit' => array('election_candidate_nomination_form_submit'),
  );

  return $form;
}


/**
 * Validation function for both the candidate edit form and the nomination form.
 */
function election_candidate_form_validate($form, &$form_state) {

  if (!empty($form_state['values']['mail']) && !filter_var($form_state['values']['mail'], FILTER_VALIDATE_EMAIL)) {
    form_set_error('mail', t('Please enter a valid email address'));
  }

  if (!empty($form_state['values']['phone']) && !preg_match('/\b\+?([0-9]+[\s-\+]?)+\b/', $form_state['values']['phone'])) {
    form_set_error('phone', t('Please enter a valid phone number.'));
  }

  field_attach_form_validate('election_candidate', $form_state['candidate'], $form, $form_state);

}

/**
 * Validation function for just the nomination form.
 */
function election_candidate_nomination_form_validate($form, &$form_state) {
  // @todo validate post again
}

function election_candidate_nomination_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  // election object won't be changed, no need to pass by reference
  $election = $form_state['election'];

  $values = $form_state['values'];

  $candidate->election_id = $election->election_id;

  $post = election_post_load($values['post']);
  $candidate->post_id = $post->post_id;

  $candidate->first_name = $values['first_name'];
  $candidate->last_name = $values['last_name'];
  $candidate->mail = $values['mail'];
  $candidate->phone = $values['phone'];
  $candidate->status = ELECTION_CANDIDATE_PENDING;

  // @todo set up manifesto, summary, photo fields

  field_attach_submit('election_candidate', $candidate, $form, $form_state);

  global $user;
  if (!empty($user->uid)) {
    $candidate->uid = $user->uid;
    $candidate->username = $user->name;
  }

  // Save the nomination.
  election_candidate_save($candidate);

  // Notify the user that the post was saved.
  drupal_set_message(t(
    'Your nomination for the @posts_name %title was successfully submitted.',
    array(
      '@posts_name' => _election_get_posts_name($election->type),
      '%title' => $post->title,
    )
  ));

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_uri_path($election);
  }

}

/**
 * Generate an array for rendering the given candidate.
 *
 * @param $candidate
 *   An election object.
 * @param $election
 *   An (optional) election object.
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser', 'details'...
 *
 * @return
 *   An array as expected by drupal_render().
 */
function election_candidate_view($candidate, $election = NULL, $view_mode = 'full') {

  // Remove previously built content, if it exists.
  $candidate->content = array();

  $title = t('Candidate: @first-name @last-name', array('@first-name' => $candidate->first_name, '@last-name' => $candidate->last_name));

  if ($view_mode == 'teaser') {
    $candidate->content['title'] = array(
      '#markup' => $title,
      '#weight' => -5,
    );
  }
  else {
    drupal_set_title($title);
  }

  // Build fields content.
  // In case of a multiple view, node_view_multiple() already ran the
  // 'prepare_view' step. An internal flag prevents the operation from running
  // twice.
  field_attach_prepare_view('election_candidate', array($candidate->candidate_id => $candidate), $view_mode);
  entity_prepare_view('election_candidate', array($candidate->candidate_id => $candidate));
  $candidate->content += field_attach_view('election_candidate', $candidate, $view_mode);

  $build = $candidate->content;
  // We don't need duplicate rendering info in election->content.
  unset($candidate->content);

  $post = election_post_load($candidate->post_id);

  if (empty($election)) {
    $election = election_load($post->election_id);
  }

  $build += array(
    '#theme' => 'election_candidate',
    '#candidate' => $candidate,
    '#election' => $election,
    '#post' => $post,
    '#view_mode' => $view_mode,
  );

  // Add contextual links for this election, except when the election is already being
  // displayed on its own page. Modules may alter this behavior (for example,
  // to restrict contextual links to certain view modes) by implementing
  // hook_election_candidate_view_alter().
  if (!empty($candidate->candidate_id) && $view_mode != 'full') {
    $build['#contextual_links']['election_candidate'] = array('election_candidate', array($candidate->candidate_id));
  }

  // Allow modules to modify the structured election.
  $type = 'election_candidate'; // $type is passed by reference to drupal_alter() so it must be a variable.
  drupal_alter(array('election_candidate_view', 'entity_view'), $build, $type);

  return $build;

}

/*
 * Form builder function for editing a single candidate.
 *
 * @param array $form
 * @param array $form_state
 * @param stdClass $election
 * @param stdClass $candidate
 *
 * @return array
 *   Render array.
 */
function election_candidate_edit_form($form, $form_state, stdClass $election, stdClass $candidate) {

 // @todo this should not be necessary
  drupal_set_title(t('Edit candidate: %first-name %last-name', array('%first-name' => $candidate->first_name, '%last-name' => $candidate->last_name)), PASS_THROUGH);
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_CANDIDATES_')
  );

  $form['#id'] = 'election-form';
  $form['#election'] = $election;
  $form['#candidate'] = $election;

  $form_state['election'] = $election;
  $form_state['candidate'] = $candidate;

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('election_form_submit'),
  );
  $form['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('election_candidate_form_delete_submit'),
    '#access' => entity_access('delete', 'election_candidate', $candidate),
  );

  $form['#validate'][] = 'election_candidate_form_validate';

  // Add custom fields from the Field system.
  field_attach_form('election_candidate', $candidate, $form, $form_state);

  return $form;

}

/**
 * Form builder function for the main administrative election candidate list.
 *
 * @param array $form
 * @param array $form_state
 * @param stdClass $election
 *
 * @return array
 *   Render array.
 */
function election_candidate_list_form($form, $form_state, $election) {

 // @todo this should not be necessary
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
    'status' => array('data' => t('Status')),
    'operations' => array('data' => t('Operations')),
  );

  // Build the query, adding a pager and sorter.
  $query = db_select('election_candidate', 'ec')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ec', array('candidate_id', 'post_id'))
    ->condition('ec.election_id', $election->election_id)
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
    if (!empty($candidate->post_id)) {
      $post = election_post_load($candidate->post_id);
      $post_title = check_plain($post->title);
    }

    $options[$candidate->candidate_id] = array(
      'created' => format_date($candidate->created, 'short'),
      'post_title' => $post_title,
      'first_name' => check_plain($candidate->first_name),
      'last_name' => check_plain($candidate->last_name),
      'status' => _election_candidate_format_status($candidate),
    );

    $operations = array();
    $candidate_uri_path = election_candidate_uri_path($candidate);

    if (election_candidate_access('view', $candidate)) {
      $operations['view'] = l(
        t('view'),
        $candidate_uri_path
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
          '#attributes' => array('class' => array('links', 'inline')),
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
