<?php
/**
 * @file
 * Default Rules for the Election Vote module.
 */

/**
 * Implements hook_default_rules_configuration()
 */
function election_vote_default_rules_configuration() {

  $items = array();

  // Email the voter after voting in an STV election.
  $items['election_email_voter_stv'] = entity_import('rules_config', '{ "election_email_voter_stv" : {
    "LABEL" : "Email user after voting (STV elections)",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_vote" ],
    "ON" : [ "election_vote_vote" ],
    "IF" : [ { "data_is" : { "data" : [ "election:type" ], "value" : "stv" } } ],
    "DO" : [
      { "mail" : {
          "to" : "[account:name] \u003C[account:mail]\u003E",
          "subject" : "Thank you for voting",
          "message" : "You successfully cast a vote, using the online elections system at: [site:url]\r\n\r\n    Election: [election:title]\r\n    Position: [post:title]\r\n\r\n(for security reasons further details are not displayed)\r\n\r\nIf you have any queries relating to this information, please contact us.",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  // Email the voter after voting in a referendum.
  $items['election_email_voter_referendum'] = entity_import('rules_config', '{ "election_email_voter_referendum" : {
    "LABEL" : "Email user after voting (Referendums)",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_vote" ],
    "ON" : [ "election_vote_vote" ],
    "IF" : [
      { "data_is" : { "data" : [ "election:type" ], "value" : "referendum" } }
    ],
    "DO" : [
      { "mail" : {
          "to" : "[account:name] \u003C[account:mail]\u003E",
          "subject" : "Thank you for voting",
          "message" : "You successfully cast a referendum vote, using the system at: [site:url]\r\n\r\n    Referendum: [election:title]\r\n    Motion: [post:title]\r\n\r\n(for security reasons further details are not displayed)\r\n\r\nIf you have any queries relating to this information, please contact us.",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  return $items;
}

