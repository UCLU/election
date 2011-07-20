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
  $type = str_replace('-', '_', $type_path);
  $types = election_types();
  return isset($types[$type])? $types[$type]->name : FALSE;
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

function election_page_type($type) {
  $types = election_types();
  $type = $types[$type];
  return $type->name . ': ' . $type->description;
}
