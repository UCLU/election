<?php
/**
 * @file
 * Candidate type editing UI.
 */

/**
 * Generates the candidate type editing form.
 */
function election_candidate_type_form($form, &$form_state, $candidate_type, $op = 'update') {

  if ($op == 'clone') {
    $candidate_type->label .= ' (cloned)';
    $candidate_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $candidate_type->label,
    '#description' => t('The human-readable name of this candidate type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($candidate_type->type) ? $candidate_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'election_candidate_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['create_field_candidate_name'] = array(
    '#title' => t('Add name (<code>candidate_name</code>) field.'),
    '#type' => 'checkbox',
    '#default_value' => 1,
    '#disabled' => TRUE,
  );

  $form['create_field_candidate_mail'] = array(
    '#title' => t('Add e-mail address (<code>candidate_mail</code>) field.'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save candidate type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the candidate type form.
 */
function election_candidate_type_form_submit(&$form, &$form_state) {
  // Create the candidate type entity.
  $candidate_type = entity_ui_form_submit_build_entity($form, $form_state);
  $candidate_type->save();

  // Create field instances.
  $add_name_field = !empty($form_state['values']['create_field_candidate_name']);
  $add_mail_field = !empty($form_state['values']['create_field_candidate_mail']);
  if ($add_name_field || $add_mail_field) {
    module_load_include('inc', 'election_candidate', 'election_candidate.field');
    $field_instances = _election_candidate_installed_field_instances();
    foreach ($field_instances as $instance) {
      if (($instance['field_name'] == 'candidate_name' && $add_name_field)
        || ($instance['field_name'] == 'candidate_mail' && $add_mail_field)) {
        $instance['bundle'] = $candidate_type->type;
        field_create_instance($instance);
      }
    }
  }

  $form_state['redirect'] = 'admin/config/election/candidate-types';
}

/**
 * Form API submit callback for the delete button on the candidate type form.
 */
function election_candidate_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/config/election/candidate-types/' . $form_state['candidate_type']->type . '/delete';
}
