<?php
/**
 * @file
 * Rules integration file for the Election module.
 */

/**
 * Implements hook_rules_event_info().
 */
function election_rules_event_info() {
  $events = array();
  $events['election_candidate_nomination_submitted'] = array(
    'label' => t('Candidate nomination submitted'),
    'module' => 'election',
    'arguments' => array(
      'candidate' => array(
        'type' => 'election_candidate',
        'label' => t('The candidate.'),
      ),
      'post' => array(
        'type' => 'election_post',
        'label' => t('The position for which the nomination was submitted.'),
      ),
      'election' => array(
        'type' => 'election',
        'label' => t('The election to which the position belongs.'),
      ),
    ),
  );
  return $events;
}
