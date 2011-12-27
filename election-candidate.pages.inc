<?php
/**
 * @file
 * Page callbacks for election candidates and nominations (Election module).
 */

function election_page_nomination($election) {

  $title = t('Candidate nomination form');
  drupal_set_breadcrumb(_election_build_breadcrumb($election));
  drupal_set_title($title);

  if ($election->type == 'referendum') {
    return MENU_NOT_FOUND;
  }

  $nopen = _election_is_open($election->nstatus, $election->nopen_time, $election->nclose_time);
  if (!$nopen && !user_access('bypass nomination access control')) {
    drupal_set_message(t('Nominations are currently closed.'), 'error', FALSE);
    return MENU_ACCESS_DENIED;
  }

  if ($nominations = election_candidate_user_nominations($election)) {
    foreach ($nominations as $nomination) {
      drupal_set_message(
        t(
          'You have already submitted a nomination for this election (name %name, position %position).',
          array(
            '%name' => $nomination->first_name . ' ' . $nomination->last_name,
            '%position' => $nomination->post_title,
          )
        ),
        'warning',
        FALSE
      );
    }
  }

  $post_ids = election_candidate_available_posts($election, $nominations);
  if (empty($post_ids)) {
    return array(
      '#markup' => t('There are no more positions available for nomination in this election.'),
    );
  }

  $posts = election_post_load_multiple($post_ids);

  $candidate = entity_get_controller('election_candidate')->create();

  return drupal_get_form('election_nomination_form', $election, $candidate, $posts);

}

/*
 * Election nominations form function. This creates election_candidate entities.
 */
function election_nomination_form($form, &$form_state, $election, $candidate, array $posts) {

  $candidate->is_new = empty($candidate->candidate_id);

  $form['#id'] = 'election-nomination-form';

  if (!isset($form_state['seconder_count'])) {
    $form_state['seconder_count'] = 1;
  }

  $form_state['candidate'] = $candidate;
  $form_state['election'] = $election;

  $form['name']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name(s)'),
    '#default_value' => isset($candidate->first_name)? $candidate->first_name : NULL,
    '#max_length' => 100,
    '#size' => 50,
    '#required' => TRUE,
  );

  $form['name']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name(s)'),
    '#default_value' => isset($candidate->last_name)? $candidate->last_name : NULL,
    '#max_length' => 100,
    '#size' => 50,
    '#required' => TRUE,
  );

  $bypass = user_access('bypass nomination access control');
  $post_options = array();
  foreach ($posts as $post) {
    if ($bypass || $post->nstatus_inheritance == ELECTION_POST_STATUS_INHERIT) {
      $post_options[$post->post_id] = $post->title;
    }
  }

  $posts_name = _election_get_posts_name($election->type);
  $form['post'] = array(
    '#type' => 'select',
    '#title' => check_plain(drupal_ucfirst($posts_name)),
    '#description' => t('Select the @posts_name you wish to stand for.', array('@posts_name' => $posts_name)),
    '#default_value' => isset($candidate->post_id)? $candidate->post_id : NULL,
    '#options' => $post_options,
    '#empty_option' => t('- Select @posts_name -', array('@posts_name' => $posts_name)),
    '#required' => TRUE,
  );

  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#default_value' => isset($candidate->mail)? $candidate->mail : NULL,
    '#size' => 30,
    '#required' => TRUE,
    '#element_validate' => array('election_nomination_validate_email'),
  );

  $form['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact telephone number'),
    '#description' => t('Please enter a valid phone number so that we can contact you about your nomination.'),
    '#default_value' => isset($candidate->phone)? $candidate->phone : NULL,
    '#size' => 12,
    '#required' => TRUE,
    '#element_validate' => array('election_nomination_validate_phone'),
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

function election_nomination_validate_email($element, &$form_state, $form) {
  if (!empty($element['#value']) && !filter_var($element['#value'], FILTER_VALIDATE_EMAIL)) {
    form_error($element, t('Please enter a valid email address'));
  }
}

function election_nomination_validate_phone($element, &$form_state, $form) {
  if (!empty($element['#value']) && !preg_match('/\b\+?([0-9]+[\s-\+]?)+\b/', $element['#value'])) {
    form_error($element, t('Please enter a valid phone number.'));
  }
}

function election_nomination_form_validate($form, &$form_state) {

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
