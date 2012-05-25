<?php

/**
 * @file
 * Default theme implementation to display an election candidate's full details.
 */

?>
<div id="election-<?php print $election->election_id; ?>-candidate-details-<?php print $candidate->candidate_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <div class="content"<?php print $content_attributes; ?>>
    <?php print render($content); ?>
  </div>

</div>
