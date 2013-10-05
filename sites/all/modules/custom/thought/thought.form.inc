<?php

function thought_form($form, &$form_state) {
  $current = isset($form_state['build_info']['args']['current']) ? $form_state['build_info']['args']['current'] : NULL;
//  $parent = isset($form_state['build_info']['args']['parent']) ? $form_state['build_info']['args']['parent'] : NULL;
  
  $has_source_value = $current && $current->field_source[LANGUAGE_NONE][0]['value'];
  $category_default_value = NULL;
  if ($current && isset($current->field_category[LANGUAGE_NONE][0]['tid'])) {
    $category_term = taxonomy_term_load($current->field_category[LANGUAGE_NONE][0]['tid']);
    if ($category_term)
      $category_default_value = $category_term->name;
  }
  
  if ($current) {
    //only give some time for users to edit thoughts, after that threshold they shouldn't be able to
    if ((REQUEST_TIME - $current->created) > THOUGHT_EDIT_THRESHOLD) {
      drupal_set_message(t('You can only edit Thoughts created in the last ').format_interval(THOUGHT_EDIT_THRESHOLD, 1), 'error');
      drupal_goto('node/'.$current->nid);
    }
  }
  
  $form = array(
    '#theme' => 'thought_form_theme',
    'content' => array(
      '#type' => 'textarea',
      '#attributes' => array('placeholder' => t('New thought')),
      '#resizable' => FALSE,
      '#theme_wrappers' => NULL,
      '#rows' => NULL,
      '#cols' => NULL,
      '#required' => TRUE,
      '#default_value' => $current ? $current->body[LANGUAGE_NONE][0]['value'] : NULL,
    ),
    'source' => array(
      '#type' => 'textfield',
      '#attributes' => array('placeholder' => t('Author, URL, or any other source for this thought')),
      '#theme_wrappers' => NULL,
      '#maxlength' => NULL,
      '#size' => NULL,
      '#default_value' => $current ? $current->field_source[LANGUAGE_NONE][0]['value'] : NULL,
      '#prefix' => !$has_source_value ? '<div class="source-wrapper">' : NULL,
      '#suffix' => !$has_source_value ? '</div>' : NULL,
    ),
    'add-source' => array(
      '#type' => 'button',
      '#value' => t('Add source'),
      '#attributes' => array('class' => array('add-source')),
    ),
    'category' => array(
      '#type' => 'textfield',
      '#attributes' => array('placeholder' => t('category')),
      '#theme_wrappers' => NULL,
      '#maxlength' => 30,
      '#size' => NULL,
      '#default_value' => $category_default_value,
    ),
    'public' => array(
      '#type' => 'checkbox',
      '#title' => t('Public'),
      '#attributes' => array('title' => t('Public thoughts are accessible to anyone. It is encouraged to share as many thoughts as possible in order to increase the chance of making unexpected connections.')),
      '#default_value' => $current ? $current->field_public[LANGUAGE_NONE][0]['value'] : 1,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => $current ? t('Save changes') : t('Save new thought'),
      '#attributes' => array('class' => array('submit-button')),
    ),
  );
  
//  dpm($current);
  
  if ($has_source_value)
    unset($form['add-source']);
  
  return $form;
}

function thought_form_submit($form, &$form_state) {
  $current = isset($form_state['build_info']['args']['current']) ? $form_state['build_info']['args']['current'] : NULL;
  $parent_node = isset($form_state['build_info']['args']['parent']) ? $form_state['build_info']['args']['parent'] : NULL;
  
//  dpm(array('current' => $current, 'parent' => $parent_node));

  $values = $form_state['values'];
//  dpm($values);
  
  $content = trim($values['content']);
  $category = strtolower(trim(strip_tags($values['category'])));
  $public = $values['public'];
  $parent = $parent_node ? $parent_node->nid : NULL;
  $source = trim($_POST['source']);
  
  global $user;
  
  $node = $current ? $current : (object) array(
    'type' => 'thought',
    'uid' => $user->uid,
  );
  
  $node->body = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $content,
        'format' => 'filtered_html',
      )
    )
  );
  
  $node->field_public = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $public,
      )
    )
  );
  
  $node->field_source = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $source,
        'format' => 'source',
      )
    )
  );
  $node->field_parent = $parent ? array(
    LANGUAGE_NONE => array(
      array(
        'target_id' => $parent,
      )
    )
  ) : NULL;
  
  $category_tid = NULL;
  if ($category) {
    $vocabulary = taxonomy_vocabulary_machine_name_load('categories');
    $category_tid = thought_taxonomy_term_check_term($category, $vocabulary->vid);
  }
  $node->field_category = $category_tid ? array(
    LANGUAGE_NONE => array(
      array(
        'vid' => $vocabulary->vid,
        'tid' => $category_tid,
      )
    )
  ) : NULL;
  
  node_object_prepare($node);
  node_save($node);
  
//  drupal_goto('latest');
  if (!$form_state['redirect'])
    $form_state['redirect'] = 'latest';
}

function thought_form_comment_form_alter(&$form) {
  $form['#theme'] = 'thought_comment_form_theme';
  $form['author']['#type'] = 'hidden';
  $form['comment_body'][LANGUAGE_NONE][0]['#title_display'] = 'invisible';
  $form['comment_body'][LANGUAGE_NONE][0]['#attributes'] = array('placeholder' => 'Add comment');
  $form['comment_body']['#after_build'][] = '_thoughts_customize_comment_form';
  unset($form['actions']['preview']);
}

function _thoughts_customize_comment_form(&$form) {
//  echo '<pre>'; print_r($form); exit;
//  unset($form[LANGUAGE_NONE][0]['format']['guidelines']);
  $form[LANGUAGE_NONE][0]['format']['#access'] = FALSE;
  return $form;
}