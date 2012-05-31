<?php
/**
 * @file
 * Pathauto hooks for the Election Candidate module.
 */

/**
 * Implements hook_pathauto(). See the Pathauto module.
 *
 * @param string $op
 * @return stdClass
 */
function election_candidate_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'election_candidate';
      $settings['token_type'] = 'election_candidate';
      $settings['groupheader'] = t('Election candidate paths');
      $settings['patterndescr'] = t('Default path pattern');
      $settings['patterndefault'] = 'election/[election_candidate:election:title]/candidate/[election_candidate:first_name]-[election_candidate:last_name]';
      $settings['batch_update_callback'] = 'election_candidate_pathauto_bulk_update_batch_process';
      $settings['batch_file'] = drupal_get_path('module', 'election_candidate') . '/election_candidate.pathauto.inc';
      $elections = db_query('SELECT election_id, title FROM election WHERE type != \'referendum\'')->fetchAllAssoc('election_id');
      foreach ($elections as $election_id => $election) {
        $settings['patternitems'][$election_id] = t('Pattern for candidates in the election %election', array('%election' => $election->title));
      }
      return (object) $settings;
    default:
      break;
  }
}

/**
 * Allow bulk updating of paths. See the Pathauto module.
 *
 * @see election_candidate_pathauto()
 * @param array &$context
 * @return void
 */
function election_candidate_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }
  $query = db_select('election_candidate', 'ec');
  $query->leftJoin('url_alias', 'ua', "CONCAT('election/', ec.election_id, '/candidate/', ec.candidate_id) = ua.source");
  $query->addField('ec', 'candidate_id');
  $query->isNull('ua.source');
  $query->condition('ec.candidate_id', $context['sandbox']['current'], '>');
  $query->orderBy('ec.candidate_id');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'election_candidate');
  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();
    // If there are no elections to update, then stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }
  $query->range(0, 25);
  $ids = $query->execute()->fetchCol();
  pathauto_election_candidate_update_alias_multiple($ids, 'bulkupdate');
  $context['sandbox']['count'] += count($ids);
  $context['sandbox']['current'] = max($ids);
  $context['message'] = t('Updated alias for election candidate @id.', array('@id' => end($ids)));
  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}


/**
 * Implements hook_path_alias_types(). See Pathauto module.
 *
 * Used primarily by the bulk delete form.
 */
function election_candidate_path_alias_types() {
  $objects = array(
    'election/%/candidate/' => t('Election candidates')
  );
  return $objects;
}