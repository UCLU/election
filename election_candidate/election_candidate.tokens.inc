<?php
/**
 * @file
 * Token integration for the Election Candidate module.
 */

/**
 * Implements hook_token_info().
 */
function election_candidate_token_info() {
  $info = array();
  $info['types'] = array(
    'election_candidate' => array(
      'name' => t('Candidates'),
      'description' => t('Tokens related to election candidates.'),
      'needs-data' => 'election_candidate',
    ),
  );
  $info['tokens'] = array(
    'election_candidate' => array(
      'id' => array(
        'name' => t('Election candidate ID'),
        'description' => t('The unique ID of the candidate.'),
      ),
      'election' => array(
        'name' => t('Election'),
        'description' => t('The election in which the candidate is standing.'),
        'type' => 'election',
      ),
      'mail' => array(
        'name' => t('E-mail address'),
        'description' => t('The e-mail address of the candidate.'),
      ),
      'first_name' => array(
        'name' => t('First name'),
        'description' => t('The first name of the candidate.'),
      ),
      'last_name' => array(
        'name' => t('Last name'),
        'description' => t('The last name of the candidate.'),
      ),
    ),
  );
  return $info;
}

/**
 * Implements hook_tokens().
 */
function election_candidate_tokens($type, array $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  if ($type == 'election_candidate' && !empty($data['election_candidate'])) {
    $candidate = $data['election_candidate'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'id':
          $replacements[$original] = $candidate->candidate_id;
          break;

        case 'mail':
          $replacements[$original] = $sanitize ? check_plain($candidate->mail) : $candidate->mail;
          break;

        case 'first_name':
          $replacements[$original] = $sanitize ? check_plain($candidate->first_name) : $candidate->first_name;
          break;

        case 'last_name':
          $replacements[$original] = $sanitize ? check_plain($candidate->last_name) : $candidate->last_name;
          break;

        case 'election':
          $election = election_load($candidate->election_id);
          $replacements[$original] = $sanitize ? check_plain($election->title) : $election->title;
          break;

      }
    }
    if ($election_tokens = token_find_with_prefix($tokens, 'election')) {
      $election = election_load($candidate->election_id);
      $replacements += token_generate('election', $election_tokens, array('election' => $election), $options);
    }
  }
  return $replacements;
}
