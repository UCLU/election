<?php
/**
 * @file
 * Theme and preprocess functions for the Election Vote module.
 */

/**
 * Preprocess function for the voting confirmation page.
 */
function template_preprocess_election_vote_confirm(&$variables) {

  $form_state = $variables['form_state'];
  $post = $form_state['post'];
  $election = $form_state['election'];

  if ($election->type == 'stv') {
    $candidate_rankings = $form_state['candidate_rankings'];
    $original_rankings = $form_state['original_rankings'];

    $rank_changes = $form_state['rank_changes'];
    $ranking_changed = count($rank_changes) ? TRUE : FALSE;
    $variables['ranking_changed'] = $ranking_changed;

    $header = array(t('Candidate'));
    foreach ($candidate_rankings as $candidate_id => $rank) {
      if ($candidate_id == 'RON') {
        $candidate_item = t('RON (Re-Open Nominations)');
      }
      else {
        $candidate = $form_state['candidates'][$candidate_id];
        $candidate_item = theme('election_candidate_full_name', array('candidate' => $candidate));
      }
      $rows[$candidate_id][] = $candidate_item;
    }

    // Add an intermediate column if the user's input had to be changed to produce a consecutive ranking.
    if ($ranking_changed) {
      $header[] = t('Your input');
      foreach ($original_rankings as $candidate_id => $rank) {
        $rows[$candidate_id][] = $rank == 'NONE' ? t('-') : _election_vote_get_ordinal($rank);
      }
    }

    $header[] = t('Your vote') . ($ranking_changed ? '<sup>*</sup>' : '');
    foreach ($candidate_rankings as $candidate_id => $rank) {
      if ($candidate_id == 'RON') {
        $candidate_item = t('RON (Re-Open Nominations)');
      }
      else {
        $candidate = $form_state['candidates'][$candidate_id];
        $candidate_item = theme('election_candidate_full_name', array('candidate' => $candidate));
      }
      $rows[$candidate_id][] = $rank == 'NONE' ? t('-') : _election_vote_get_ordinal($rank);
    }

    $variables['rankings'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

    return;
  }

  if ($election->type == 'referendum') {
    $variables['answer'] = $form_state['values']['answer'];
    return;
  }

}

/**
 * Theme a confirmation page for a vote.
 */
function theme_election_vote_confirm($variables) {
  $form_state = $variables['form_state'];

  $post = $form_state['post'];
  $election = $form_state['election'];

  if ($election->type == 'stv') {

    $output = render($variables['rankings']);

    if ($variables['ranking_changed']) {
      $output .= '<p><sup>*</sup>' . t('Your input is converted so that the candidates are ranked consecutively.') . '</p>';
    }

    return $output;

  }

  if ($election->type == 'referendum') {

    $answer = t('Your answer: %answer', array('%answer' => _election_post_get_answer_text($post, $variables['answer'])));

    return '<dl><dt>' . check_plain($post->title) . '</dt><dd>' . $answer . '</dd></dl>';

  }

}

