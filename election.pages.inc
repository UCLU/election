<?php
/**
 * @file
 * Page callbacks for the Election module.
 */


function election_page() {

  $item = menu_get_item();
  $links = system_admin_menu_block($item);

  foreach ($links as $link) {

    $items[] = array(
      'title' => $link['title'],
      'href' => $link['href'],
      'localized_options' => $item['localized_options'],
      'description' => $link['description'],
    );

  }

  return theme('admin_block_content', array('content' => $items));

}

function election_list() {

  return FALSE;

}

function election_page_type_overview() {

  $item = menu_get_item();

  $output = array();

  foreach (election_types() as $type) {

    $output['content'][] = array(
      'title' => 'Manage ' . $type->name,
      'href' => $item['href'] . '/' . $type->type,
      'localized_options' => $item['localized_options'],
      'description' => $type->description,
    );

  }

  return theme('admin_block_content', $output);

}

function election_type_title($type_path) {
  if (!$type = election_load_type($type_path)) {
    return FALSE;
  }
  return $type->name;
}

function election_page_title($election) {
  return $election->name;
}

function election_page_view($election, $view_mode = 'full') {

  // Remove previously built content, if it exists.
  $election->content = array();

  if ($view_mode == 'teaser') {
    $election->content['title'] = array(
      '#markup' => filter_xss($election->name),
      '#weight' => -5,
    );
  }

  $election->content['description'] = array(
    '#markup' => filter_xss($election->description),
    '#weight' => 0,
  );

  field_attach_prepare_view('election', array($election->election_id => $election), $view_mode);
  entity_prepare_view('election', array($election->election_id => $election));
  $election->content += field_attach_view('election', $election, $view_mode);

  return $election->content;
}

function election_page_type($type_path) {
  $type = election_type_load($type_path);
  return $type->name . ': ' . $type->description;
}

function election_page_add($type_path) {

  global $user;

  if (!$type = election_type_load($type_path)) {
    return MENU_NOT_FOUND;
  }

  $election = entity_get_controller('election')->create($type->type);

  drupal_set_title(
    t('Add new @name', array('@name' => $type->name)),
    PASS_THROUGH
  );

  return drupal_get_form($type->type . '_election_form', $election);

}