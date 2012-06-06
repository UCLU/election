<?php
/**
 * @file
 * Field handler to present a link to an election's nomination form. This depends on
 * the election_candidate module.
 *
 * @todo add option to hide if nominations are closed.
 */
class views_handler_field_election_link_nominate extends views_handler_field_election_link {

  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['nstatus'] = 'nstatus';
    $this->additional_fields['nopen_time'] = 'nopen_time';
    $this->additional_fields['nclose_time'] = 'nclose_time';
  }

  function render_link($data, $values) {
    $election = new stdClass();
    $election->election_id = $this->get_value($values, 'election_id');
    $election->type = $this->get_value($values, 'type');

    // ensure we're not dealing with a referendum.
    if ($election->type == 'referendum') {
      return;
    }

    // exit if the election_candidate module is not enabled.
    if (!module_exists('election_candidate')) {
      return;
    }

    // ensure user has access to view this election.
    if (!election_candidate_access_per_election('nominate', $election)) {
      return;
    }

    // ensure nominations are open for the election.
    $nstatus = $this->get_value($values, 'nstatus');
    $nopen_time = $this->get_value($values, 'nopen_time');
    $nclose_time = $this->get_value($values, 'nclose_time');
    if (!_election_is_open($nstatus, $nopen_time, $nclose_time)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "election/{$election->election_id}/nomination-form";

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Nomination form');
    return $text;
  }
}
