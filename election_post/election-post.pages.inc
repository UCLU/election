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
      $view = views_embed_view('election_posts', 'block_referendum', $election->election_id);
      break;
    case 'stv':
      $view = views_embed_view('election_posts', 'block_stv', $election->election_id);
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

  if (isset($form_state['vote_require_confirm'])) {
    return election_post_vote_form_vote_confirm($form, $form_state);
  }

  if (isset($form_state['abstain_require_confirm'])) {
    return election_post_vote_form_abstain_confirm($form, $form_state);
  }

  $election = election_load($post->election_id);

  if ($previously_voted = election_vote_check_user_previously_voted($post)) {
    drupal_set_message(t('You have already voted for (or abstained from) this @post_name.', array('@post_name' => _election_get_posts_name($election->type))), 'status', FALSE);
    $form['#disabled'] = TRUE;
    drupal_goto(election_post_uri_path($post));
  }

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
    '#value' => t('Vote'),
    '#submit' => array('election_post_vote_form_vote'),
  );
  $form['buttons']['abstain'] = array(
    '#type' => 'submit',
    '#value' => t('Abstain'),
    '#submit' => array('election_post_vote_form_abstain'),
    '#limit_validation_errors' => array(),
  );

  $form['#validate'][] = 'election_post_vote_form_validate';

  $type = _election_type_get_info($election->type);
  $post_key = $type['post key'];
  $post_vote_form_function = 'election_post_' . $post_key . '_vote_form';
  if (function_exists($post_vote_form_function)) {
    $form = $post_vote_form_function($form, $form_state, $post, $election);
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

  $form['motion_preview'] = array(
    '#type' => 'item',
    '#title' => t('Motion'),
    '#markup' => check_plain($post->title),
  );

  $form['answer'] = array(
    '#type' => 'select',
    '#title' => t('Your answer'),
    '#options' => $answer_options,
    '#required' => TRUE,
  );

  if (empty($post->allow_abstention)) {
    unset($form['buttons']['abstain']);
  }

  return $form;

}

/**
 * Form modifying function for election-post/%election_post/vote
 */
function election_post_position_vote_form($form, &$form_state, $post, $election) {

  // Load candidates
  // @todo reconcile all this without mutual dependency between election_candidate and election_post, probably via another submodule.
  $candidates = election_candidate_load_by_post($post);
  $form_state['candidates'] = $candidates;

  $num_candidates = count($candidates);
  $form_state['num_candidates'] = $num_candidates;

  if ($num_candidates == 0) {
    drupal_set_message(t('There are no approved candidates standing for this @post_name.', array('@post_name' => _election_get_posts_name($election->type))), 'warning', FALSE);
    $form['#disabled'] = TRUE;
    drupal_goto(election_post_uri_path($post));
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
      'The following candidates are standing for the @post_name <a href="@post_url">%post_title</a> (!vacancies).',
      array(
        '@post_name' => $post_name,
        '%post_title' => $post->title,
        '@post_url' => url(election_post_uri_path($post)),
        '!vacancies' => format_plural($post->vacancy_count, '1 vacancy', '@count vacancies'),
      )
    ),
  );

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

  // Last minute check to see whether a vote has already been entered.
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
  $someone_ranked = FALSE;
  foreach ($form_state['values'] as $key => $value) {
    if (substr($key, 0, 10) == 'candidate_') {
      $candidate_id = substr($key, 10);
      $candidate_rankings[$candidate_id] = $value;
      if (isset($had[$value])) {
        form_set_error('ballot_paper', t('You have used the preference level %value more than once. Candidates cannot be ranked equally.', array('%value' => $value)));
        $form['rebuild'] = TRUE;
        break;
      }
      if ($value != 'NONE' && $value !== NULL) {
        $someone_ranked = TRUE;
        $had[$value] = $key;
      }
    }
  }

  if (!$someone_ranked) {
    form_set_error('', t('You must rank at least one candidate.'));
  }

  // Force the ranks to be consecutive, starting with 1.
  $i = 1;
  asort($candidate_rankings);
  foreach ($candidate_rankings as &$rank) {
    if ($rank != 'NONE') {
      $rank = $i++;
    }
  }

  $form_state['candidate_rankings'] = $candidate_rankings;

}

/**
 * Submit callback for the 'Vote' button on the vote form.
 */
function election_post_vote_form_vote_confirm($form, &$form_state) {

  $form['vote_confirmed'] = array(
    '#type' => 'hidden',
    '#value' => TRUE,
  );

  $form['#submit'] = array('election_post_vote_form_vote');

  $post = $form_state['post'];

  return confirm_form(
    $form,
    t('Are you sure you want to cast this vote?'),
    $_GET['q'],
    theme('election_vote_confirm', array('form_state' => $form_state)),
    t('Confirm'),
    t('Cancel')
  );

}

/**
 * Theme a confirmation page for a vote.
 */
function theme_election_vote_confirm($variables) {
  $form_state = $variables['form_state'];
  $post = $form_state['post'];

  // @todo split this sensibly

  if ($post->type == 'position') {

    $header = array(t('Candidate'), t('Preference'));
    foreach ($form_state['candidate_rankings'] as $candidate_id => $rank) {
      if ($candidate_id == 'RON') {
        $candidate_item = t('RON (Re-Open Nominations)');
      }
      else {
        $candidate = $form_state['candidates'][$candidate_id];
        $candidate_item = theme('election_candidate_ballot_item', array('candidate' => $candidate));
      }
      $rows[] = array(
        $candidate_item,
        $rank == 'NONE'? t('No preference') : _election_vote_get_ordinal($rank)
      );
    }
    return theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'empty' => 'No ranking',
    ));

  }
  elseif ($post->type == 'motion') {

    switch ($form_state['values']['answer']) {
      case ELECTION_ANSWER_YES:
        $answer = t('Yes');
        break;
      case ELECTION_ANSWER_NO:
        $answer = t('No');
        break;
    }

    $answer = t('Answer: !answer', array('!answer' => $answer));

    return '<dl><dt>' . check_plain($post->title) . '</dt><dd>' . $answer . '</dd></dl>';

  }

}

/**
 * Confirm the 'Abstain' button on the vote form.
 */
function election_post_vote_form_abstain_confirm($form, &$form_state) {

  $form['abstain_confirmed'] = array(
    '#type' => 'hidden',
    '#value' => TRUE,
  );

  $form['#submit'] = array('election_post_vote_form_abstain');

  return confirm_form($form, t('Are you sure you want to abstain?'), $_GET['q']);

}


/**
 * Submit callback for the 'Abstain' button on the vote form.
 */
function election_post_vote_form_abstain($form, &$form_state) {

  if (empty($form_state['values']['abstain_confirmed'])) {
    $form_state['abstain_require_confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
    return;
  }

  $post = $form_state['post'];
  $election = $form_state['election'];

  $success = FALSE;

  $transaction = db_transaction();

  try {

    global $user;

    $vote = array(
      'post_id' => $post->post_id,
      'uid' => $user->uid,
      'ip' => ip_address(),
      'agent' => $_SERVER['HTTP_USER_AGENT'],
      'sid_hash' => _election_vote_session_hash(),
      'vote_time' => REQUEST_TIME,
    );
    $vote_id = db_insert('election_vote')
      ->fields($vote)
      ->execute();
    if (empty($vote_id)) {
      throw new Exception('The vote record could not be saved.');
    }

    $ballot_entry = array(
      'vote_id' => $vote_id,
      'post_id' => $post->post_id,
      'abstain' => 1,
    );
    $success = db_insert('election_ballot')
      ->fields($ballot_entry)
      ->execute();

  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('election_post', $e, NULL, WATCHDOG_ERROR);
  }

  if ($success) {
    $post_name = _election_get_posts_name($election->type);
    $message = 'You have abstained from voting for the @post_name !post_link.';
    $message = t($message, array('@post_name' => $post_name, '!post_link' => l($post->title, election_post_uri_path($post))));
    drupal_set_message($message);
    $form_state['redirect'] = url(election_uri_path($election));
  }
  else {
    $form_state['rebuild'] = TRUE;
  }

}

/**
 * Submit callback for the 'Vote' button on the form election_post_vote_form_confirm.
 */
function election_post_vote_form_vote($form, &$form_state) {

  if (empty($form_state['values']['vote_confirmed'])) {
    $form_state['vote_require_confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
    return;
  }

  $post = $form_state['post'];
  $election = $form_state['election'];

  $success = FALSE;

  $transaction = db_transaction();

  try {

    global $user;

    $vote = array(
      'post_id' => $post->post_id,
      'uid' => $user->uid,
      'ip' => ip_address(),
      'agent' => $_SERVER['HTTP_USER_AGENT'],
      'sid_hash' => _election_vote_session_hash(),
      'vote_time' => REQUEST_TIME,
    );
    $vote_id = db_insert('election_vote')
      ->fields($vote)
      ->execute();
    if (empty($vote_id)) {
      throw new Exception('The vote record could not be saved.');
    }

    // Call election_post_POST_TYPE_save_votes() callback.
    $function = 'election_post_' . $post->type . '_save_votes';
    if (function_exists($function)) {
      $success = $function($form, $form_state, $post, $election, $vote_id);
    }

  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('election_post', $e, NULL, WATCHDOG_ERROR);
  }

  if ($success) {
    $post_name = _election_get_posts_name($election->type);
    $message = 'Thank you for voting for the @post_name !post_link.';
    $message = t($message, array('@post_name' => $post_name, '!post_link' => l($post->title, election_post_uri_path($post))));
    drupal_set_message($message);
    $form_state['redirect'] = url(election_uri_path($election));
  }
  else {
    $form_state['rebuild'] = TRUE;
  }

}

/**
 * Save votes for a position.
 */
function election_post_position_save_votes($form, &$form_state, $post, $election, $vote_id) {

  global $user;

  $candidate_rankings = $form_state['candidate_rankings'];

  foreach ($candidate_rankings as $cid => $candidate_ranking) {

    if ($candidate_ranking == 'NONE') {
      // Don't save anything if the user did not rank this candidate.
      continue;
    }

    $ballot_entry = array(
      'vote_id' => $vote_id,
      'post_id' => $post->post_id,
      'rank' => $candidate_ranking,
    );

    if ($cid == 'RON') {
      $ballot_entry['ron'] = 1;
    }
    else {
      $ballot_entry['candidate_id'] = $cid;
    }

    db_insert('election_ballot')
      ->fields($ballot_entry)
      ->execute();

  }

  return TRUE;

}

/**
 * Save answer to a motion.
 */
function election_post_motion_save_votes($form, &$form_state, $post, $election, $vote_id) {

  $ballot_entry = array(
    'vote_id' => $vote_id,
    'post_id' => $post->post_id,
    'answer' => $form_state['values']['answer'],
  );

  db_insert('election_ballot')
    ->fields($ballot_entry)
    ->execute();

  return TRUE;

}
