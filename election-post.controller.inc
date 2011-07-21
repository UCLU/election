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
class ElectionPostController extends DrupalDefaultEntityController {

  public function create() {

    return (object) array(
      'post_id' => '',
      'election_id' => '',
      'title' => '',
      'description' => '',
    );

  }

}