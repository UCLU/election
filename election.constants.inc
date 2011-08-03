<?php
/**
 * @file
 * Election module constants.
 */

// Constants for the {election} table.
define('ELECTION_TYPE_REFERENDUM', 'referendum');
define('ELECTION_TYPE_STV', 'stv');
define('ELECTION_STATUS_CLOSED', -1);
define('ELECTION_STATUS_SCHEDULED', 0);
define('ELECTION_STATUS_OPEN', 1);

// Constants for the {election_post} table.
define('ELECTION_POST_STATUS_CLOSED', -2);
define('ELECTION_POST_STATUS_INHERIT', 2);

// Constants for the {election_ballot} table.
define('ELECTION_ANSWER_NO', -1);
define('ELECTION_ANSWER_ABSTAIN', 0);
define('ELECTION_ANSWER_YES', 1);

// Constants for the {election_endorser} table.
define('ELECTION_ENDORSER_TYPE_PROPOSER', 1);
define('ELECTION_ENDORSER_TYPE_SECONDER', 2);
