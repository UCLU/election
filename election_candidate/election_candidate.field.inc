<?php
/**
 * @file
 * Field integration for the Election Candidate module.
 */

/**
 * Implements hook_field_default_fields().
 */
function election_candidate_field_default_fields() {
  $fields = array();

  $fields['election_candidate-candidate-field_phone_number'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_phone_number',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'candidate',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => t('Please enter a contact telephone number.'),
      'display' => array(
        // Phone numbers are private: hide by default but display in Details.
        'default' => array(
          'label' => 'inline',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2',
        ),
        'details' => array(
          'label' => 'inline',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_plain',
          'weight' => '4',
        ),
      ),
      'entity_type' => 'election_candidate',
      'field_name' => 'field_phone_number',
      'label' => t('Phone number'),
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '12',
        ),
        'type' => 'text_textfield',
        'weight' => '8',
      ),
    ),
  );

  return $fields;
}
