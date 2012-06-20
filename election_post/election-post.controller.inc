<?php
/**
 * @file
 * Entity controller for posts (in the Election module).
 *
 * @class
 * ElectionPostController
 * @extends
 * DrupalDefaultEntityController
 */

class ElectionPostController extends ElectionDefaultController {

  /**
   * Overrides parent::create().
   */
  public function create(array $values = array()) {
    $default_values = array(
      'post_id' => '',
      'election_id' => '',
      'type' => '',
      'title' => '',
      'rids' => array(),
      'vstatus_inheritance' => ELECTION_POST_STATUS_INHERIT,
      'nstatus_inheritance' => ELECTION_POST_STATUS_INHERIT,
      'require_manifesto' => 1,
      'require_phone' => 0,
      'require_proposer' => 1,
      'require_photo' => 0,
      'vacancy_count' => 1,
      'exclusive' => 1,
      'use_ron' => 1,
    );
    // Overwrite default values with supplied values.
    $values = array_merge($default_values, $values);
    return parent::create($values);
  }

}
