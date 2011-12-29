<?php

/**
 * @file
 * Default theme implementation to display an election candidate.
 */

?>
<div id="election-<?php print $election->election_id; ?>-candidate-<?php print $candidate->candidate_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $url; ?>"><?php print check_plain($name); ?></a></h2>
  <?php endif; ?>

  <dl>
    <dt>Standing for</dt>
    <dd><?php print $post_link; ?></dd>
    <dt>Election</dt>
    <dd><?php print $election_link ?></dd>
  </dl>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>


  <?php if ($details): ?>
    <h3>Private details</h3>
    <p class="tips">These are only visible to administrators and to the candidates themselves.</p>
    <dl>
      <dt>Email address</dt>
      <dd><?php print check_plain($candidate->mail); ?></dd>
      <dt>Phone number</dt>
      <dd><?php print check_plain($candidate->phone); ?></dd>
    </dl>
  <?php endif; ?>

</div>
