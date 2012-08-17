<?php
/**
 * @file
 * Default Rules for the Election Vote module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function election_vote_default_rules_configuration() {

  $items = array();

  // Email the voter after voting in an election.
  $items['election_vote_email_voter'] = entity_import('rules_config', '{ "election_vote_email_voter" : {
    "LABEL" : "Email user after voting",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_vote" ],
    "ON" : [ "election_vote_vote" ],
    "DO" : [
      { "mail" : {
          "to" : "[account:name] \u003C[account:mail]\u003E",
          "subject" : "Thank you for voting",
          "message" : "You successfully cast a vote, using the system at: [site:url]\r\n\r\n    [election:type:name-ucfirst]: [election:title]\r\n    [election:type:post-name-ucfirst]: [post:title]\r\n    Time of vote: [site:current-date:long]\r\n\r\n(for security reasons further details are not displayed)\r\n\r\nIf you have any queries relating to this information, please contact us.",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  return $items;
}
