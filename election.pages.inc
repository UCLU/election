<?php


function election_add_page() {
  
  $item = menu_get_item();
  $links = system_admin_menu_block($item);
  
  foreach ($links as $link) {
    
    $items[] = l($link['title'], $link['href'], $item['localized_options'])
             . ': ' . filter_xss_admin($link['description']);
    
  }
  
  return theme('item_list', array('items' => $items));
  
}


function election_main_page() {
  
  $item = menu_get_item();
  $links = system_admin_menu_block($item);
  
  foreach ($links as $link) {
    
    $items[] = l($link['title'], $link['href'], $item['localized_options'])
             . ': ' . filter_xss_admin($link['description']);
    
  }
  
  return theme('item_list', array('items' => $items));
  
}



function election_forms() {
  
  $forms = array();
  if ($types = election_types()) {
    foreach (array_keys($types) as $type) {
      $forms[$type . '_election_form']['callback'] = 'election_form';
    }
  }
  return $forms;
  
}

function election_form($form, &$form_state, $election) {
  
  $form['#id'] = 'election-form';
  $form['#election'] = $election;
  
  $form_state['election'] = $election;
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $election->title,
    '#weight' => -5,
    '#required' => TRUE,
  );
  
  $form['buttons'] = array();
  $form['buttons']['#weight'] = 100;
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('election_form_submit'),
  );
  
  if (!empty($election->election_id)) {    
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#delete' => array('election_form_delete_submit'),
      '#access' => user_access('delete elections'),
    );
  }
  
  $form['#validate'][] = 'election_form_validate';
  
  // Add custom fields from the Field system.
  field_attach_form('election', $election, $form, $form_state);
  
  return $form;
  
}

function election_form_validate($form, &$form_state) {
  $election = $form_state['election']; // @TODO check if this should be passed by reference
  field_attach_form_validate('election', $election, $form, $form_state);
}

function election_form_submit($form, &$form_state) {
  global $user;
  $election = &$form_state['election'];
  
  if (empty($election->uid)) {
    $election->uid = $user->uid;
  }
  
  $election->name = $form_state['values']['name'];
  
  field_attach_submit('election', $election, $form, $form_state);
  
  // Save the election.
  election_save($election);
  
  // Notify the user that the election was saved.
  drupal_set_message(
    t('The election "@name" was saved.', array('@name' => $election->name))
  );
  
  $form_state['redirect'] = 'election/' . $election->election_id;  
}