<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Define available voting conditions.
 *
 * @return array
 *   A structured election condition array, keyed by a unique machine name. Each
 *   election condition array can contain:
 *   - name: (Required) The name of the condition.
 *   - callbacks: (Required) An array of function names for the conditions. Each
 *     function takes two arguments - the election post object, and the user
 *     account object - and it must return TRUE if the user passes the
 *     condition.
 *   - description: A description, explaining what the condition does.
 */
function hook_election_condition_info() {
  return array(
    'gmail' => array(
      'name' => t('Have a Gmail account'),
      'callbacks' => array('check_gmail'),
      'description' => t('The user\'s registered email address must end with "gmail.com" or "googlemail.com".'),
    ),
  );
}
