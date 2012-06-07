<?php
/**
 * @file
 * Contains a field handler for displaying the user's eligibility to vote.
 */

class views_handler_field_election_post_eligibility extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->field = 'vstatus_inheritance';
    $this->real_field = 'vstatus_inheritance';
    $this->additional_fields['post_id'] = 'post_id';
    $this->additional_fields['vstatus'] = array('table' => 'election', 'field' => 'vstatus');
    $this->additional_fields['vopen_time'] = array('table' => 'election', 'field' => 'vopen_time');
    $this->additional_fields['vclose_time'] = array('table' => 'election', 'field' => 'vclose_time');
    $this->additional_fields['election_type'] = array('table' => 'election', 'field' => 'type');
  }

  function render($values) {
    $vstatus_inheritance = $this->get_value($values);
    $election_type = $this->get_value($values, 'election_type');
    $post_name = _election_get_posts_name($election_type);
    $vstatus = $this->get_value($values, 'vstatus');
    $vopen_time = $this->get_value($values, 'vopen_time');
    $vclose_time = $this->get_value($values, 'vclose_time');
    if ($vstatus_inheritance == ELECTION_POST_STATUS_CLOSED) {
      return t('Voting is currently closed for this @post_name.', array('@post_name' => $post_name));
    }
    elseif (!_election_is_open($vstatus, $vopen_time, $vclose_time)) {
      return t('Voting is currently closed.');
    }
    // The post now needs to be loaded in full for further checks.
    $post = election_post_load($this->get_value($values, 'post_id'));
    if (!_election_post_has_approved_candidates($post)) {
      return t('There are currently no approved candidates standing for this @post_name.', array('@post_name' => $post_name));
    }
    elseif (election_post_access('vote', $post)) {
      if (election_vote_check_user_previously_voted($post)) {
        return t('You have already voted for this @post_name.', array('@post_name' => $post_name));
      }
      return t('You can <a href="@vote_link">vote</a> for this @post_name.', array('@post_name' => $post_name, '@vote_link' => url(election_post_uri_path($post) . '/vote')));
    }
    return t('You are not eligible to vote for this @post_name.', array('@post_name' => $post_name));
  }

}
