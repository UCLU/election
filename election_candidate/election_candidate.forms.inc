<?php
/**
 * @file
 * Form functions for the Election Candidate module.
 */

/**
 * Form builder function for editing a single candidate.
 */
function election_candidate_form($form, &$form_state, stdClass $candidate, stdClass $post) {

  global $user;

  $election = $post->election;
  $candidate->is_new = empty($candidate->is_new) ? FALSE : TRUE;

  $form['#id'] = 'election-candidate-form';
  $form['#candidate'] = $candidate;

  $form_state['election'] = $election;
  $form_state['candidate'] = $candidate;

  // Determine whether the user is an 'election admin', i.e. he/she has access
  // to update the election. This automatically gives access to create, update,
  // or delete candidates in this election.
  $election_admin = election_access('update', $election);

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
    '#type' => module_exists('elements') ? 'emailfield' : 'textfield',
    '#title' => t('E-mail address'),
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

  if ($election_admin) {
    $form['cstatus'] = array(
      '#type' => 'select',
      '#options' => _election_candidate_get_cstatus_options(),
      '#title' => t('Status'),
      '#required' => TRUE,
      '#default_value' => $candidate->cstatus,
      '#empty_option' => t('- Select status -'),
      '#weight' => 97,
    );
    $candidate_account = NULL;
    if (!empty($candidate->uid)) {
      $candidate_account = user_load($candidate->uid);
    }
    $form['user'] = array(
      '#title' => t('Associated user account'),
      '#type' => 'textfield',
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($candidate_account) ? $user->name : $candidate_account->name,
      '#weight' => 98,
    );
    $form['published'] = array(
      '#type' => 'checkbox',
      '#title' => t('Published'),
      '#default_value' => isset($candidate->published) ? $candidate->published : 0,
      '#weight' => 99,
    );
  }

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('election_candidate_form_submit'),
  );

  if ($election_admin && !$candidate->is_new) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('election_candidate_form_delete_submit'),
      '#access' => entity_access('delete', 'election_candidate', $candidate),
      '#limit_validation_errors' => array(),
    );
  }

  if (!empty($_GET['destination'])) {
    $options = drupal_parse_url($_GET['destination']);
    $cancel_path = $options['path'];
  }
  elseif (!$candidate->is_new) {
    $cancel_path = election_candidate_uri_path($candidate);
  }
  else {
    $cancel_path = election_post_uri_path($post);
  }
  $form['buttons']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $cancel_path,
  );

  form_load_include($form_state, 'inc', 'election_candidate', 'election_candidate.forms');
  $form['#validate'][] = 'election_candidate_form_validate';

  return $form;

}

/**
 * Form submit function for the Save button in election_candidate_form.
 */
function election_candidate_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  foreach ($candidate as $property => $value) {
    if (isset($form_state['values'][$property])) {
      $candidate->$property = $form_state['values'][$property];
    }
  }

  if (!empty($form_state['values']['user'])) {
    if ($account = user_load_by_name($form_state['values']['user'])) {
      $candidate->uid = $account->uid;
      $candidate->username = $account->name;
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

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_candidate_uri_path($candidate);
  }

}

/**
 * Form submit function for the Delete button in election_candidate_form.
 */
function election_candidate_form_delete_submit($form, &$form_state) {
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
function election_candidate_form_delete_confirm($form, &$form_state, stdClass $candidate) {

  $election = election_load($candidate->election_id);

  election_set_breadcrumb($election);

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
function election_candidate_form_delete_confirm_submit($form, &$form_state) {
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

/**
 * Nomination form for a post. This creates election_candidate entities.
 */
function election_candidate_nomination_form($form, &$form_state, stdClass $post) {

  $candidate_type = !empty($post->settings['candidate_type']) ? $post->settings['candidate_type'] : 'candidate';
  $candidate = entity_get_controller('election_candidate')->create(array(
    'type' => $candidate_type,
    'is_new' => TRUE,
  ));

  $form['#id'] = 'election-nomination-form';

  if (!isset($form_state['seconder_count'])) {
    $form_state['seconder_count'] = 1;
  }

  $election = $post->election;

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
    '#type' => module_exists('elements') ? 'emailfield' : 'textfield',
    '#title' => t('E-mail address'),
    '#size' => 30,
    '#required' => TRUE,
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

  if (!empty($_GET['destination'])) {
    $options = drupal_parse_url($_GET['destination']);
    $cancel_path = $options['path'];
    $form['buttons']['cancel'] = array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => $cancel_path,
    );
  }

  form_load_include($form_state, 'inc', 'election_candidate', 'election_candidate.forms');
  $form['#validate'][] = 'election_candidate_form_validate';

  return $form;
}


/**
 * Validation function for both the candidate edit form and the nomination form.
 */
function election_candidate_form_validate($form, &$form_state) {

  if ($form['mail']['#type'] == 'textfield' && !empty($form_state['values']['mail']) && !filter_var($form_state['values']['mail'], FILTER_VALIDATE_EMAIL)) {
    form_set_error('mail', t('Please enter a valid e-mail address.'));
  }

  field_attach_form_validate('election_candidate', $form_state['candidate'], $form, $form_state);

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
