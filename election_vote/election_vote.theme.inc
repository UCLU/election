<?php
/**
 * @file
 * Theme and preprocess functions for the Election Vote module.
 */

/**
 * Preprocess function for the voting confirmation page.
 */
function template_preprocess_election_vote_confirm(&$variables) {

  $variables['confirm'] = array(
    '#markup' => t('This action cannot be undone.'),
  );

}

/**
 * Theme a confirmation page for a vote.
 */
function theme_election_vote_confirm($variables) {

  $confirm = $variables['confirm'];

  return render($confirm);

}
