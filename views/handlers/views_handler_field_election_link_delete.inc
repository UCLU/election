<?php
/**
 * @file
 * Field handler to present a link to delete an election.
 */
class views_handler_field_election_link_delete extends views_handler_field_election_link {
  public function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }

  public function render_link($data, $values) {
    // ensure user has access to delete this election.
    $election = new stdClass();
    $election = election_load($this->get_value($values, 'election_id'));
    $election->election_id = $this->get_value($values, 'election_id');
    $election->type = $this->get_value($values, 'type');
    if (!election_access('delete', $election)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = election_uri_path($election) . '/delete';
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Delete');
    return $text;
  }
}
