<?php
/**
 * @file
 * Field handler to present a link to an election's candidates. This depends on
 * the election_candidate module.
 */
class views_handler_field_election_link_candidates extends views_handler_field_election_link {

  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }

  function render_link($data, $values) {
    $election = new stdClass();
    $election->election_id = $this->get_value($values, 'election_id');
    $election->type = $this->get_value($values, 'type');

    // Exit if the election type doesn't support candidates.
    $type_info = _election_type_get_info($election->type);
    if (empty($type_info['has candidates'])) {
      return;
    }

    // exit if the election_candidate module is not enabled.
    if (!module_exists('election_candidate')) {
      return;
    }

    // ensure user has access to view this election.
    if (!entity_access('view', 'election', $election)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/candidates";

    $text = !empty($this->options['text']) ? $this->options['text'] : t('View candidates');
    return $text;
  }
}
