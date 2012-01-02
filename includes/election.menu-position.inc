<?php
/**
 * @file
 * Provides the Election rule plugin for the Menu Position module.
 */

/**
 * Checks if the election is of a certain type.
 *
 * @param $variables
 *   An array containing each of the variables saved in the database necessary
 *   to evaluate this condition of the rule.
 * @return
 *   TRUE if condition applies successfully. Otherwise FALSE.
 */
function election_menu_position_condition_election($variables) {
  list($arg0, $arg1, ) = explode('/', $_GET['q']);
  if ($arg0 == 'election' && is_numeric($arg1)) {
    $election = election_load($arg1);
    $type = $election->type;
  }
  if (empty($election)) {
    return FALSE;
  }
  return (in_array($type, $variables['election_type']));
}

/**
 * Adds form elements for the "election" plugin to the rule configuration form.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array. New form elements should be
 *   added directly to this array.
 * @param $form_state
 *   A reference to the current form state.
 */
function election_menu_position_rule_election_form(&$form, &$form_state) {
  // If this is an existing rule, load the variables stored in the rule for this plugin.
  $variables = !empty($form_state['#menu-position-rule']['conditions']['election']) ? $form_state['#menu-position-rule']['conditions']['election'] : array();

  $form['conditions']['election'] = array(
    '#type' => 'fieldset',
    '#title' => t('Elections'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
/*
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'election') . '/includes/election.menu-position.js'),
    ),
*/
  );

  $form['conditions']['election']['election_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Election types'),
    '#default_value' => !empty($variables['election_type']) ? $variables['election_type'] : array(),
    '#options' => election_type_get_names(),
    '#description' => t('Apply this rule only for elections of the given type(s).'),
    '#weight' => -20,
  );

  // Add a submit handler.
  $form['#submit'][] = 'election_menu_position_rule_election_form_submit';
}

/**
 * Prepares the "election_type" variables to be stored in the rule.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array.
 * @param $form_state
 *   A reference to the current form state, including submitted values.
 */
function election_menu_position_rule_election_form_submit(&$form, &$form_state) {
  // The user has added our plugin's form elements as a condition for the rule.
  if (!empty($form_state['values']['election_type'])) {
    // Remove any 0 valued options.
    foreach ($form_state['values']['election_type'] as $key => $value) {
      if ($value === 0) {
        unset($form_state['values']['election_type'][$key]);
      }
    }
    // Determine if any checkboxes were on.
    if (!empty($form_state['values']['election_type'])) {
      // Add this plugin's variables to the rule.
      $variables = array(
        'election_type' => $form_state['values']['election_type'],
      );
      $form_state['values']['conditions']['election'] = $variables;
    }
  }
}