<?php

function thought_search_settings_form() {
  $form = array();
  
  drupal_add_css('#thought-search-info td {vertical-align: top;}', array('type' => 'inline'));
  
  $output = '';
  
  $output .= '<table id="thought-search-info"><tr><td>';
  
  $output .= '<h2>'.t('Stats').'</h2>';
  
  $table = array();
  $table['rows'] = array();
  
  $q = db_select('thought_processed_nodes', 'pn');
  $q->addExpression('count(*)', 'count');
  $table['rows'][] = array(
    t('Nodes processed'),
    $q->execute()->fetchField(),
  );

  $q = db_select('node', 'n');
  $q->leftJoin('thought_processed_nodes', 'pn', 'n.nid = pn.nid');
  $q->isNull('pn.nid')
    ->condition('n.type', 'thought');
  $q->addExpression('count(*)', 'count');
  $table['rows'][] = array(
    t('Nodes to be processed'),
    $q->execute()->fetchField(),
  );
  
  $q = db_select('thought_word_count');
  $q->addExpression('count(*)', 'count');
  $table['rows'][] = array(
    t('Words stored'),
    $q->execute()->fetchField(),
  );
  
  $q = db_select('thought_word_assoc');
  $q->addExpression('count(*)', 'count');
  $table['rows'][] = array(
    t('Words associations stored'),
    $q->execute()->fetchField(),
  );
  
  $output .= theme('table', $table);

  $output .= '</td><td>';
  
  $output .= '<h2>'.t('Most common words').'</h2>';
  
  $table = array();
  $table['header'] = array(
    t('Language'),
    t('Word'),
    t('Count'),
  );
  $table['rows'] = array();
  
  $data = db_select('thought_word_count', 'wc')
    ->fields('wc')
    ->orderBy('rank', 'DESC')
    ->range(0, 10)
    ->execute()
    ->fetchAll();
  
  foreach ($data as $row) {
    $table['rows'][] = array($row->language, $row->word, $row->rank);
  }
  
  $output .= theme('table', $table);

  $output .= '</td><td>';

  $output .= '<h2>'.t('Strongest word associations').'</h2>';
  
  $table = array();
  $table['header'] = array(
    t('Language'),
    array('data' => t('Words'), 'colspan' => 2),
  );
  $table['rows'] = array();
  
  $data = db_select('thought_word_assoc', 'wa')
    ->fields('wa', array('word1', 'word2', 'language'))
    ->orderBy('sort', 'DESC')
    ->range(0, 20)
    ->execute()
    ->fetchAll();
  
  $pairs_seen = array();
  foreach ($data as $row) {
    $compare = strcasecmp($row->word1, $row->word2);
    $word1 = $compare < 0 ? $row->word1 : $row->word2;
    $word2 = $compare < 0 ? $row->word2 : $row->word1;
    
    if (in_array($word1.' '.$word2, $pairs_seen))
      continue;
    $pairs_seen[] = $word1.' '.$word2;
    
    $table['rows'][] = array($row->language, $word1, $word2);
    
    if (count($pairs_seen) == 10)
      break;
  }
  
  $output .= theme('table', $table);
  
  $output .= '</td></tr></table>';
  
  $form['#prefix'] = $output;
  
//  $form['type'] = array(
//    '#type' => 'select',
//    '#title' => t('TV Listing content type'),
//    '#options' => node_type_get_names(),
//    '#description' => t('Select the content type used for TV Listings.'),
//    '#default_value' => $type,
//  );
  
  $form['delete-indexes'] = array(
    '#type' => 'submit',
    '#value' => t('Delete all Thought specific indexes')
  );
  
  return $form;
}

function thought_search_settings_form_submit($form, &$form_state) {
  db_delete('thought_processed_nodes')->execute();
  db_delete('thought_word_count')->execute();
  db_delete('thought_word_assoc')->execute();
  
  drupal_set_message(t('All indexes have been deleted. Cron will begin rebuilding them now.'));
}
