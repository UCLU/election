<?php
/**
 * @file
 * Class extending DrooPHPCount (for running an STV count on an election post).
 *
 * @todo add equal rankings
 */

class election_stv_droophp_count extends DrooPHP_Count {

  /**
   * Overrides parent::__construct().
   */
  public function __construct(stdClass $post, $options = array()) {

    $this->loadOptions($options);

    $dp_election = new DrooPHP_Election;

    $dp_election->title = $post->title;
    $dp_election->source = $post->election->title;

    $dp_election->num_seats = $post->settings['vacancy_count'];

    $candidates = election_candidate_load_by_post($post, array(ELECTION_CANDIDATE_APPROVED, ELECTION_CANDIDATE_WITHDRAWN), 'last_name, first_name');

    $dp_election->num_candidates = count($candidates);

    foreach ($candidates as $candidate) {
      $name = $candidate->first_name . ' ' . $candidate->last_name;
      $dp_candidate = new DrooPHP_Candidate($name, ($candidate->cstatus == ELECTION_CANDIDATE_WITHDRAWN));
      $dp_candidate->cid = $candidate->candidate_id;
      $dp_election->candidates[$candidate->candidate_id] = $dp_candidate;
    }

    if ($post->settings['use_ron']) {
      $dp_candidate_ron = new DrooPHP_Candidate(t('RON (Re-Open Nominations)'));
      $dp_candidate_ron->cid = 'ron';
      $dp_election->candidates['ron'] = $dp_candidate_ron;
    }

    $result = db_query(
      'SELECT ballot_id, GROUP_CONCAT(IF(ron = 1, \'ron\', IF(abstain = 1, NULL, candidate_id)) ORDER BY RANK SEPARATOR \' \') AS ballot FROM {election_vote} WHERE post_id = :pid GROUP BY ballot_id',
      array(':pid' => $post->post_id)
    );

    /*
     * Each row in $result is an object with the following properties:
     *    vote_id    The unique ID of the vote.
     *    ballot     A string (e.g. '10 15 6 23') of candidate IDs, in order of preference and separated by spaces.
    */
    foreach ($result as $row) {
      $dp_election->num_ballots++;
      // If the string $row->ballot is empty, it indicates an abstention: increment invalid ballot count and move on.
      if (!strlen($row->ballot)) {
        $dp_election->num_invalid_ballots++;
        continue;
      }
      else {
        $dp_election->num_valid_ballots++;
        // The string $key means that identical ballots can simply have their ->value property incremented (saves memory).
        $key = $row->ballot;
        if (isset($dp_election->ballots[$key])) {
          $dp_election->ballots[$key]->value++;
        }
        else {
          $ranking = explode(' ', $row->ballot);
          $ranking = array_combine(range(1, count($ranking)), $ranking);
          // $ranking is an array of candidate IDs, keyed by integers representing preferences.
          $dp_election->ballots[$key] = new DrooPHP_Ballot($ranking);
        }
      }
    }

    $this->election = $dp_election;

  }

}
