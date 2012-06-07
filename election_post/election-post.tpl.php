<?php

/**
 * @file
 * Default theme implementation to display an election post.
 */
?>
<div id="election-<?php print $election->election_id; ?>-post-<?php print $post->post_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $election_post_url; ?>"><?php print render($content['title']); ?></a></h2>
  <?php endif; ?>

  <?php if ($view_mode == 'full' && $post->type != 'motion') { ?>

    <p><strong>Number of vacancies: </strong><?php print $vacancy_count; ?></p>

    <?php if ($post->use_ron) { ?>
      <p>RON is included as a candidate for this position.</p>
    <?php } ?>

    <p>
      <strong>Exclusivity: </strong><?php print $exclusivity; ?>
      <br /><span class="tips"><?php print $exclusivity_help; ?></span>
    </p>

  <?php } ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>

</div>
