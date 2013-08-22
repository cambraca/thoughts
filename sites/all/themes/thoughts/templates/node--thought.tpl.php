<?php
/**
 * @file
 * Returns the HTML for a node.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */
?>
<article class="node-<?php print $node->nid; ?> <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php /*if ($title_prefix || $title_suffix || $display_submitted || $unpublished || !$page && $title): ?>
    <header>
      <?php print render($title_prefix); ?>
      <?php if (!$page && $title): ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php if ($display_submitted): ?>
        <p class="submitted">
          <?php print $user_picture; ?>
          <?php print $submitted; ?>
        </p>
      <?php endif; ?>

      <?php if ($unpublished): ?>
        <mark class="unpublished"><?php print t('Unpublished'); ?></mark>
      <?php endif; ?>
    </header>
  <?php endif;*/ ?>

  <div class="content <?php if (rand(1,2) == 2) echo 'large'; ?>">
    <?php
      hide($content['comments']);
      hide($content['links']);
      
      hide($content['field_category']);
      
      print render($content);
    ?>
  </div>
  
  <footer>
    <time pubdate="<?= $node->created ?>"><?= format_interval(time() - $node->created, 1).t(' ago') ?></time>
    <?php print render($content['field_category']); ?>
  </footer>

  <?php /*print render($content['links']); ?>
  <?php print render($content['comments']);*/ ?>

</article>
