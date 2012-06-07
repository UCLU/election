<?php
/**
 * @file
 * Field handler to present a link to the election nomination form.
 */
class views_handler_field_election_link_nomination extends views_handler_field_election_link {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['uid'] = 'uid';
  }

  function render_link($data, $values) {
    $election = new stdClass();
    $election->election_id = $data;
    $election->uid = $this->get_value($values, 'uid');
    $election->type = $this->get_value($values, 'type');
    if ($election->type == 'referendum') {
      return;
    }
    if (!election_access('view', $election)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/nomination";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Nomination form');
    return $text;
  }
}
