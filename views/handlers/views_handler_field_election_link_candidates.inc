<?php
/**
 * @file
 * Field handler to present a link to the election candidates list.
 */
class views_handler_field_election_link_candidates extends views_handler_field_election_link {
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
    if ($election->type == 'referendum') {
      return;
    }
    if (!entity_access('edit candidates', 'election', $election)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/candidates";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit candidates');
    return $text;
  }
}
