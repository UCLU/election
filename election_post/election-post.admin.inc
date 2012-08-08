<?php
/**
 * @file
 * Administration page callbacks for the Election Post module.
 */

/**
 * Form builder function for adding or editing an election post.
 *
 * @param array $form
 *   The Drupal Form API form definition.
 * @param array &$form_state
 *   The form state array.
 * @param stdClass $post
 *   An election post object.
 * @param stdClass $election
 *   Optional: the election the post belongs to.
 *
 * @return array
 *   Form API render array.
 */
function election_post_form($form, &$form_state, stdClass $post, $election = NULL) {

  if (empty($election)) {
    $election = election_load($post->election_id);
  }

  $post->is_new = empty($post->post_id);

  if ($post->is_new) {
    $breadcrumb = _election_build_breadcrumb(
      $election,
      '_MANAGE_POSTS_'
    );
  }
  else {
    $breadcrumb = _election_build_breadcrumb(
      $election,
      '_MANAGE_POSTS_',
      l($post->title, election_post_uri_path($post))
    );
  }

  drupal_set_breadcrumb($breadcrumb);

  $post_name = _election_get_posts_name($election->type);

  // Warn if voting is open.
  if (!$post->is_new && _election_post_is_open($post, $election, 'v')) {
    drupal_set_message(t('Voting is currently open: you should not make changes to this @post_name!', array('@post_name' => $post_name)), 'warning');
  }

  $form['#id'] = 'election-post-form';
  $form_state['post'] = $post;
  $form_state['election'] = $election;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($post->title) ? $post->title : '',
    '#required' => TRUE,
    '#weight' => -100,
  );

  $form['role_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role eligibility'),
    '#collapsible' => TRUE,
    '#weight' => 10,
  );
  $roles_that_can_vote_global = user_roles(FALSE, 'vote in elections');
  $form['role_access']['rids'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles eligible to vote'),
    '#options' => $roles_that_can_vote_global,
    '#description' => t("Select the roles eligible to vote in this election. There may be other restrictions. Only roles with the permission 'vote in elections' are listed here."),
    '#default_value' => (array) $post->rids,
  );

  if ($post->is_new) {
    $form['role_access']['rids']['#default_value'] = array_keys($roles_that_can_vote_global);
  }

  $form['voting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Voting'),
    '#collapsible' => TRUE,
    '#weight' => 98,
  );
  $form['voting']['vstatus_inheritance'] = array(
    '#type' => 'radios',
    '#title' => t('Voting status'),
    '#default_value' => isset($post->vstatus_inheritance) ? $post->vstatus_inheritance : ELECTION_POST_STATUS_INHERIT,
    '#options' => array(
      ELECTION_POST_STATUS_INHERIT => t('Inherited (currently: %status)', array('%status' => strip_tags(_election_format_status($election, 'v')))),
      ELECTION_POST_STATUS_CLOSED => t('Closed'),
    ),
  );
  $form['voting']['allow_abstention'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow abstention'),
    '#default_value' => isset($post->allow_abstention) ? $post->allow_abstention : 1,
    '#description' => t('This enables a voter to abstain instead of entering a vote. Abstentions can be counted.'),
  );

  $form['buttons'] = array(
    '#type' => 'actions',
    '#weight' => 100,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('election_post_form_submit'),
  );
  if (!$post->is_new) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('election_post_form_delete_submit'),
      '#access' => election_post_access('delete', $post),
    );
  }

  $form['#validate'][] = 'election_post_form_validate';

  // Modify the form with the function $type['post settings form'], if it exists.
  $type = $election->type_info;
  if (!empty($type['post settings form']) && function_exists($type['post settings form'])) {
    $settings_form_function = $type['post settings form'];
    $form = $settings_form_function($form, $form_state, $post, $election);
  }

  // Add custom fields from the Field system.
  field_attach_form('election_post', $post, $form, $form_state);

  return $form;

}


/**
 * Settings form for motions in referendums.
 *
 * This adds to the form 'election_post_form'.
 *
 * @see election_post_form()
 *
 * @param array $form
 *   The Drupal Form API form definition.
 * @param array &$form_state
 *   The form state array.
 * @param stdClass $post
 *   An election post object.
 * @param stdClass $election
 *   The election the post belongs to.
 *
 * @return array
 *   Form API render array.
 */
function election_post_referendum_settings_form($form, &$form_state, stdClass $post, stdClass $election) {
  $form['answers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Answers'),
    '#description' => t('Define the valid positive and negative answers to this motion (e.g. Yes/No, Agree/Disagree, etc.).'),
    '#weight' => -30,
    '#collapsible' => TRUE,
  );
  $form['answers']['settings_answer_pos'] = array(
    '#type' => 'textfield',
    '#title' => t('Positive answer'),
    // The default value is sanitized by the Form API so no check_plain() needed here.
    '#default_value' => _election_post_get_answer_text($post, ELECTION_ANSWER_YES),
    '#maxlength' => 10,
    '#size' => 10,
  );
  $form['answers']['settings_answer_neg'] = array(
    '#type' => 'textfield',
    '#title' => t('Negative answer'),
    // The default value is sanitized by the Form API so no check_plain() needed here.
    '#default_value' => _election_post_get_answer_text($post, ELECTION_ANSWER_NO),
    '#maxlength' => 10,
    '#size' => 10,
  );
  $form['#validate'][] = 'election_post_referendum_settings_form_validate';
  return $form;
}

/**
 * Settings form for positions in STV elections.
 *
 * This adds to the form 'election_post_form'.
 *
 * @see election_post_form()
 *
 * @param array $form
 *   The Drupal Form API form definition.
 * @param array &$form_state
 *   The form state array.
 * @param stdClass $post
 *   An election post object.
 * @param stdClass $election
 *   The election the post belongs to.
 *
 * @return array
 *   Form API render array.
 */
function election_post_stv_settings_form($form, &$form_state, stdClass $post, stdClass $election) {

  $form['settings_exclusive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclusive'),
    '#description' => t('A candidate can only stand for one exclusive position per election.<br /><strong>N.B. if anonymous users are given permission to submit nominations, exclusivity cannot be enforced.</strong>'),
    '#default_value' => isset($post->settings['exclusive']) ? $post->settings['exclusive'] : 1,
    '#weight' => -30,
  );

  $form['settings_vacancy_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of vacancies'),
    '#default_value' => isset($post->settings['vacancy_count']) ? $post->settings['vacancy_count'] : 1,
    '#size' => 2,
    '#maxlength' => 2,
    '#required' => TRUE,
    '#weight' => -20,
  );

  $form['settings_use_ron'] = array(
    '#type' => 'select',
    '#title' => t('RON (Re-Open Nominations)'),
    '#default_value' => isset($post->settings['use_ron']) ? $post->settings['use_ron'] : 1,
    '#options' => array(
      1 => t('Include RON as a candidate'),
      0 => t('Do not include RON'),
    ),
    '#weight' => -10,
  );

  $form['voting']['settings_allow_equal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow equal rankings'),
    '#default_value' => isset($post->settings['allow_equal']) ? $post->settings['allow_equal'] : 0,
    '#description' => t('Used (rarely) in STV elections, this allows the voter to select more than one candidate as the same preference.'),
  );

  // @todo Implement an election-level settings form, to allow RON in multi-vacancy positions.
  if (!variable_get('election_allow_multi_vacancy_ron', TRUE)) {
    $form['use_ron']['#states'] = array(
      'visible' => array(
        ':input[name=settings_vacancy_count]' => array('value' => '1'),
      ),
    );
    $form['use_ron']['#description'] = t('RON can only be included for single-vacancy positions.');
  }

  $form['nominations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nominations'),
    '#collapsible' => TRUE,
    '#weight' => 97,
  );

  $form['nominations']['nstatus_inheritance'] = array(
    '#type' => 'radios',
    '#title' => t('Nominations status'),
    '#default_value' => isset($post->nstatus_inheritance) ? $post->nstatus_inheritance : ELECTION_POST_STATUS_INHERIT,
    '#options' => array(
      ELECTION_POST_STATUS_INHERIT => t('Inherited (currently: %status)', array('%status' => strip_tags(_election_format_status($election, 'n')))),
      ELECTION_POST_STATUS_CLOSED => t('Closed'),
    ),
  );

  $form['#validate'][] = 'election_post_stv_settings_form_validate';

  return $form;
}

/**
 * Form submit function for the Delete button in the form election_post_form.
 */
function election_post_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $post = $form_state['post'];
  $election = $form_state['election'];
  $form_state['redirect'] = array(
    election_post_uri_path($post) . '/delete',
    array('query' => $destination),
  );
}


/**
 * Form validate function for the form election_post_form.
 */
function election_post_form_validate($form, &$form_state) {

  field_attach_form_validate('election_post', $form_state['post'], $form, $form_state);

}

/**
 * Form validate function for the referendum motion settings form.
 *
 * @see election_post_referendum_settings_form()
 */
function election_post_referendum_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['settings_answer_pos'] == $form_state['values']['settings_answer_neg']) {
    form_set_error('answers', t('The positive and negative answers cannot be the same.'));
  }
}

/**
 * Form validate function for the STV position settings form.
 *
 * @see election_post_stv_settings_form()
 */
function election_post_stv_settings_form_validate($form, &$form_state) {

  $post = $form_state['post'];
  if ($form_state['values']['settings_use_ron'] == 1 && $form_state['values']['settings_vacancy_count'] > 1 && !variable_get('election_allow_multi_vacancy_ron', TRUE)) {
    form_set_error('use_ron', t('RON can only be included for single-vacancy positions.'));
  }

}

/**
 * Form submit function for the Save button in the form election_post_form.
 */
function election_post_form_submit($form, &$form_state) {

  $post = $form_state['post'];
  $election = $form_state['election'];

  if (empty($post->uid)) {
    global $user;
    $post->uid = $user->uid;
  }

  foreach ($form_state['values'] as $property => $value) {
    if (substr($property, 0, 9) == 'settings_') {
      $key = substr($property, 9);
      $post->settings[$key] = $value;
    }
  }

  foreach ($post as $property => $value) {
    if (isset($form_state['values'][$property])) {
      $post->$property = $form_state['values'][$property];
    }
  }

  field_attach_submit('election_post', $post, $form, $form_state);

  // Save the post.
  election_post_save($post);

  // Notify the user that the post was saved.
  drupal_set_message(
    t('The @post_name "@title" was saved.', array('@post_name' => _election_get_posts_name($election->type), '@title' => $post->title))
  );

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_post_uri_path($post);
  }

}

/**
 * Form builder function to confirm the delete action for an election post.
 *
 * See http://api.drupal.org/api/drupal/modules--system--system.module/function/confirm_form.
 */
function election_post_form_delete_confirm($form, &$form_state, stdClass $post) {

  $election = $post->election;

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_MANAGE_POSTS_', l($post->title, election_post_uri_path($post)))
  );

  $form['election_id'] = array(
    '#type' => 'value',
    '#value' => $election->election_id,
  );
  $form['post_id'] = array(
    '#type' => 'value',
    '#value' => $post->post_id,
  );

  return confirm_form(
    $form,
    t(
      'Are you sure you want to delete the @post_name %post_title?',
      array(
        '@post_name' => _election_get_posts_name($election->type),
        '%post_title' => $post->title,
      )
    ),
    election_post_uri_path($post),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submit function for the Delete button in the form election_post_form_delete_confirm.
 */
function election_post_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {

    $election = election_load($form_state['values']['election_id']);

    $post = election_post_load($form_state['values']['post_id']);

    // Delete the post.
    $delete = election_post_delete($post->post_id);

    $post_name = _election_get_posts_name($election->type);

    if ($delete) {
      watchdog('election_post', 'Deleted @post_name %title', array('@post_name' => $post_name, '%title' => $post->title));
    }

    if ($delete) {
      drupal_set_message(
        t('The @post_name %title has been deleted.', array('@post_name' => $post_name, '%title' => $post->title))
      );
    }
    else {
      drupal_set_message(
        t('Failed to delete the @post_name %title.', array('@post_name' => $post_name, '%title' => $post->title)),
        'error'
      );
    }

  }

  if (empty($_GET['destination'])) {
    $form_state['redirect'] = election_uri_path($election) . '/posts';
  }

}

/**
 * Form builder function for the administrative list of election posts at election/%election/posts/manage.
 */
function election_post_manage_form($form, &$form_state, stdClass $election) {

  $title = election_post_manage_page_title($election);
  drupal_set_title($title, PASS_THROUGH);
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election)
  );

  $form['#election'] = $election;

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'ep.title', 'sort' => 'asc'),
    'changed' => array('data' => t('Last updated'), 'field' => 'ep.changed'),
    'operations' => array('data' => t('Operations')),
  );

  // Build the posts query, adding a pager and sorter.
  $query = db_select('election_post', 'ep')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ep', array('post_id'))
    ->condition('election_id', $election->election_id);

  $query->limit(50)->orderByHeader($header);

  // Get an array of post IDs.
  $post_ids = $query->execute()->fetchCol();

  $posts = election_post_load_multiple($post_ids);
  $form['#posts'] = $posts;

  $destination = drupal_get_destination();

  $rows = array();
  foreach ($posts as $id => $post) {

    $rows[$post->post_id] = array(
      'title' => l($post->title, election_post_uri_path($post)),
      'changed' => format_date($post->changed, 'short'),
      'operations' => array(),
    );

    $operations = array();
    if (entity_access('update', 'election', $election)) {
      $operations['edit'] = l(
        t('edit'),
        election_post_uri_path($post) . '/edit',
        array('query' => $destination)
      );
      $operations['delete'] = l(
        t('delete'),
        election_post_uri_path($post) . '/delete',
        array('query' => $destination)
      );
    }

    if (count($operations)) {
      // Render an unordered list of operations links.
      $rows[$post->post_id]['operations'] = array(
        'data' => array(
          '#theme' => 'item_list',
          '#items' => $operations,
          '#attributes' => array('class' => array('operations')),
        ),
      );
    }

  }

  $form['election_posts'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No @posts_name available.', array('@posts_name' => _election_get_posts_name($election->type, TRUE))),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;

}


/**
 * Page callback for editing an election post, at election-post/%election_post/edit.
 */
function election_post_page_edit($post) {
  $title = election_post_page_title($post);
  drupal_set_title(t('Edit: %title', array('%title' => truncate_utf8($title, 20, TRUE, TRUE))), PASS_THROUGH);

  $election = $post->election;

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_MANAGE_POSTS_', l($post->title, election_post_uri_path($post)))
  );

  return drupal_get_form('election_post_form', $post, $election);
}

/**
 * Page callback for adding an election post, path: election/%election/posts/add.
 */
function election_post_page_add($election) {

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_MANAGE_POSTS_')
  );

  if (!is_object($election)) {
    return MENU_NOT_FOUND;
  }

  $values = array();
  $values['election_id'] = $election->election_id;
  $type = _election_type_get_info($election->type);
  $values['type'] = $type['post machine name'];

  $post = entity_get_controller('election_post')->create($values);

  $post_name = 'post';
  if (!empty($type['post name'])) {
    $post_name = $type['post name'];
  }

  drupal_set_title(
    t('Add new @post_name to %election', array('%election' => $election->title, '@post_name' => $post_name)),
    PASS_THROUGH
  );

  return drupal_get_form('election_post_form', $post, $election);

}
