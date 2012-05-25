<?php
/**
 * @file
 * Constants for the Election Candidate module.
 */

// Constants for the {election_candidate} table.
define('ELECTION_CANDIDATE_APPROVED', 1);
define('ELECTION_CANDIDATE_PENDING', 0);
define('ELECTION_CANDIDATE_WITHDRAWN', -1);
define('ELECTION_CANDIDATE_REJECTED', -2);

// Cardinality of the nomination form's "seconder" field (instance of "endorser").
define('ELECTION_CANDIDATE_MAX_SECONDERS', 10);
