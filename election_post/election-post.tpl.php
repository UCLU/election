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

  <div class="pseudo-field post-election">
    <span class="label">Election: </span><span class="item"><?php print $election_link; ?></span>
  </div>

  <div class="pseudo-field post-eligibility">
    <span class="label">Your eligibility: </span><span class="item"><?php print $eligibility; ?></span>
  </div>

  <?php if ($view_mode == 'full' && isset($settings['vacancy_count'])) { ?>

    <div class="pseudo-field vacancy_count">
      <span class="label">Number of vacancies: </span><span class="item"><?php print $settings['vacancy_count']; ?></span>
    </div>

  <?php } ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>

</div>
