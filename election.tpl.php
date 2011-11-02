<?php

/**
 * @file
 * Default theme implementation to display an election.
 *
 * Available variables:
 * - $title: the (sanitized) title of the election.
 * - $content: An array of election items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The election author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of election author, output from theme_username().
 * - $election_url: Direct url of the current election.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_election().
 *
 * Other variables:
 * - $election: Full election object. Contains data that may not be safe.
 * - $type: Election type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the election.
 * - $uid: User ID of the election author.
 * - $created: Time the election was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the election. Increments each time it's output.
 *
 * Election status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the election.
 * - $readmore: Flags true if the teaser content of the election cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the election a corresponding
 * variable is defined, e.g. $election->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $election->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_election()
 * @see template_process()
 */
?>
<div id="election-<?php print $election->election_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php print $user_picture; ?>

  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $election_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>

  <?php if ($display_submitted): ?>
    <div class="submitted">
      <?php print $submitted; ?>
    </div>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>

  <dl>
    <dt>Voting status</dt>
    <dd><?php print $vstatus; ?></dd>
    <dt>Nominations status</dt>
    <dd><?php print $nstatus; ?></dd>
  </dl>

</div>
