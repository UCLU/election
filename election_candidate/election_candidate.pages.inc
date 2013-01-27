<?php
/**
 * @file
 * Page callbacks and form functions for election candidates and nominations
 * (Election module).
 */

/**
 * Page callback for election/%election/candidates/list.
 */
function election_candidate_list_page(stdClass $election) {

  $view = views_embed_view('election_candidates', 'embed_candidates_per_election', $election->election_id);

  drupal_set_title(t('Candidates'), PASS_THROUGH);
  drupal_set_breadcrumb(_election_build_breadcrumb($election));

  return array(
    '#markup' => $view,
  );

}

/**
 * Page callback for election-candidate/%election_candidate.
 */
function election_candidate_page_view(stdClass $candidate, $view_mode = 'full') {

  if ($view_mode == 'full' || $view_mode == 'details') {
    drupal_set_title(
      t('Candidate: @full_name', array(
        '@full_name' => theme('election_candidate_full_name', array('candidate' => $candidate)),
      )),
      PASS_THROUGH
    );
    $election = election_load($candidate->election_id);
    drupal_set_breadcrumb(
      _election_build_breadcrumb($election, '_CANDIDATES_')
    );
  }

  return election_candidate_view($candidate, $view_mode);

}
