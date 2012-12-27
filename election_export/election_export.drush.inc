<?php
/**
 * @file
 * Drush commands for the Election Export module.
 */

/**
 * Implements hook_drush_command().
 */
function election_export_drush_command() {
  $items = array();
  $items['election-export'] = array(
    'description' => 'Exports results for an election as a ZIP archive (.zip) containing ballot (.blt) files.',
    'arguments' => array(
      'election_id' => 'The ID of the election (required).',
      'destination' => 'Optional: A writeable destination filename, relative to private://election-export/. Defaults to STDOUT.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'required-arguments' => 1,
    'options' => array(
      'include-ballot-ids' => 'Ballot IDs could help to identify ballots uniquely.',
    ),
    'aliases' => array('eex'),
  );
  $items['election-export-post'] = array(
    'description' => 'Exports results for an election post as a ballot file.',
    'arguments' => array(
      'post_id' => 'The ID of the election post (required).',
      'destination' => 'Optional: A writeable destination filename, relative to private://election-export/. Defaults to STDOUT.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'required-arguments' => 1,
    'options' => array(
      'include-ballot-ids' => 'Ballot IDs could help to identify ballots uniquely.',
    ),
    'aliases' => array('eexp'),
  );
  return $items;
}

/**
 * Export the results for an election.
 *
 * @param int $election_id
 *   The ID of the election.
 * @param string $destination
 *   The destination filename to write to (optional). If unspecified, output
 *   will be printed to stdout. This filename should have the extension '.zip'
 *   and be in a writeable directory. Relative filenames will relate to the
 *   directory 'private://election-export'.
 */
function drush_election_export($election_id, $destination = NULL) {
  $election = election_load($election_id);
  if (!$election) {
    return drush_set_error(dt('No election found with the ID "@eid".', array('@eid' => $election_id)));
  }
  if (empty($election->type_info['export']) || !_election_candidate_check_support($election)) {
    return drush_set_error(dt("This election type doesn't support exporting."));
  }
  if (!empty($destination)) {
    $dirname = dirname($destination);
    if ($dirname == '.') {
      $dirname = 'private://election-export';
      if (!file_exists($dirname)) {
        mkdir($dirname);
      }
      $destination = $dirname . '/' . $destination;
    }
    if ($dirname == $destination) {
      $destination .= '/election-export-' . $election_id . '.zip';
    }
    elseif (file_exists($destination)) {
      return drush_set_error(dt('The file @destination already exists.', array('@destination' => $destination)));
    }
    if (!is_writable($dirname)) {
      return drush_set_error(dt('Directory not writeable: "@dir".', array('@dir' => $dirname)));
    }
    $extension = pathinfo($destination, PATHINFO_EXTENSION);
    if ($extension && $extension !== 'zip') {
      return drush_set_error(dt('The destination extension must be ".zip".'));
    }
  }
  $options = array(
    'include ballot ids' => drush_get_option('include-ballot-ids'),
  );
  // Output to STDOUT.
  if (empty($destination)) {
    $tmp_filename = election_export_election($election, NULL, $options);
    $success = readfile($tmp_filename);
    unlink($tmp_filename);
    return (bool) $success;
  }
  // Output to specified filename.
  else {
    $success = election_export_election($election, $destination, $options);
    return (bool) $success;
  }
}

/**
 * Export the results for an election post.
 *
 * @param int $post_id
 *   The ID of the election post.
 * @param string $destination
 *   The destination filename to write to (optional). If unspecified, output
 *   will be printed to stdout. This filename should be in a writeable
 *   directory. Relative filenames will relate to the directory
 *   'private://election-export'.
 */
function drush_election_export_post($post_id, $destination = NULL) {
  $post = election_post_load($post_id);
  if (!$post) {
    return drush_set_error(dt('No election post found with the ID "@id".', array('@id' => $post_id)));
  }
  $election = $post->election;
  if (empty($election->type_info['export']) || !_election_candidate_check_support($election)) {
    return drush_set_error(dt("This election type doesn't support exporting."));
  }
  if (!empty($destination)) {
    $dirname = dirname($destination);
    if ($dirname == '.') {
      $dirname = 'private://election-export';
      if (!file_exists($dirname)) {
        mkdir($dirname);
      }
      $destination = $dirname . '/' . $destination;
    }
    if ($dirname == $destination) {
      $destination .= '/election-export-' . $election->election_id . '-' . $post_id . '.blt';
    }
    elseif (file_exists($destination)) {
      return drush_set_error(dt('The file @destination already exists.', array('@destination' => $destination)));
    }
    if (!is_writable($dirname)) {
      return drush_set_error(dt('Directory not writeable: "@dir".', array('@dir' => $dirname)));
    }
  }
  else {
    $destination = 'php://stdout';
  }
  $options = array(
    'include ballot ids' => drush_get_option('include-ballot-ids'),
  );
  $success = election_export_post($post, $destination, $options);
  return (bool) $success;
}
