<?php
/**
 * @file
 * Theme and preprocess functions for the Election Statistics module.
 */

/**
 * Theme election statistics.
 */
function theme_election_statistics($variables) {

  $output = render($variables['content']);

  return $output;
}

/**
 * Preprocess function for election statistics.
 */
function template_preprocess_election_statistics(&$variables) {

  $election = $variables['election'];

  $variables['content'] = array();

  $totals = db_query('SELECT COUNT(DISTINCT ballot_id) AS ballots, COUNT(DISTINCT IF(abstain=0,ballot_id,NULL)) AS votes, COUNT(DISTINCT IF(abstain=1, ballot_id, NULL)) AS abstentions FROM {election_vote} WHERE election_id = :eid', array(':eid' => $election->election_id))->fetch();
  $totals->voters = db_query('SELECT COUNT(DISTINCT IF(uid = 0, sid_hash, uid)) AS voters FROM {election_ballot} WHERE election_id = :eid', array(':eid' => $election->election_id))->fetchField();

  $post_name = election_get_post_name($election->type);

  $header = array(
    'voters' => t('Voters'),
    'ballots' => t('Ballots'),
    'votes' => t('Votes'),
    'abstentions' => t('Abstentions'),
  );
  $rows = array();
  $voter_avg = $totals->ballots / $totals->voters;
  $rows[0] = array(
    'voters' => number_format($totals->voters),
    'ballots' => t(
      '@ballots (@avg each)',
      array('@ballots' => number_format($totals->ballots), '@avg' => number_format($voter_avg, 1))
    ),
    'votes' => number_format($totals->votes),
    'abstentions' => number_format($totals->abstentions),
  );

  $election_table = array(
    '#caption' => t('Election totals'),
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $variables['content']['election_table'] = $election_table;

  $posts_table = NULL;
  if ($totals->votes > 0) {
    $header = array(
      'post' => check_plain(drupal_ucfirst($post_name)),
      'voters' => t('Voters'),
      'ballots' => t('Ballots'),
      'votes' => t('Votes'),
      'abstentions' => t('Abstentions'),
    );

    $post_ids = db_query('SELECT post_id FROM {election_post} WHERE election_id = :eid ORDER BY title', array(':eid' => $election->election_id))->fetchCol();

    $posts = election_post_load_multiple($post_ids);

    $posts_totals = db_query(
      'SELECT ev.post_id, COUNT(DISTINCT ev.ballot_id) AS ballots, COUNT(DISTINCT IF(ev.abstain=0,ev.ballot_id,NULL)) AS votes, COUNT(DISTINCT IF(ev.abstain=1,ev.ballot_id,NULL)) AS abstentions, COUNT(DISTINCT IF(eb.uid = 0,eb.sid_hash,eb.uid)) AS voters FROM {election_vote} ev LEFT JOIN {election_ballot} eb USING (ballot_id) WHERE ev.election_id = :eid GROUP BY ev.post_id',
      array(':eid' => $election->election_id)
    )->fetchAllAssoc('post_id');

    $rows = array();
    foreach ($posts as $post_id => $post) {

      $post_totals = isset($posts_totals[$post_id]) ? $posts_totals[$post_id] : FALSE;

      $rows[] = array(
        'post' => l($post->title, election_post_uri_path($post)),
        'voters' => $post_totals ? number_format($post_totals->voters) : 0,
        'ballots' => $post_totals ? number_format($post_totals->ballots) : 0,
        'votes' => $post_totals ? number_format($post_totals->votes) : 0,
        'abstentions' => $post_totals ? number_format($post_totals->abstentions) : 0,
      );

    }

    $posts_table = array(
      '#caption' => t('Totals per @post_name', array('@post_name' => $post_name)),
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

  }

  $variables['content']['posts_table'] = $posts_table;

  $top_agents_table = NULL;
  $top_agents = db_query_range('SELECT agent, COUNT(DISTINCT IF(uid = 0, sid_hash, uid)) AS voters FROM {election_ballot} WHERE election_id = :eid GROUP BY agent ORDER BY COUNT(DISTINCT uid) DESC, COUNT(DISTINCT ballot_id) DESC', 0, 10, array(':eid' => $election->election_id));
  if ($top_agents->rowCount()) {
    $header = array(t('Voters'), t('User agent'));
    $rows = array();
    $browscap = FALSE;
    if (ini_get('browscap')) {
      $browscap = TRUE;
      $header[] = t('Browser');
      $header[] = t('Platform');
    }
    foreach ($top_agents as $record) {
      $row = array(
        number_format($record->voters),
        check_plain(truncate_utf8($record->agent, 60, FALSE, TRUE)),
      );
      if ($browscap) {
        $browser = get_browser($record->agent);
        $row[] = check_plain($browser->parent);
        $row[] = check_plain($browser->platform);
      }
      $rows[] = $row;
    }
    $top_agents_table = array(
      '#caption' => t('Top 10 user agents'),
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
    if (!$browscap) {
      $top_agents_table['#rows'][] = array(
        array(
          'data' => t(
            'Install <code>browscap</code> to display browser and platform details in this table. See the PHP docs for <a href="@get_browser" target="_blank"><code>get_browser()</code></a>.',
            array('@get_browser' => 'http://php.net/manual/en/function.get-browser.php')
          ),
          'colspan' => 3,
        ),
      );
    }
  }
  $variables['content']['top_agents_table'] = $top_agents_table;

  $top_ips_table = NULL;
  $ips = db_query_range('SELECT ip, COUNT(DISTINCT IF(uid = 0, sid_hash, uid)) AS voters FROM {election_ballot} WHERE election_id = :eid GROUP BY ip ORDER BY COUNT(DISTINCT uid) DESC, COUNT(DISTINCT ballot_id) DESC', 0, 10, array(':eid' => $election->election_id));
  if ($ips->rowCount()) {
    $header = array(t('Voters'), t('IP address'));
    $rows = array();
    foreach ($ips as $record) {
      $rows[] = array(
        number_format($record->voters),
        check_plain($record->ip),
      );
    }
    $top_ips_table = array(
      '#caption' => t('Top 10 IP addresses'),
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  $variables['content']['top_ips_table'] = $top_ips_table;

  $variables['totals'] = $totals;


}
