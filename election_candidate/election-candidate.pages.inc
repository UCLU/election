<?php
/**
 * @file
 * Page callbacks and form functions for election candidates and nominations
 * (Election module).
 */

/**
 * Page callback for election/%election/candidates/list.
 */
function election_candidate_list_page(stdClass $election) {

  $view = views_embed_view('election_candidates', 'candidates_per_election', $election->election_id);

  drupal_set_title(t('Candidates'), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  return array(
    '#markup' => $view,
  );

}

/**
 * Page callback for election-candidate/%election_candidate.
 */
function election_candidate_page_view(stdClass $candidate, $view_mode = 'full') {

  $election = election_load($candidate->election_id);

  if ($view_mode == 'full' || $view_mode == 'details') {
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
  }

  return election_candidate_view($candidate, $election, $view_mode);

}

/**
 * Generate a table of a user's previous nominations in an election.
 *
 * @param stdClass $election
 *   An election object.
 * @param array $previous_nominations
 *   An array of candidate objects representing the user's previous nominations.
 *
 * @todo make this themeable
 */
function election_candidate_display_previous_nominations(stdClass $election, array $previous_nominations) {
  if (empty($previous_nominations)) {
    return;
  }
  $caption = t('Your previous nominations in this election');
  $header = array(
    t('Nomination date'),
    drupal_ucfirst(election_get_post_name($election->type)),
    t('Candidate name'),
    t('Status'),
  );
  $rows = array();
  foreach ($previous_nominations as $candidate) {
    $post = election_post_load($candidate->post_id);
    $post_title = empty($post) ? '?' : check_plain($post->title);
    $rows[] = array(
      format_date($candidate->created, 'short'),
      $post_title,
      check_plain($candidate->first_name . ' ' . $candidate->last_name),
      _election_candidate_format_status($candidate),
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
 * Election nominations form function. This creates election_candidate entities.
 *
 * @param array $form
 *   The Drupal Form API form definition.
 * @param array $form_state
 *   The Form API state array.
 * @param stdClass $post
 *   The election post object.
 *
 * @return array
 *   Form API definition.
 */
function election_candidate_nomination_form($form, &$form_state, stdClass $post) {

  $election = $post->election;

  drupal_set_title(t('Nomination form: %post', array(
    '%post' => truncate_utf8($post->title, 50, TRUE, TRUE, 30),
  )), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb(
    $election,
    l($post->title, election_post_uri_path($post))
  ));

  $candidate_type = !empty($post->settings['candidate_type']) ? $post->settings['candidate_type'] : 'candidate';
  $candidate = entity_get_controller('election_candidate')->create(array(
    'type' => $candidate_type,
    'is_new' => TRUE,
  ));

  $form['#id'] = 'election-nomination-form';

  if (!isset($form_state['seconder_count'])) {
    $form_state['seconder_count'] = 1;
  }

  $form_state['candidate'] = $candidate;
  $form_state['election'] = $election;
  $form_state['post'] = $post;

  $previous_nominations = (array) election_candidate_get_nominations($election);
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

  $form['post'] = array(
    '#type' => 'item',
    '#title' => check_plain(drupal_ucfirst(election_get_post_name($election->type))),
    '#markup' => check_plain($post->title),
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
    '#default_value' => empty($GLOBALS['user']->mail) ? NULL : $GLOBALS['user']->mail,
  );

  // Add custom fields from the Field system.
  field_attach_form('election_candidate', $candidate, $form, $form_state);

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit nomination'),
    '#submit' => array('election_candidate_nomination_form_submit'),
  );

  $form['#validate'][] = 'election_candidate_form_validate';

  return $form;
}


/**
 * Validation function for both the candidate edit form and the nomination form.
 */
function election_candidate_form_validate($form, &$form_state) {

  if (!empty($form_state['values']['mail']) && !filter_var($form_state['values']['mail'], FILTER_VALIDATE_EMAIL)) {
    form_set_error('mail', t('Please enter a valid email address'));
  }

  field_attach_form_validate('election_candidate', $form_state['candidate'], $form, $form_state);

}

/**
 * Validation function for editing a candidate.
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
 */
function election_candidate_nomination_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  $values = $form_state['values'];

  $election = $form_state['election'];
  $post = $form_state['post'];

  $candidate->election_id = $election->election_id;
  $candidate->post_id = $post->post_id;

  $candidate->first_name = $values['first_name'];
  $candidate->last_name = $values['last_name'];
  $candidate->mail = $values['mail'];
  $candidate->cstatus = ELECTION_CANDIDATE_HOPEFUL;

  field_attach_submit('election_candidate', $candidate, $form, $form_state);

  if (!empty($GLOBALS['user']->uid)) {
    $candidate->uid = $GLOBALS['user']->uid;
    $candidate->username = $GLOBALS['user']->name;
  }

  // Save the nomination.
  election_candidate_save($candidate);

  // Notify the user that the nomination was saved.
  drupal_set_message(t(
    'The nomination for the @post %title (name %first_name %last_name) was successfully submitted.',
    array(
      '@post' => election_get_post_name($election->type),
      '%title' => $post->title,
      '%first_name' => $candidate->first_name,
      '%last_name' => $candidate->last_name,
    )
  ));

  // Invoke rules for "After submitting a nomination".
  if (module_exists('rules')) {
    rules_invoke_event('election_candidate_nomination_submitted', $GLOBALS['user'], $candidate, $post, $election);
  }

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_uri_path($election);
  }

}

/**
 * Generate an array for rendering the given candidate.
 *
 * @param stdClass $candidate
 *   An election candidate object.
 * @param stdClass $election
 *   The election to which the candidate belongs (optional).
 * @param string $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 *
 * @return array
 *   An array as expected by drupal_render().
 */
function election_candidate_view($candidate, $election = NULL, $view_mode = 'full') {

  if (empty($election)) {
    $election = election_load($candidate->election_id);
  }

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
  // We don't need duplicate rendering info in $candidate->content.
  unset($candidate->content);

  $post = election_post_load($candidate->post_id);

  $build += array(
    '#theme' => 'election_candidate',
    '#candidate' => $candidate,
    '#election' => $election,
    '#post' => $post,
    '#view_mode' => $view_mode,
  );

  // Add contextual links for this candidate, except when the candidate is
  // already being displayed on its own page.
  if (!empty($candidate->candidate_id) && $view_mode != 'full') {
    $build['#contextual_links']['election_candidate'] = array('election_candidate', array($candidate->candidate_id));
  }

  // Allow modules to modify the structured election.
  $type = 'election_candidate';
  drupal_alter(array('election_candidate_view', 'entity_view'), $build, $type);

  return $build;

}

/**
 * Form builder function for editing a single candidate.
 */
function election_candidate_edit_form($form, &$form_state, stdClass $candidate) {

  $election = election_load($candidate->election_id);

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

  $post = election_post_load($candidate->post_id);
  $post_name = election_get_post_name($election->type);
  $form['post'] = array(
    '#type' => 'item',
    '#title' => check_plain(drupal_ucfirst($post_name)),
    '#markup' => $post ? check_plain($post->title) : t('Not defined'),
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

  field_attach_form('election_candidate', $candidate, $form, $form_state);

  foreach ($form as $key => &$field) {
    if (strpos($key, 'field_') === 0 && isset($field['#required'])) {
      $field['#required'] = FALSE;
    }
  }

  $form['cstatus'] = array(
    '#type' => 'select',
    '#options' => _election_candidate_get_cstatus_options(),
    '#title' => t('Status'),
    '#required' => TRUE,
    '#default_value' => $candidate->cstatus,
    '#empty_option' => t('- Select status -'),
    '#weight' => 97,
  );

  $form['current_user'] = array(
    '#title' => t('Associated user account'),
    '#type' => 'item',
    '#default_value' => empty($candidate_account) ? NULL : $candidate_account->name,
    '#weight' => 98,
  );
  if (empty($candidate_account)) {
    $form['current_user']['#markup'] = t('No user account associated.');
  }
  else {
    $form['current_user']['#markup'] = theme(
      'username',
      array('account' => $candidate_account, 'link_options' => array('attributes' => array('target' => '_blank')))
    );
    $form['current_user']['#description'] = t('The account that submitted the nomination.');
  }

  $form['published'] = array(
    '#type' => 'checkbox',
    '#title' => t('Published'),
    '#default_value' => isset($candidate->published) ? $candidate->published : 0,
    '#weight' => 99,
  );

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
    '#limit_validation_errors' => array(),
  );

  $form['#validate'][] = 'election_candidate_form_validate';
  $form['#validate'][] = 'election_candidate_edit_form_validate';

  return $form;

}

/**
 * Form submit function for the Save button in election_candidate_edit_form.
 */
function election_candidate_edit_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  foreach ($candidate as $property => $value) {
    if (isset($form_state['values'][$property])) {
      $candidate->$property = $form_state['values'][$property];
    }
  }

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
 * Form submit function for the Delete button in election_candidate_edit_form.
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
 */
function election_candidate_edit_form_delete_confirm($form, &$form_state, stdClass $candidate) {

  $election = election_load($candidate->election_id);

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
 * Form submit function for confirming delete for an election candidate.
 */
function election_candidate_edit_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {

    $candidate = election_candidate_load($form_state['values']['candidate_id']);
    $election = election_load($candidate->election_id);

    $info = array(
      '%first_name' => $candidate->first_name,
      '%last_name' => $candidate->last_name,
      '%election_title' => !empty($election) ? $election->title : t('unknown'),
    );

    // Delete the candidate.
    $delete = election_candidate_delete($form_state['values']['candidate_id']);
    if ($delete) {
      watchdog('election_candidate', 'Deleted candidate "%first_name %last_name".', $info);
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
