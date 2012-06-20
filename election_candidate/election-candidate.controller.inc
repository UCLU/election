<?php
/**
 * @file
 * Entity controller for candidates (in the Election module).
 *
 * @class
 * ElectionCandidateController
 * @extends
 * ElectionDefaultController
 */

class ElectionCandidateController extends ElectionDefaultController {

  /**
   * Overrides EntityAPIController::create(), allowing defaults to be defined.
   */
  public function create(array $values = array()) {
    $default_values = array(
      'candidate_id' => '',
      'election_id' => '',
      'post_id' => '',
      'cstatus' => ELECTION_CANDIDATE_PENDING,
      'uid' => '',
      'username' => '',
      'first_name' => '',
      'last_name' => '',
      'phone' => '',
      'mail' => '',
      'summary' => '',
      'manifesto' => '',
      'photo' => '',
      'proposer' => new stdClass(),
      'seconders' => array(),
      'created' => 0,
      'changed' => 0,
    );
    // Overwrite default values with supplied values.
    $values = array_merge($default_values, $values);
    return parent::create($values);
  }

}
