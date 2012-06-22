<?php
/**
 * @file
 * Token integration for the Election module.
 */

/**
 * Implements hook_token_info().
 *
 * See http://api.drupal.org/api/drupal/modules--system--system.api.php/function/hook_token_info/7.
 */
function election_token_info() {
  $info = array();
  $info['types'] = array(
    'election' => array(
      'name' => t('Elections'),
      'description' => t('Tokens related to elections.'),
      'needs-data' => 'election',
    ),
  );
  $info['tokens'] = array(
    'election' => array(
      'id' => array(
        'name' => t('Election ID'),
        'description' => t('The unique ID of the election.'),
      ),
      'type' => array(
        'name' => t('Election type'),
        'description' => t('The machine-readable name of the election type.'),
      ),
      'type_name' => array(
        'name' => t('Election type name'),
        'description' => t('The human-readable name of the election type.'),
      ),
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the election.'),
      ),
      'created' => array(
        'name' => t('Date created'),
        'description' => t('The date the election was created.'),
        'type' => 'date',
      ),
    ),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function election_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  if ($type == 'election' && !empty($data['election'])) {
    $election = $data['election'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = $election->election_id;
          break;

        case 'type':
          $replacements[$original] = $sanitize ? check_plain($election->type) : $election->type;
          break;

        case 'type_name':
          $type = _election_type_get_info($election->type);
          $type_name = $type['name'];
          $replacements[$original] = $sanitize ? check_plain($type_name) : $type_name;
          break;

        case 'title':
          $replacements[$original] = $sanitize ? check_plain($election->title) : $election->title;
          break;

        case 'created':
          $replacements[$original] = format_date($election->created, 'medium');
          break;

      }
    }
  }
  return $replacements;
}
