<?php // dpm($form, 'comment form'); ?>

<article class="comment new">
  <header>
    <?= render($form['actions']) ?>
    <div class="author">me</div>
  </header>
  <?= render($form['comment_body']) ?>
  
</article>

<?= drupal_render_children($form) ?>