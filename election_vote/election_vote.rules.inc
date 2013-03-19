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
 * Implements hook_rules_action_info().
 */
function election_vote_rules_action_info() {
  $actions = array();

  $actions['election_vote_abstain_message'] = array(
    'label' => t('Display a "You have abstained" message'),
    'parameter' => array(
      'post' => array(
        'type' => 'election_post',
        'label' => t('The election post'),
      ),
    ),
    'group' => t('Election Vote'),
    'callbacks' => array(
      'execute' => 'election_vote_rules_abstain_message',
    ),
  );

  $actions['election_vote_vote_message'] = array(
    'label' => t('Display a "Thank you for voting" message'),
    'parameter' => array(
      'post' => array(
        'type' => 'election_post',
        'label' => t('The election post'),
      ),
    ),
    'group' => t('Election Vote'),
    'callbacks' => array(
      'execute' => 'election_vote_rules_vote_message',
    ),
  );

  return $actions;
}

/**
 * Rules action: displays a "You have abstained" message.
 */
function election_vote_rules_abstain_message($post) {
  drupal_set_message(
    t('You have abstained from voting (@post_name: <a href="!post_url">@post_title</a>).', array(
      '@post_name' => election_get_post_name($post->election->type),
      '!post_url' => url(election_post_uri_path($post)),
      '@post_title' => $post->title,
    ))
  );
}

/**
 * Rules action: displays a "Thank you for voting" message.
 */
function election_vote_rules_vote_message($post) {
  drupal_set_message(
    t('Thank you for voting (@post_name: <a href="!post_url">@post_title</a>).', array(
      '@post_name' => election_get_post_name($post->election->type),
      '!post_url' => url(election_post_uri_path($post)),
      '@post_title' => $post->title,
    ))
  );
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
