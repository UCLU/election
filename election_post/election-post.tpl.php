<?php
/**
 * @file
 * Default theme implementation to display an election post.
 */
?>
<div id="election-<?php print $election->election_id; ?>-post-<?php print $post->post_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $post_url; ?>"><?php print render($content['title']); ?></a></h2>
  <?php endif; ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>

</div>
