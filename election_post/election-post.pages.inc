<?php
/**
 * @file
 * Page callbacks for the Election Post module.
 *
 * @todo split most of this into an election_vote submodule
 */

/**
 * Page callback for election/%election/posts/list
 */
function election_post_list_page(stdClass $election) {

  $posts_name = _election_get_posts_name($election->type, TRUE);

  switch($election->type) {
    case 'referendum':
      $view = views_embed_view('election_posts', 'page_referendum', $election->election_id);
      break;
    case 'stv':
      $view = views_embed_view('election_posts', 'page_stv', $election->election_id);
      break;
    default:
      $view = views_embed_view('election_posts', 'default', $election->election_id);
      break;
  }

  drupal_set_title(drupal_ucfirst($posts_name), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  return array(
    '#markup' => $view,
  );

}

/**
 * Form builder function for election-post/%election_post/vote
 *
 * @param array $form
 * @param array &$form_state
 *
 * @return array $form
 */
function election_post_vote_form($form, &$form_state, $post) {

  $election = election_load($post->election_id);

  $form_state['election'] = $election;
  $form_state['post'] = $post;

  $title = election_post_page_title($post);
  drupal_set_title(t('Vote: %title_trim', array('%title_trim' => truncate_utf8($post->title, 20, TRUE, TRUE))), PASS_THROUGH);
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, l($post->title, election_post_uri_path($post)))
  );

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit vote'),
    '#submit' => array('election_post_vote_form_submit'),
  );
  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('election_post_vote_form_cancel_submit'),
    '#limit_validation_errors' => array(),
  );

  $form['#validate'][] = 'election_post_vote_form_validate';

  $type = _election_type_get_info($election->type);
  $post_key = $type['post key'];
  $post_vote_form_function = 'election_post_' . $post_key . '_vote_form';
  if (function_exists($post_vote_form_function)) {
    $form = $post_vote_form_function($form, $form_state, $post, $election);
  }

  if ($previously_voted = election_vote_check_user_previously_voted($post)) {
    drupal_set_message(t('You have already submitted a vote for this @post_name.', array('@post_name' => _election_get_posts_name($election->type))));
    $form['ballot_paper']['#disabled'] = TRUE;
    $form['buttons']['#disabled'] = TRUE;
  }

  return $form;

}


/**
 * Form modifying function for election-post/%election_post/vote
 */
function election_post_motion_vote_form($form, &$form_state, $post, $election) {

  $answer_options = array(
    ELECTION_ANSWER_YES => t('Yes'),
    ELECTION_ANSWER_NO => t('No'),
  );

  if (!empty($post->allow_abstention)) {
    $answer_options[ELECTION_ANSWER_ABSTAIN] = t('Abstain');
  }

  $form['motion_preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Motion'),
  );

  $form['motion_preview']['motion'] = election_post_view($election, $post, 'teaser');

  $form['answer'] = array(
    '#type' => 'select',
    '#title' => t('Answer'),
    '#description' => t('Please select your answer to the motion.'),
    '#options' => $answer_options,
    '#required' => TRUE,
  );

  return $form;

}

/**
 * Form modifying function for election-post/%election_post/vote
 */
function election_post_position_vote_form($form, &$form_state, $post, $election) {

  // Load candidates
  // @todo reconcile all this without mutual dependency between election_candidate and election_post, probably via another submodule.
  $candidates = election_candidate_load_by_post($post);

  $num_candidates = count($candidates);
  $form_state['num_candidates'] = $num_candidates;

  if ($num_candidates == 0) {
    drupal_set_message(t('There are no approved candidates standing for this @post_name.', array('@post_name' => _election_get_posts_name($election->type))), 'warning', FALSE);
    unset($form['buttons']);
    $form['ballot_paper']['#disabled'] = TRUE;
    return $form;
  }

  // For each candidate the user should be able to select a 'No preference' option instead of a rank.
  $rank_options = array();

  // Find all the other possible preference levels (number of (candidates + ron)).
  $pref_limit = $num_candidates;
  if ($post->use_ron) {
    $pref_limit++;
  }
  for ($i = 0; $i < $pref_limit; $i++) {
    $rank_options[$i + 1] = _election_vote_get_ordinal($i + 1);
  }

  $rank_options['NONE'] = t('No preference');

  $post_name = _election_get_posts_name($election->type);

  $form['post_help'] = array('#type' => 'container');

  $form['post_help']['nums'] = array(
    '#markup' => t(
      'The following !candidates_are standing for the @post_name <a href="@post_url">%post_title</a> (!vacancies).',
      array(
        '!candidates_are' => format_plural($num_candidates, 'candidate is', '@count candidates are'),
        '@post_name' => $post_name,
        '%post_title' => $post->title,
        '@post_url' => url(election_post_uri_path($post)),
        '!vacancies' => format_plural($post->vacancy_count, '1 vacancy', '@count vacancies'),
      )
    ),
  );

  if ($post->use_ron) {
    $form['post_help']['ron'] = array(
      '#markup' => ' ' . t('RON (Re-Open Nominations) is also an option for this @post_name.', array('@post_name' => $post_name)),
    );
  }

  $form['ballot_paper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Your ballot paper'),
  );

  $form['ballot_paper']['help'] = array(
    '#type' => 'item',
    '#markup' => t('Please rank the candidates according to your preference.'),
  );

  $form['ballot_paper']['start'] = array(
    '#markup' => '<table class="election-ballot-table"><tr><th>' . t('Name') . '</th><th>' . t('Preference') . '</th></tr>',
  );

  foreach ($candidates as $candidate) {
    $form['ballot_paper']['candidate_' . $candidate->candidate_id] = array(
      '#type' => 'select',
      '#prefix' => '<tr><td>'
        . theme('election_candidate_ballot_item', array('candidate' => $candidate))
        . '</td><td>',
      '#suffix' => '</td></tr>',
      '#title' => t('Your rank for %candidate', array('%candidate' => $candidate->first_name . ' ' . $candidate->last_name)),
      '#title_display' => 'invisible',
      '#options' => $rank_options,
      '#required' => TRUE,
    );
  }

  if ($post->use_ron) {
    $form['ballot_paper']['candidate_RON'] = array(
      '#type' => 'select',
      '#prefix' => '<tr><td>' . t('RON (Re-Open Nominations)') . '</td><td>',
      '#suffix' => '</td></tr>',
      '#title' => t('Your rank for %candidate', array('%candidate' => t('RON (Re-Open Nominations)'))),
      '#title_display' => 'invisible',
      '#options' => $rank_options,
      '#required' => TRUE,
    );
  }

  $form['ballot_paper']['end'] = array(
    '#markup' => '</table>',
  );

  $form['#attached']['js'][] = drupal_get_path('module', 'election_post') . '/vote.js';
  $form['#validate'][] = 'election_post_position_vote_form_validate';

  return $form;

}

/**
 * Validate the voting form (general).
 *
 * @param array $form
 * @param array &$form_state
 *
 * @return void
 */
function election_post_vote_form_validate($form, &$form_state) {

  $post = $form_state['post'];

  if (election_vote_check_user_previously_voted($post)) {
    $form_state['rebuild'] = TRUE;
  }

}

/**
 * Validate the voting form (positions only).
 *
 * @param array $form
 * @param array &$form_state
 *
 * @return void
 */
function election_post_position_vote_form_validate($form, &$form_state) {

  $post = $form_state['post'];
  $num_candidates = $form_state['num_candidates'];

  $candidate_rankings = array();
  $had = array();
  foreach ($form_state['values'] as $key => $value) {
    if ($value != 'NONE' && substr($key, 0, 10) == 'candidate_') {
      $candidate_id = substr($key, 10);
      $candidate_rankings[$candidate_id] = $value;
      if (in_array($value, $had)) {
        form_set_error($key, t('You have used the same preference level more than once. Candidates cannot be ranked equally.'));
        $form['rebuild'] = TRUE;
        break;
      }
      $had[] = $value;
    }
  }

  if (!count($candidate_rankings)) {
    form_set_error('', t('You must rank at least one candidate.'));
  }

  // Get the actual rankings.
  asort($candidate_rankings);

  $form_state['candidate_rankings'] = $candidate_rankings;

}

/**
 * Submit callback for the vote form.
 */
function election_post_vote_form_submit($form, &$form_state) {

  $post = $form_state['post'];
  $election = $form_state['election'];

  $success = FALSE;

  $transaction = db_transaction();

  try {

    $function = 'election_post_' . $post->type . '_save_votes';
    if (function_exists($function)) {
      $success = $function($form, $form_state, $post, $election);
    }

  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('election_post', $e, NULL, WATCHDOG_ERROR);
  }

  if ($success) {
    $post_name = _election_get_posts_name($election->type);
    drupal_set_message(t('Thank you for submitting a vote for the @post_name !post_link.', array('@post_name' => $post_name, '!post_link' => l($post->title, election_post_uri_path($post)))));
    $form_state['redirect'] = url(election_uri_path($election));
  }
  else {
    $form_state['rebuild'] = TRUE;
  }

}

/**
 * Save votes for a position.
 */
function election_post_position_save_votes($form, &$form_state, $post, $election) {

  global $user;

  $candidate_rankings = $form_state['candidate_rankings'];

  $vote = array(
    'post_id' => $post->post_id,
    'uid' => $user->uid,
    'ip' => ip_address(),
    'agent' => $_SERVER['HTTP_USER_AGENT'],
    'sid_hash' => _election_vote_session_hash(),
  );

  $insert_id = db_insert('election_vote')
    ->fields($vote)
    ->execute();
  if (empty($insert_id)) {
    throw new Exception('election_vote record could not be saved');
  }

  foreach ($candidate_rankings as $cid => $candidate_ranking) {

    $ballot_entry = array(
      'vote_id' => $insert_id,
      'post_id' => $post->post_id,
      'candidate_id' => $cid == 'RON'? NULL : $cid,
      'ron' => $cid == 'RON'? 1 : 0,
      'rank' => $candidate_ranking,
    );
    db_insert('election_ballot')
      ->fields($ballot_entry)
      ->execute();

  }

  return TRUE;

}

/**
 * Save answer to a motion.
 */
function election_post_motion_save_votes($form, &$form_state, $post, $election) {

  global $user;

  $vote = array(
    'post_id' => $post->post_id,
    'uid' => $user->uid,
    'ip' => ip_address(),
    'agent' => $_SERVER['HTTP_USER_AGENT'],
    'sid_hash' => _election_vote_session_hash(),
  );

  $insert_id = db_insert('election_vote')
    ->fields($vote)
    ->execute();
  if (empty($insert_id)) {
    throw new Exception('election_vote record could not be saved');
  }

  $ballot_entry = array(
    'vote_id' => $insert_id,
    'post_id' => $post->post_id,
    'answer' => $form_state['values']['answer'],
  );

  db_insert('election_ballot')
    ->fields($ballot_entry)
    ->execute();

  return TRUE;

}


/**
 * Cancel the vote form.
 */
function election_post_vote_form_cancel_submit($form, &$form_state) {
  $post = $form_state['post'];
  $form_state['redirect'] = url(election_post_uri_path($post));
}

