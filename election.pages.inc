<?php
/**
 * @file
 * Page callbacks for the Election module.
 */

function election_post_page($election, $post) {

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, 'Posts', $post->title)
  );

  return array('#markup' => 'Under construction');
}

/**
 * Page callback for elections/%/%election/posts.
 * @todo use Views
 */
function election_page_postlist($election) {

  $election_posts_title = _election_get_posts_title($election);
  drupal_set_title($election_posts_title);
  drupal_set_breadcrumb(
    _election_build_breadcrumb($election, $election_posts_title)
  );

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'ep.title'),
    'changed' => array('data' => t('Last updated'), 'field' => 'ep.changed', 'sort' => 'desc'),
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
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $post->title,
          '#href' => election_post_uri_path($post),
        ),
      ),
      'changed' => format_date($post->changed, 'short'),
      'operations' => array(),
    );

    $operations = array();
    if (election_access('update', $election)) {
      $operations['edit'] = l(
        t('Edit'),
        election_post_uri_path($post) . '/edit',
        array('query' => $destination)
      );
      $operations['delete'] = l(
        t('Delete'),
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

  $output['election_posts'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No @type available.', array('@type' => $election_posts_title)),
  );

  $output['pager'] = array('#markup' => theme('pager'));

  return $output;

}

function election_page_view($election, $view_mode = 'full') {

  drupal_set_breadcrumb(
    _election_build_breadcrumb($election)
  );

  // Remove previously built content, if it exists.
  $election->content = array();

  if ($view_mode == 'teaser') {
    $election->content['title'] = array(
      '#markup' => filter_xss($election->title),
      '#weight' => -5,
    );
  }
  else {
    drupal_set_title($election->title);
  }

/*
  $election->content['description'] = array(
    '#markup' => filter_xss($election->description),
    '#weight' => 0,
  );
*/

  field_attach_prepare_view('election', array($election->election_id => $election), $view_mode);
  entity_prepare_view('election', array($election->election_id => $election));
  $election->content += field_attach_view('election', $election, $view_mode);

  return $election->content;
}
