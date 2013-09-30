<?php // dpm($form); ?>

<!--<input type="hidden" name="parent" value="%node:nid" />-->
<article class="node-thought new">
  <header>
    <div class="author"><?= t('me') ?></div>
    <nav class="toolbar">
      <?= render($form['public']) ?><?= render($form['add-source']) ?><?= render($form['submit']) ?>
    </nav>
  </header>
  <div class="content"><div class="wrapper">
    <?= render($form['content']) ?>
    <?= render($form['source']) ?>
  </div></div>
  <footer>
    <div class="field-name-field-category">
      <?= render($form['category']) ?>
    </div>
  </footer>
</article>

<?= drupal_render_children($form) ?>