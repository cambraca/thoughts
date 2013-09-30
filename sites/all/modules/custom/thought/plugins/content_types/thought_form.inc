<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Thought add/edit form'),
//  'icon' => 'icon_search.png',
  'description' => t('A form for adding or editing thoughts.'),
  'required context' => array(
    new ctools_context_optional(t('Current node (for editing)'), 'entity:node'),
    new ctools_context_optional(t('Parent node'), 'entity:node'),
  ),
  'category' => t('Thoughts'),
  'defaults' => array(
    'node' => NULL,
    'parent' => NULL,
  ),
);

/**
 * Render the custom content type.
 */
function thought_thought_form_content_type_render($subtype, $conf, $panel_args, $context) {
//  dpm($context);
  $block = new stdClass();
  $block->module  = 'thought';
  $block->delta   = 'form';
  $block->title   = '';

  $args = array(
    'current' => $context[0]->data,
    'parent' => $context[1]->data,
  );
  
  $form_state = array(
    'build_info' => array(
      'args' => $args,
    ),
  );

  $block->content = drupal_build_form('thought_form', $form_state);

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function thought_thought_form_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * Submit handler for search form.
 */
//function thought_thought_form_content_type_edit_form_submit($form, &$form_state) {
//  // Copy everything from our defaults.
//  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
//    $form_state['conf'][$key] = $form_state['values'][$key];
//  }
//}

/**
 * Returns the administrative title for a type.
 */
//function thought_thought_form_content_type_admin_title($subtype, $conf, $context) {
//  return t('Thought form');
//}
