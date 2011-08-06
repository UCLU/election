<?php
/**
 * @file
 * Administration page callbacks for election posts in the Election module.
 */

/**
 * Form builder function for adding or editing an election post.
 *
 * @param array $form
 * @param array &$form_state
 * @param stdClass $election
 * @param stdClass $post
 *
 * @return array
 *   Render array.
 */
function election_post_form($form, &$form_state, $election, $post) {

  $post->is_new = empty($post->post_id);

  if ($post->is_new) {
    $breadcrumb = _election_build_breadcrumb(
      $election,
      '_POSTS_',
      t('Add new')
    );
  }
  else {
    $breadcrumb = _election_build_breadcrumb(
      $election,
      '_POSTS_',
      l($post->title, election_post_uri_path($post)),
      t('Edit')
    );
  }

  drupal_set_breadcrumb($breadcrumb);

  $form['#id'] = 'election-post-form';
  $form['#post'] = $post;

  $form_state['post'] = $post;
  $form_state['election'] = $election;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($post->title)? $post->title : '',
    '#required' => TRUE,
  );

  $type = _election_type_load($election->type);
  $post_key = $type['post key'];

  $post_form_function = 'election_post_' . $post_key . '_form';
  if (function_exists($post_form_function)) {
    $form = $post_form_function($form, $form_state, $election, $post);
  }

  $form['vstatus_inheritance'] = array(
    '#type' => 'radios',
    '#title' => t('Voting status'),
    '#default_value' => isset($post->vstatus_inheritance)? $post->vstatus_inheritance : ELECTION_POST_STATUS_INHERIT,
    '#options' => array(
      ELECTION_POST_STATUS_INHERIT => t('Inherited (currently: %status)', array('%status' => strip_tags(_election_format_status($election, 'v')))),
      ELECTION_POST_STATUS_CLOSED => t('Closed'),
    ),
    '#required' => TRUE,
  );

  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
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
      '#access' => election_access('update', $election), // deleting a post is really 'updating' an election
    );
  }

  $form['#validate'][] = 'election_post_form_validate';

  // Add custom fields from the Field system.
  field_attach_form('election_post', $post, $form, $form_state);

  return $form;
}

/**
 * Partial form builder function for [referendum] motions.
 *
 * @see election_post_form()
 *
 * @param array $form
 * @param array &$form_state
 * @param stdClass $election
 * @param stdClass $post
 *
 * @return array
 */
function election_post_motion_form($form, &$form_state, $election, $post) {

  $form['allow_abstention'] = array(
    '#type' => 'select',
    '#title' => t('Allow abstention'),
    '#description' => t(
      'Select whether voters would be able to select @abstain as an option alongside @yes and @no.',
      array(
        '@abstain' => t('Abstain'),
        '@yes' => t('Yes'),
        '@no' => t('No'),
      )
    ),
    '#default_value' => isset($post->allow_abstention)? $post->allow_abstention : 1,
    '#options' => array(
      1 => t('Allow abstention'),
      0 => t('Do not allow abstention'),
    ),
  );

  return $form;
}

/**
 * Partial form builder function for [electoral] positions.
 *
 * @see election_post_form()
 *
 * @param array $form
 * @param array &$form_state
 * @param stdClass $election
 * @param stdClass $post
 *
 * @return array
 */
function election_post_position_form($form, &$form_state, $election, $post) {

  $form['vacancy_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of vacancies'),
    '#default_value' => isset($post->vacancy_count)? $post->vacancy_count : 1,
    '#size' => 2,
    '#maxlength' => 2,
    '#required' => TRUE,
  );

  $form['use_ron'] = array(
    '#type' => 'select',
    '#title' => t('RON (Re-Open Nominations)'),
    '#description' => t('RON can only be included for single-vacancy positions.'),
    '#default_value' => isset($post->use_ron)? $post->use_ron : 1,
    '#options' => array(
      1 => t('Include RON as a candidate'),
      0 => t('Do not include RON'),
    ),
    '#states' => array(
      'enabled' => array(
        ':input[name=vacancy_count]' => array('value' => '1'),
      ),
    ),
  );

  $form['nominations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nominations form settings'),
  );

  $form['nominations']['require_manifesto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require a manifesto'),
    '#default_value' => empty($post->require_manifesto)? 0 : 1,
  );
  $form['nominations']['require_proposer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require proposer details'),
    '#default_value' => empty($post->require_proposer)? 0 : 1,
  );
  $form['nominations']['require_phone'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require a phone number'),
    '#default_value' => empty($post->require_phone)? 0 : 1,
  );
  $form['nominations']['require_photo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require a photo'),
    '#default_value' => empty($post->require_photo)? 0 : 1,
  );
  $form['nominations']['nstatus_inheritance'] = array(
    '#type' => 'radios',
    '#title' => t('Nominations status'),
    '#default_value' => isset($post->nstatus_inheritance)? $post->nstatus_inheritance : ELECTION_POST_STATUS_INHERIT,
    '#options' => array(
      ELECTION_POST_STATUS_INHERIT => t('Inherited (currently: %status)', array('%status' => strip_tags(_election_format_status($election, 'n')))),
      ELECTION_POST_STATUS_CLOSED => t('Closed'),
    ),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Form submit function for the Delete button in the form election_post_form.
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_post_form_delete_submit($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $post = $form['#post'];
  $election = $form['#election'];
  $form_state['redirect'] = array(
    election_post_uri_path($post, $election) . '/delete',
    array('query' => $destination),
  );
}


/**
 * Form validate function for the form election_post_form.
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_post_form_validate($form, &$form_state) {

  // Run posts form checks here.

  // Check other Fields.
  field_attach_form_validate('election_post', $form_state['post'], $form, $form_state);

}

/**
 * Form submit function for the Save button in the form election_post_form.
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_post_form_submit($form, &$form_state) {

  $post = &$form_state['post'];

  // election object won't be changed, no need to pass by reference
  $election = $form_state['election'];

  if (empty($post->uid)) {
    global $user;
    $post->uid = $user->uid;
  }

  $post->title = $form_state['values']['title'];
  $post->description = $form_state['values']['description'];
  $post->vstatus_inheritance = $form_state['values']['vstatus_inheritance'];

  $post->allow_abstention = $form_state['values']['allow_abstention'];

  $post->nstatus_inheritance = $form_state['values']['nstatus_inheritance'];
  $post->vacancy_count = $form_state['values']['vacancy_count'];
  $post->use_ron = $form_state['values']['use_ron'];
  $post->require_phone = $form_state['values']['require_phone'];
  $post->require_proposer = $form_state['values']['require_proposer'];
  $post->require_manifesto = $form_state['values']['require_manifesto'];
  $post->require_photo = $form_state['values']['require_photo'];

  field_attach_submit('election_post', $post, $form, $form_state);

  // Save the post.
  election_post_save($post);

  // Notify the user that the post was saved.
  drupal_set_message(
    t('The @post_name "@title" was saved.', array('@post_name' => _election_get_posts_name($election->type), '@title' => $post->title))
  );

  //$form_state['redirect'] = election_uri_path($election) . '/posts';

}

/**
 * Form builder function to confirm the delete action for an election post.
 * See http://api.drupal.org/api/drupal/modules--system--system.module/function/confirm_form.
 *
 * @param array $form
 * @param array &$form_state
 * @param stdClass $election
 * @param stdClass $post
 *
 * @return array
 *   Render array.
 */
function election_post_form_delete_confirm($form, &$form_state, $election, $post) {

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_POSTS_', l($post->title, election_post_uri_path($post)), 'Delete')
  );

  $form['election_id'] = array(
    '#type' => 'value',
    '#value' => $election->election_id,
  );
  $form['post_id'] = array(
    '#type' => 'value',
    '#value' => $post->post_id,
  );

  $type = _election_type_load($election->type);

  return confirm_form(
    $form,
    t(
      'Are you sure you want to delete the @post_name %post_title from the @election_type %election_title?',
      array(
        '@post_name' => _election_get_posts_name($election->type),
        '@election_type' => $type['name'],
        '%election_title' => $election->title,
        '%post_title' => $post->title,
      )
    ),
    election_post_uri_path($post, $election),
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Form submit function for the Delete button in the form election_post_form_delete_confirm.
 *
 * @param array $form
 * @param array &$form_state
 * @return void
 */
function election_post_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {

    $election = election_load($form_state['values']['election_id']);

    $post = election_post_load($form_state['values']['post_id']);

    // Delete the post.
    $delete = election_post_delete($form_state['values']['post_id']);

    $post_name = _election_get_posts_name($election->type);

    if ($delete) {
      watchdog('election', 'Deleted @post_name %title', array('@post_name' => $post_name, '%title' => $post->title));
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

  $form_state['redirect'] = election_uri_path($election) . '/posts';

}

/**
 * Form builder function for the administrative list of election posts, path:
 * election/%election/posts.
 *
 * @param array $form
 * @param array $form_state
 * @param stdClass $election
 *
 * @return array
 *   Render array.
 */
function election_post_list_form($form, $form_state, $election) {

  $posts_name = _election_get_posts_name($election->type, TRUE);
  drupal_set_title(drupal_ucfirst($posts_name));
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, drupal_ucfirst($posts_name))
  );

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

  // Get an array of post IDs
  $post_ids = $query->execute()->fetchCol();

  $posts = election_post_load_multiple($post_ids);

  $destination = drupal_get_destination();

  $rows = array();
  foreach ($posts as $id => $post) {

    $rows[$post->post_id] = array(
      'title' => l($post->title, election_post_uri_path($post)),
      'changed' => format_date($post->changed, 'short'),
      'operations' => array(),
    );

    $operations = array();
    if (election_access('update', $election)) {
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
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }

  }

  $form['election_posts'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No @type available.', array('@type' => $posts_name)),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;

}


/**
 * Page callback for editing an election post, path:
 * election/%election/posts/%election_post/edit.
 *
 * @param stdClass $election
 * @param stdClass $post
 *
 * @return array
 *   Render array generated by Form API for the form: election_post_form.
 */
function election_post_page_edit($election, $post) {
  $title = election_post_page_title($election, $post);
  drupal_set_title(t('Edit: %title', array('%title' => $title)), PASS_THROUGH);

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_POSTS_', l($post->title, election_post_uri_path($post)), 'Edit')
  );

  return drupal_get_form('election_post_form', $election, $post);
}

/**
 * Page callback for adding an election post, path:
 * election/%election/posts/add.
 *
 * @param stdClass $election
 *
 * @return array
 *   Render array generated by Form API for the form: election_post_form.
 */
function election_post_page_add($election) {

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, '_POSTS_', 'Add new')
  );

  global $user;

  if (!is_object($election)) {
    return MENU_NOT_FOUND;
  }

  $post = entity_get_controller('election_post')->create($election);

  $post_name = 'post';
  $type = _election_type_load($election->type);
  if (!empty($type['post name'])) {
    $post_name = $type['post name'];
  }

  drupal_set_title(
    t('Add new @post_name to %election', array('%election' => $election->title, '@post_name' => $post_name)),
    PASS_THROUGH
  );

  return drupal_get_form('election_post_form', $election, $post);

}
