<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Defines election types.
 *
 * Default election types are provided by the election_stv and
 * election_referendum sub-modules. Other types can be provided using this hook.
 * Information about existing election types is loaded via the election_types()
 * function.
 *
 * @return
 *   An array of election type arrays, keyed by a unique machine name. Election
 *   type arrays can include:
 *   - name: (Required) The human-readable name of the election type, sanitized.
 *   - description: A description of the election type, sanitized.
 *   - post name: The name of the election's posts, as a singular noun. See
 *     README.txt. The name for a referendum's posts may be 'motion', while
 *     another election's posts may be referred to as 'role', 'position', or
 *     'title', perhaps.
 *   - post name plural: The plural version of the post name (default: post name
 *     + s).
 *   - has candidates: Whether the election type has candidates. Modules
 *     providing elections that have candidates should depend on the
 *     election_candidate submodule.
 *   - results page: A page callback that shows results for the election (used
 *     by the election_results submodule).
 *   - results download form: A Form API form callback (taking the parameters
 *     $form, &$form_state, $post) that allows users to download the election's
 *     results.
 *
 * @see election_types()
 */
function hook_election_type_info() {
  // @todo remove post settings form, just use hook_form_FORM_ID_alter()
  // @todo remove vote form and save votes, just use hook_form_FORM_ID_alter()
  return array(
    'fptp' => array(
      'name' => t('FPTP election'),
      'description' => t('A first-past-the-post election type, where voters can select a single candidate who wins based on a simple majority with a quorum.'),
      'post name' => t('position'),
      'has candidates' => TRUE,
      'results page' => 'fptpelection_results_page',
      'results download form' => 'fptpelection_results_download_form',
    ),
  );
}
