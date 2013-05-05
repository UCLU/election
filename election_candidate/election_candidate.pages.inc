<?php
/**
 * @file
 * Page callbacks and form functions for election candidates and nominations
 * (Election module).
 */

 /**
  * Page callback for election-post/%election_post/nomination-form.
  */
function election_candidate_nomination_form_page(stdClass $post) {
  drupal_set_title(t('Nomination form: %post', array(
    '%post' => truncate_utf8($post->title, 50, TRUE, TRUE, 30),
  )), PASS_THROUGH);
  election_post_set_breadcrumb($post->election, $post);
  module_load_include('inc', 'election_candidate', 'election_candidate.forms');
  return drupal_get_form('election_candidate_nomination_form', $post);
}

/**
 * Page callback for election/%election/candidates/list.
 */
function election_candidate_list_page(stdClass $election) {

  $view = views_embed_view('election_candidates', 'embed_candidates_per_election', $election->election_id);

  drupal_set_title(t('Candidates: %election', array('%election' => $election->title)), PASS_THROUGH);
  election_set_breadcrumb($election);

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
      t('Candidate: !full_name', array(
        '!full_name' => theme('election_candidate_full_name', array('candidate' => $candidate)),
      )),
      PASS_THROUGH
    );
    $post = election_post_load($candidate->post_id);
    $election = $post->election;
    election_post_set_breadcrumb($election, $post);
  }

  return election_candidate_view($candidate, $view_mode);

}
