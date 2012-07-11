<?php
/**
 * @file
 * Rules integration file for the Election Vote module.
 */

/**
 * Implements hook_rules_event_info().
 */
function election_vote_rules_event_info() {
  $defaults = array(
    'group' => t('Election'),
    'module' => 'election_vote',
    'access callback' => 'election_vote_rules_config_access',
  );
  $events = array();
  $events['election_vote_vote'] = $defaults + array(
    'label' => t('After casting a vote'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The user who cast the vote'),
        'skip save' => TRUE,
      ),
      'post' => array(
        'type' => 'election_post',
        'label' => t('The post for which the vote was cast'),
        'skip save' => TRUE,
      ),
      'election' => array(
        'type' => 'election',
        'label' => t('The election to which the post belongs'),
        'skip save' => TRUE,
      ),
    ),
  );
  $events['election_vote_abstain'] = $defaults + array(
    'label' => t('After abstaining'),
    'variables' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('The user who abstained'),
        'skip save' => TRUE,
      ),
      'post' => array(
        'type' => 'election_post',
        'label' => t('The post for which the user abstained'),
        'skip save' => TRUE,
      ),
      'election' => array(
        'type' => 'election',
        'label' => t('The election to which the post belongs'),
        'skip save' => TRUE,
      ),
    ),
  );
  return $events;
}

/**
 * Access callback for configuring Rules related to voting.
 *
 * @see election_vote_rules_event_info()
 */
function election_vote_rules_config_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return election_access('edit') && entity_access('view', 'user');
  }
}
