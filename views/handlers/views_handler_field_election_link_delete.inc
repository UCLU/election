<?php
/**
 * Field handler to present a link to delete a election.
 */
class views_handler_field_election_link_delete extends views_handler_field_election_link {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['uid'] = 'uid';
  }

  function render_link($data, $values) {
    // ensure user has access to edit this election.
    $election = new stdClass();
    $election->election_id = $this->get_value($values, 'election_id');
    $election->uid = $this->get_value($values, 'uid');
    $election->type = $this->get_value($values, 'type');
    if (!entity_access('delete', 'election', $election)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
