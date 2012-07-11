<?php
/**
 * @file
 * Default Rules for the Election Candidate module.
 */

/**
 * Implements hook_default_rules_configuration()
 */
function election_candidate_default_rules_configuration() {

  $items = array();

  // Email the user who submitted a nomination (the 'nominator').
  $items['election_email_nominator'] = entity_import('rules_config', '{ "election_email_nominator" : {
    "LABEL" : "Email the user who submitted a nomination",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_candidate" ],
    "ON" : [ "election_candidate_nomination_submitted" ],
    "DO" : [
      { "mail" : {
          "to" : "[account:name] \u003C[account:mail]\u003E",
          "subject" : "Thank you for submitting a nomination for [post:title]",
          "message" : "This is an automatic email from [site:name] ([site:url-brief]) to confirm your nomination.\r\n\r\n    Election: [election:title]\r\n        [election:url])\r\n\r\n    Position: [post:title]\r\n        [post:url]\r\n\r\n    Candidate name: [candidate:first-name] [candidate:last-name]\r\n    Candidate email address: [candidate:mail]\r\n",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  // Email the nominee (if different from the nominator).
  $items['election_email_nominee'] = entity_import('rules_config', '{ "election_email_nominee" : {
    "LABEL" : "Email the nominee (if nominee \u0026 nominator email addresses differ)",
    "PLUGIN" : "reaction rule",
    "WEIGHT" : "1",
    "ACTIVE" : false,
    "TAGS" : [ "election" ],
    "REQUIRES" : [ "rules", "election_candidate" ],
    "ON" : [ "election_candidate_nomination_submitted" ],
    "IF" : [
      { "NOT data_is" : { "data" : [ "account:mail" ], "value" : [ "candidate:mail" ] } }
    ],
    "DO" : [
      { "mail" : {
          "to" : "[candidate:first_name] [candidate:last_name] \u003C[candidate:mail]\u003E",
          "subject" : "You have been nominated for [post:title]",
          "message" : "This is an automatic email from [site:name] ([site:url-brief]) to confirm your nomination.\r\n\r\n    Election: [election:title]\r\n        [election:url])\r\n\r\n    Position: [post:title]\r\n        [post:url]\r\n\r\nYou were nominated by the user [account:name] \u003C[account:mail]\u003E.\r\n",
          "language" : [ "" ]
        }
      }
    ]
  }
}');

  return $items;
}

