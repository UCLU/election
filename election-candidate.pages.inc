<?php
/**
 * @file
 * Page callbacks for election candidates and nominations (Election module).
 */

function election_page_nomination($election) {

  $title = t('Candidate nomination form');
  drupal_set_breadcrumb(_election_build_breadcrumb($election, $title));
  drupal_set_title($title);

  // Check nominations status (debugging)
  // @todo remove this
  $status = _election_is_open($election->nstatus, $election->nopen_time, $election->nclose_time);
  $status_message = array(
    '#markup' => t(
      'Nominations are !status.',
      array(
        '!status' => $status? t('open') : t('closed'),
      )
    ),
  );

  $candidate = entity_get_controller('election_candidate')->create($election);

  return array(
    $status_message,
    drupal_get_form('election_nomination_form', $election, $candidate),
  );

}

function election_nomination_form($form, &$form_state, $election, $candidate) {

  $candidate->is_new = empty($candidate->candidate_id);

  $form['#id'] = 'election-nomination-form';

  if (!isset($form_state['seconder_count'])) {
    $form_state['seconder_count'] = 1;
  }

  $form_state['candidate'] = $candidate;
  $form_state['election'] = $election;

  $posts_name = _election_get_posts_name($election->type);

  $post_ids = db_query(
    'SELECT post_id FROM {election_post} WHERE election_id = :eid',
    array(':eid' => $election->election_id)
  )->fetchCol();
  $posts = election_post_load_multiple($post_ids);
  $post_options = array();
  foreach ($posts as $post) {
    if ($post->nstatus_inheritance == ELECTION_POST_STATUS_INHERIT) {
      $post_options[$post->post_id] = $post->title;
    }
  }

  $form['post'] = array(
    '#type' => 'select',
    '#title' => drupal_ucfirst($posts_name),
    '#description' => t('Select the @posts_name you wish to stand for.', array('@posts_name' => $posts_name)),
    '#default_value' => isset($candidate->post_id)? $candidate->post_id : NULL,
    '#options' => $post_options,
    '#empty_option' => t('- Select @posts_name -', array('@posts_name' => $posts_name)),
    '#required' => TRUE,
  );

/*
  $form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => isset($candidate->first_name)? $candidate->first_name : NULL,
    '#size' => 20,
    '#required' => TRUE,
  );
  $form['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => isset($candidate->first_name)? $candidate->first_name : NULL,
    '#size' => 20,
    '#required' => TRUE,
  );
 */

  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#default_value' => isset($candidate->mail)? $candidate->mail : NULL,
    '#size' => 30,
    '#required' => TRUE,
  );

  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact telephone number'),
    '#description' => t('Please enter a valid phone number so that we can contact you about your nomination.'),
    '#default_value' => isset($candidate->phone)? $candidate->phone : NULL,
    '#size' => 12,
    '#required' => TRUE,
  );

  $form['proposer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proposer'),
    '#collapsible' => TRUE,
  );
  $form['proposer']['proposer_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($candidate->proposer->name)? $candidate->proposer->name : NULL,
    '#size' => 50,
  );
  $form['proposer']['proposer_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#default_value' => isset($candidate->proposer->mail)? $candidate->proposer->mail : NULL,
    '#size' => 30,
    '#element_validate' => array('election_nomination_validate_email'),
  );

  $form['seconders'] = array(
    '#type' => 'fieldset',
    '#title' => t('Seconders'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($form_state['seconder_expand'])? TRUE : FALSE,
  );
  for ($i = 0; $i < $form_state['seconder_count']; $i++) {
    $form['seconders']['seconder_' . $i] = array(
      '#type' => 'fieldset',
      '#title' => t('Seconder !number', array('!number' => $i + 1)),
    );
    $form['seconders']['seconder_' . $i]['seconder_' . $i . '_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => isset($candidate->seconders[$i]->name)? $candidate->seconders[$i]->name : NULL,
      '#size' => 50,
    );
    $form['seconders']['seconder_' . $i]['seconder_' . $i . '_mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Email address'),
      '#default_value' => isset($candidate->seconders[$i]->mail)? $candidate->seconders[$i]->mail : NULL,
      '#size' => 30,
      '#element_validate' => array('election_nomination_validate_email'),
    );
  }
  $form['seconders']['seconder_addnew'] = array(
    '#type' => 'submit',
    '#value' => t('Add another seconder'),
    '#submit' => array('election_nomination_form_add_seconder'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'election_nomination_form_add_seconder_js',
      'wrapper' => 'edit-seconder-addnew',
      'method' => 'before',
      'effect' => 'slide',
    ),
  );

  // Add custom fields from the Field system.
  field_attach_form('election_candidate', $candidate, $form, $form_state);

  $form['#validate'][] = 'election_nomination_form_validate';

  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit your nomination'),
    '#submit' => array('election_nomination_form_submit'),
  );

  return $form;
}

/**
 * Form submit function for the 'Add another seconder' button in the form
 * election_nomination_form.
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_nomination_form_add_seconder($form, &$form_state) {

  if ($form_state['seconder_count'] < ELECTION_CANDIDATE_MAX_SECONDERS) {
    $form_state['seconder_expand'] = TRUE;
    $form_state['seconder_count']++;
    $form_state['rebuild'] = TRUE;
  }

}

/**
 * AJAX callback function for the 'Add another seconder' button in the form
 * election_nomination_form.
 *
 * @param array $form
 * @param array &$form_state
 * @return array
 */
function election_nomination_form_add_seconder_js($form, &$form_state) {

  if ($form_state['rebuild']) {
    $form = drupal_rebuild_form('election_nomination_form', $form_state, $form);
    return $form['seconders']['seconder_' . ($form_state['seconder_count'] -1)];
  }

  return NULL;

}

function election_nomination_validate_email($element, &$form_state, $form) {
  if (!empty($element['#value']) && !filter_var($element['#value'], FILTER_VALIDATE_EMAIL)) {
    form_error($element, t('Please enter a valid email address'));
  }
}

function election_nomination_form_validate($form, &$form_state) {

  // Run nomination form checks here.
  if (!preg_match('/\b\+?([0-9]+[\s-\+]?)+\b/', $form_state['values']['phone'])) {
    form_set_error('phone', t('Please enter a valid phone number.'));
  }

  // Check other Fields.
  field_attach_form_validate('election_candidate', $form_state['candidate'], $form, $form_state);

}

function election_nomination_form_submit($form, &$form_state) {

  $candidate = &$form_state['candidate'];

  // election object won't be changed, no need to pass by reference
  $election = $form_state['election'];

  $values = $form_state['values'];
  $post = election_post_load($values['post']);

  $candidate->first_name = $values['first_name'];
  $candidate->last_name = $values['last_name'];
  $candidate->mail = $values['mail'];
  $candidate->post_id = $post->post_id;
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

  $post_name = _election_get_posts_name($election->type);

  // Notify the user that the post was saved.
  drupal_set_message(
    t('Your nomination for the @posts_name %title was successfully submitted.')
  );

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_uri_path($election);
  }

}
