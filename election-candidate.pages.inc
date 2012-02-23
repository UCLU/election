<?php
/**
 * @file
 * Page callbacks and form functions for election candidates and nominations
 * (Election module).
 */

/**
 * Page callback for election/%election/candidate/%election_candidate
 *
 * @param stdClass $election
 * @param stdClass $candidate
 */
function election_candidate_page_view(stdClass $election, stdClass $candidate, $view_mode = 'full') {

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

  return election_candidate_view($candidate, $election, $view_mode);

}

/**
 * Page callback for election/%election/candidate/%election_candidate/details
 *
 * @param stdClass $election
 * @param stdClass $candidate
 */
function election_candidate_details_page_view(stdClass $election, stdClass $candidate) {

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

  $post = election_post_load($candidate->post_id);

  return array(
    '#theme' => 'election_candidate_details',
    '#candidate' => $candidate,
    '#election' => $election,
    '#post' => $post,
  );

}


/**
 * Generate a table of a user's previous nominations in an election.
 *
 * @param stdClass $election
 * @param array $previous_nominations
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

  $previous_nominations = (array) election_candidate_get_user_nominations($election);

  $available_post_ids = (array) election_candidate_get_available_posts($election, $previous_nominations);

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
 *
 * @param array $form
 * @param array $form_state
 * @param stdClass $election
 * @param array $previous_nominations
 * @param array $available_posts
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
      // @todo make this alterable
      $post_options[$post->post_id] = truncate_utf8($post->title, 100, TRUE, TRUE, 80);
    }
  }
  drupal_alter('election_candidate_post_options', $post_options);
  $post_name = _election_get_posts_name($election->type);
  $form['post_id'] = array(
    '#type' => 'select',
    '#options' => $post_options,
    '#title' => check_plain(drupal_ucfirst($post_name)),
    '#empty_option' => t('- Select @post_name -', array('@post_name' => $post_name)),
    '#required' => TRUE,
  );

  $form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name(s)'),
    '#max_length' => 100,
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name(s)'),
    '#max_length' => 100,
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#size' => 30,
    '#required' => TRUE,
    '#default_value' => empty($user->mail)? NULL: $user->mail,
  );

  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact telephone number'),
    '#size' => 12,
    '#required' => TRUE, // @todo make this alterable
  );

  // Add custom fields from the Field system.
  field_attach_form('election_candidate', $candidate, $form, $form_state);

  $form['#validate'][] = 'election_candidate_form_validate';
  $form['#validate'][] = 'election_candidate_nomination_form_validate';

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit nomination'),
    '#submit' => array('election_candidate_nomination_form_submit'),
  );

  return $form;
}


/**
 * Validation function for both the candidate edit form and the nomination form.
 *
 * @param array $form
 * @param array $form_state
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
 *
 * @param array $form
 * @param array $form_state
 */
function election_candidate_nomination_form_validate($form, &$form_state) {
  // @todo validate post again
}

/**
 * Validation function for just the edit form.
 *
 * @param array $form
 * @param array $form_state
 */
function election_candidate_edit_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['new_user'])) {
    if (!$account = user_load_by_name($form_state['values']['new_user'])) {
      form_set_error('new_user', t('The username %name does not exist', array('%name' => $form_state['values']['new_user'])));
    }
  }
}


/**
 * Submit callback for the nomination form.
 *
 * @param array $form
 * @param array $form_state
 */
function election_candidate_nomination_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  $values = $form_state['values'];

  // election object won't be changed, no need to pass by reference
  $election = $form_state['election'];
  $post = election_post_load($values['post_id']);

  $candidate->election_id = $election->election_id;

  $candidate->post_id = $values['post_id'];

  $candidate->first_name = $values['first_name'];
  $candidate->last_name = $values['last_name'];
  $candidate->mail = $values['mail'];
  $candidate->phone = $values['phone'];
  $candidate->cstatus = ELECTION_CANDIDATE_PENDING;

  // @todo set up manifesto, summary, photo fields

  field_attach_submit('election_candidate', $candidate, $form, $form_state);

  global $user;
  if (!empty($user->uid)) {
    $candidate->uid = $user->uid;
    $candidate->username = $user->name;
  }

  // Save the nomination.
  election_candidate_save($candidate);

  // Trigger a Rules action.
  if (function_exists('rules_invoke_event')) {
    rules_invoke_event('election_candidate_nomination_submitted', $candidate, $post, $election);
  }

  // Notify the user that the nomination was saved.
  drupal_set_message(t(
    'The nomination for the @posts_name %title (name %first_name %last_name) was successfully submitted.',
    array(
      '@posts_name' => _election_get_posts_name($election->type),
      '%title' => $post->title,
      '%first_name' => $candidate->first_name,
      '%last_name' => $candidate->last_name,
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
 * @param $election
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 *
 * @return
 *   An array as expected by drupal_render().
 */
function election_candidate_view($candidate, $election, $view_mode = 'full') {

  // Remove previously built content, if it exists.
  $candidate->content = array();

  $title = t('Candidate: @first-name @last-name', array('@first-name' => $candidate->first_name, '@last-name' => $candidate->last_name));

  if ($view_mode == 'teaser') {
    $candidate->content['title'] = array(
      '#markup' => $title,
      '#weight' => -5,
    );
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

  $form['#id'] = 'election-candidate-edit-form';
  $form['#candidate'] = $candidate;

  $form_state['election'] = $election;
  $form_state['candidate'] = $candidate;

  $candidate_account = NULL;
  if (!empty($candidate->uid)) {
    $candidate_account = user_load($candidate->uid);
  }

  $form['cstatus'] = array(
    '#type' => 'select',
    '#options' => array(
      ELECTION_CANDIDATE_PENDING => t('Pending approval'),
      ELECTION_CANDIDATE_APPROVED => t('Approved'),
      ELECTION_CANDIDATE_WITHDRAWN => t('Withdrawn'),
      ELECTION_CANDIDATE_REJECTED => t('Rejected'),
    ),
    '#title' => t('Candidate status'),
    '#required' => TRUE,
    '#default_value' => $candidate->cstatus,
    '#empty_option' => t('- Select status -'),
    '#description' => t(
      'If the status is set to !approved, the candidate will be visible to all those who have permission to view the election.',
      array('!approved' => t('Approved'))
    ),
    '#weight' => 0,
  );

  $form['user_info']['current_user'] = array(
    '#title' => t('Associated user account'),
    '#type' => 'item',
    '#default_value' => empty($candidate_account)? NULL : $candidate_account->name,
    '#weight' => 1,
  );
  if (empty($account)) {
    $form['user_info']['current_user']['#markup'] = t('No user account associated.');
  }
  else {
    $form['user_info']['current_user']['#markup'] = theme(
      'username',
      array('account' => $candidate_account, 'link_options' => array('target' => '_blank'))
    );
    $form['user_info']['current_user']['#description'] = t('The account that submitted the nomination.');
  }

  $post_ids = election_candidate_get_available_posts($election, NULL, FALSE);
  $available_posts = election_post_load_multiple($post_ids);
  $post_options = array();
  foreach ($available_posts as $post) {
    $post_options[$post->post_id] = truncate_utf8($post->title, 100, TRUE, TRUE, 80);
  }
  $post_name = _election_get_posts_name($election->type);
  $form['post'] = array(
    '#type' => 'select',
    '#options' => $post_options,
    '#title' => check_plain(drupal_ucfirst($post_name)),
    '#empty_option' => t('- Select @post_name -', array('@post_name' => $post_name)),
    '#required' => TRUE,
    '#default_value' => $candidate->post_id,
  );

  $form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name(s)'),
    '#max_length' => 100,
    '#default_value' => $candidate->first_name,
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name(s)'),
    '#max_length' => 100,
    '#default_value' => $candidate->last_name,
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#size' => 30,
    '#required' => FALSE,
    '#default_value' => $candidate->mail,
  );

  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact telephone number'),
    '#size' => 12,
    '#required' => FALSE,
    '#default_value' => $candidate->phone,
  );

  field_attach_form('election_candidate', $candidate, $form, $form_state);

  foreach ($form as $key => &$field) {
    if (strpos($key, 'field_') === 0 && isset($field['#required'])) {
      $field['#required'] = FALSE;
    }
  }

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('election_candidate_edit_form_submit'),
  );
  $form['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('election_candidate_edit_form_delete_submit'),
    '#access' => entity_access('delete', 'election_candidate', $candidate),
    '#limit_validation_errors' => array(), // No need to validate on deletion
  );

  $form['#validate'][] = 'election_candidate_form_validate';
  $form['#validate'][] = 'election_candidate_edit_form_validate';

  return $form;

}

/**
 * Form submit function for the Save button in the form election_candidate_edit_form().
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_candidate_edit_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  foreach ($candidate as $property => $value) {
    if (isset($form_state['values'][$property])) {
      $candidate->$property = $form_state['values'][$property];
    }
  }

  $candidate->post_id = $form_state['values']['post'];

  if (!empty($form_state['values']['new_user'])) {
    if ($account = user_load_by_name($form_state['values']['new_user'])) {
      $candidate->uid = $account->uid;
      $candidate->username = $account->username;
    }
  }

  field_attach_submit('election_candidate', $candidate, $form, $form_state);

  election_candidate_save($candidate);

  drupal_set_message(
    t(
      'The candidate %first_name %last_name was saved.',
      array(
        '%first_name' => $candidate->first_name,
        '%last_name' => $candidate->last_name,
      )
    )
  );

}

/**
 * Form submit function for the Delete button in the form election_candidate_edit_form.
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_candidate_edit_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $candidate = $form['#candidate'];
  $form_state['redirect'] = array(
    election_candidate_uri_path($candidate) . '/delete',
    array('query' => $destination),
  );
}

/**
 * Form builder function to confirm the delete action for a candidate.
 * See http://api.drupal.org/api/drupal/modules--system--system.module/function/confirm_form.
 *
 * @param array $form
 * @param array &$form_state
 * @param stdClass $election
 *
 * @return array
 *   Render array.
 */
function election_candidate_edit_form_delete_confirm($form, &$form_state, $election, $candidate) {

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_CANDIDATES_')
  );

  $form['candidate_id'] = array(
    '#type' => 'value',
    '#value' => $candidate->candidate_id,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the candidate %title?', array('%title' => $candidate->first_name . ' ' . $candidate->last_name)),
    election_candidate_uri_path($candidate),
    t('Deleting a candidate also deletes ALL associated data, for example votes. This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submit function for the Delete button in the form election_candidate_edit_form_delete_confirm.
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_candidate_edit_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {

    $candidate = election_candidate_load($form_state['values']['candidate_id']);
    $election = election_load($candidate->election_id);

    $info = array(
      '%first_name' => $candidate->first_name,
      '%last_name' => $candidate->last_name,
      '%election_title' => !empty($election)? $election->title : t('unknown'),
    );

    // Delete the candidate
    $delete = election_candidate_delete($form_state['values']['candidate_id']);
    if ($delete) {
      watchdog('election', 'Deleted candidate "%first_name %last_name".', $info);
    }

    if ($delete) {
      drupal_set_message(
        t('The candidate %first_name %last_name has been deleted from the election %election_title.', $info)
      );
    }
    else {
      drupal_set_message(
        t('Failed to delete the candidate %first_name %last_name from the election %election_title.', $info)
      );
    }

  }

  if (empty($_GET['destination']) && $election) {
    $form_state['redirect'] = election_uri_path($election) . '/candidates';
  }

}
