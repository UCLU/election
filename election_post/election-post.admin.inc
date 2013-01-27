<?php
/**
 * @file
 * Administration page callbacks for the Election Post module.
 */

/**
 * Page callback for managing election posts.
 */
function election_post_manage_page(stdClass $election) {
  $posts_name = election_get_post_name($election->type, TRUE);
  drupal_set_title(t('Manage @posts', array('@posts' => $posts_name)), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb($election));
  $view = views_get_view('election_posts_admin');
  // Explicitly set the path, so that Views Bulk Operations could work if
  // configured (VBO needs help setting the form's "action" attribute).
  $view->override_path = current_path();
  return $view->preview('embed', array($election->election_id));
}

/**
 * Page callback for editing an election post.
 *
 * Path: election-post/%election_post/edit.
 */
function election_post_page_edit($post) {

  $title = election_post_page_title($post);
  drupal_set_title(
    t(
      'Edit: %title',
      array('%title' => truncate_utf8($title, 20, TRUE, TRUE))
    ),
    PASS_THROUGH
  );

  $election = $post->election;

  drupal_set_breadcrumb(
    _election_build_breadcrumb(
      $election,
      '_MANAGE_POSTS_',
      l($post->title, election_post_uri_path($post))
    )
  );

  module_load_include('inc', 'election_post', 'election_post.forms');

  return drupal_get_form('election_post_form', $post, $election);
}

/**
 * Page callback for adding an election post.
 *
 * Path: election/%election/posts/add.
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
  if (!$type) {
    return t(
      'Cannot add post: the election type %type could not be found.',
      array('%type' => $election->type)
    );
  }

  $values['type'] = $type['post machine name'];

  $post = entity_get_controller('election_post')->create($values);

  $post_name = 'post';
  if (!empty($type['post name'])) {
    $post_name = $type['post name'];
  }

  drupal_set_title(
    t('Add new @post to %election', array('%election' => $election->title, '@post' => $post_name)),
    PASS_THROUGH
  );

  module_load_include('inc', 'election_post', 'election_post.forms');

  return drupal_get_form('election_post_form', $post, $election);

}
