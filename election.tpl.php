<?php
/**
 * @file
 * Default theme implementation to display an election.
 */
?>
<div id="election-<?php print $election->election_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $election_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>

  <?php if ($content): ?>
    <div class="content"<?php print $content_attributes; ?>>
      <?php print render($content); ?>
    </div>
  <?php endif; ?>

  <dl>
    <dt>Voting status</dt>
    <dd><?php print $vstatus; ?></dd>
    <dt>Nominations status</dt>
    <dd><?php print $nstatus; ?></dd>
  </dl>

</div>
