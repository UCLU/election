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

  public function create($election) {

    $type = _election_type_load($election->type);
    $post_type = $type->{'post key'};

    return (object) array(
      'post_id' => '',
      'election_id' => $election->election_id,
      'title' => '',
      'description' => '',
      'type' => $post_type,
      'vstatus' => ELECTION_POST_STATUS_INHERIT,
      'nstatus' => ELECTION_POST_STATUS_INHERIT,
    );

  }

  public function save($post) {

    $transaction = db_transaction();

    try {

      global $user;

      // Determine whether the post is new.
      $post->is_new = empty($post->post_id);

      // Set timestamp fields.
      if (empty($post->created)) {
        $post->created = REQUEST_TIME;
      }

      $post->changed = REQUEST_TIME;
      $update_election_post = TRUE;

      field_attach_presave('election_post', $post);

      if ($post->is_new) {
        drupal_write_record('election_post', $post);
        $op = 'insert';
      }
      else {
        drupal_write_record('election_post', $post, 'post_id');
        $op = 'update';
      }

      $function = 'field_attach_' . $op;
      $function('election_post', $post);

      module_invoke_all('entity_' . $op, $post, 'election_post');

      unset($post->is_new);

      // Skip the slave server for this user temporarily.
      db_ignore_slave();

      return $post;

    }
    catch (Exception $e) {

      $transaction->rollback();
      watchdog_exception('election_post', $e, NULL, WATCHDOG_ERROR);
      return FALSE;

    }

  }

  public function delete($ids) {

    if (!empty($ids)) {
      $posts = $this->load($ids, array());
      $transaction = db_transaction();

      try {
        db_delete('election_post')
          ->condition('post_id', $ids, 'IN')
          ->execute();
        foreach ($posts as $post_id => $post) {
          field_attach_delete('election_post', $post);
        }
        db_ignore_slave();
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('election', $e, NULL, WATCHDOG_ERROR);
        return FALSE;
      }

// @todo this doesn't make sense
//      module_invoke_all('entity_delete', $posts, 'election_post');

      cache_clear_all();
      $this->resetCache();

    }

    return TRUE;

  }

}