<?php
/**
 * @file
 * Theme functions for the Election Results module.
 */

/**
 * Theme function to display results totals.
 *
 * Variables will include 'ballots', 'votes', and 'abstentions' (all integers),
 * and 'post' (the election post object).
 */
function theme_election_results_totals($variables) {
  extract($variables, EXTR_SKIP);
  $header = array(t('Ballots'), t('Votes'), t('Abstentions'));
  $rows = array();
  if ($ballots) {
    $rows[] = array(
      number_format($ballots),
      number_format($votes),
      number_format($abstentions),
    );
  }
  return theme('table', array(
    'caption' => t('Totals'),
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No ballots have been cast for this @post.', array(
      '@post' => election_get_post_name($post->election->type),
    )),
  ));
}
