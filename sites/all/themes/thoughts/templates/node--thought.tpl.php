<?php
/**
 * @file
 * Returns the HTML for a node.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728164
 */

$is_main_thought = arg(0) == 'node' && arg(1) == $node->nid;

global $user;
$show_author = $uid != $user->uid;
if ($show_author && arg(0) == 'user' && arg(1) == $uid)
  $show_author = FALSE;

if ($is_main_thought)
  $classes .= ' highlighted';
if (!$show_author)
  $classes .= ' author-hidden';
?>
<article class="node-<?php print $node->nid; ?> <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <header>
    <?php // if ($show_author): ?>
    <div class="author"><?php print $name; ?></div>
    <?php // endif; ?>
    <?php print render($content['field_parent']); ?>
    <nav class="toolbar">
      <?php if ($node->uid == $user->uid && (REQUEST_TIME - $node->created) < THOUGHT_EDIT_THRESHOLD) print l('Edit', 'node/'.$node->nid.'/edit'); ?>
      <?php if (!isset($_GET['elaborate']) || $node->nid != arg(1)) print l('Elaborate', 'node/'.$node->nid, array('query' => array('elaborate' => NULL))); ?>
    </nav>
  </header>
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

  <?php
    $css_size_class = '';
    $body = field_view_field('node', $node, 'body');
    $body_length = strlen(strip_tags($body['#items'][0]['value']));
    if ($body_length > 200)
      $css_size_class = ' large';
    elseif ($body_length < 100)
      $css_size_class = ' small';
  ?>
  <div class="content<?= $css_size_class ?><?php if ($is_main_thought) echo ' expanded'; ?>"
    <?php if (!$is_main_thought) echo 'data-link="'.url('node/'.$node->nid).'"'?>>
    <div class="wrapper">
    <?php
      hide($content['comments']);
      hide($content['links']);
      
      hide($content['field_category']);
      
      print render($content);
    ?>
    </div>
  </div>
  
  <footer>

    <?php if (!$is_main_thought && $body_length > 200): ?>
      <a class="more" href="#" title="See the whole thought">+</a>
    <?php endif; ?>
    
    <?php if ($comment_count && !$is_main_thought): ?>
      <div class="comment_count"><span><?= $comment_count ?></span></div>
    <?php endif; ?>
    
    <time pubdate="<?= date('c', $node->created) ?>"><?= format_interval(time() - $node->created, 1).t(' ago') ?></time>

    <?php print render($content['field_category']); ?>
    
  </footer>

  <?php /*print render($content['links']); ?>
  <?php print render($content['comments']);*/ ?>

</article>
