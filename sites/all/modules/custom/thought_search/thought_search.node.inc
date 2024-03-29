<?php

/**
 * Implements hook_node_insert().
 */
function thought_search_node_insert($node) {
  thought_search_node_update($node);
}

/**
 * Implements hook_node_presave().
 */
function thought_search_node_presave($node) {
  if ($node->type != 'thought' || !isset($node->nid) || (isset($node->nid) && !$node->nid) || !isset($node->original))
    return;

  _thought_search_store_node_data($node->original, TRUE);
}

/**
 * Implements hook_node_update().
 */
function thought_search_node_update($node) {
   _thought_search_store_node_data($node);
}

function _thought_search_store_node_data($node, $substract = FALSE) {
  if ($node->type != 'thought')
    return;

  //is this node marked as processed?
  if (!db_select('thought_processed_nodes', 'pn')->fields('pn')->condition('pn.nid', $node->nid)->execute()->fetchField()) {
    if ($substract)
      //we hadn't processed this node, skip it
      return;
    else
      //mark this node as processed
      db_insert('thought_processed_nodes')->fields(array('nid' => $node->nid))->execute();
  }

  foreach (array_keys($node->body) as $lang) {
    //load an ordered list of words, stemmed and without stopwords
    $content = trim(strip_tags($node->body[$lang][0]['value']));
    $words = languageutil_get_words_from_text($content, $node->language);
    
    //store word counts
    foreach (array_count_values($words) as $word => $count) {
      if ($substract)
        db_update('thought_word_count')
          ->expression('rank', 'rank - :rank', array(':rank' => $count))
          ->condition('language', $node->language)
          ->condition('word', $word)
          ->execute();
      else
        db_merge('thought_word_count')
          ->key(array('language' => $node->language, 'word' => $word))
          ->fields(array(
              'rank' => $count,
          ))
          ->expression('rank', 'rank + :rank', array(':rank' => $count))
          ->execute();
    }
    
    //store word associations
    $relations = _thought_search_calculate_word_assoc($words);
    foreach ($relations as $pair => $weight) {
      list($word1, $word2) = explode(' ', $pair);
      if ($substract) {
        //first prevent division by zero
        db_delete('thought_word_assoc')
          ->condition('language', $node->language)
          ->condition('word1', $word1)
          ->condition('word2', $word2)
          ->condition('count', 1)
          ->execute();
        
        db_update('thought_word_assoc')
          ->fields(array(
              'count' => 1,
              'weight' => $weight,
              'sort' => $weight,
          ))
          ->expression('weight', '((weight * count) - :weight) / (count - 1)', array(':weight' => $weight))
          ->expression('count', 'count - 1')
          ->expression('sort', 'weight * count')
          ->condition('language', $node->language)
          ->condition('word1', $word1)
          ->condition('word2', $word2)
          ->execute();
      } else
        db_merge('thought_word_assoc')
          ->key(array('language' => $node->language, 'word1' => $word1, 'word2' => $word2))
          ->fields(array(
              'count' => 1,
              'weight' => $weight,
              'sort' => $weight,
          ))
          ->expression('weight', '((weight * count) + :weight) / (count + 1)', array(':weight' => $weight))
          ->expression('count', 'count + 1')
          ->expression('sort', 'weight * count')
          ->execute();
    }
  }
  
  if ($substract) {
    //unmark this node as processed
    db_delete('thought_processed_nodes')->condition('nid', $node->nid)->execute();
    
    //cleanup "zero count" records
    db_delete('thought_word_count')->condition('rank', 0)->execute();
  }
}

/**
 * Implements hook_node_delete().
 */
function thought_search_node_delete($node) {
   _thought_search_store_node_data($node, TRUE);
}

/**
 * Calculate pairs of words and their distance values.
 * The returned array contains a copy of each record, for example:
 *   array(
 *     'hello good' => .8,
 *     'good hello' => .8,
 *     'hello goodbye' => .3,
 *     'goodbye hello' => .3,
 *   )
 * The values of the array are 1/x, x being the distance between the words (1 means they are next to each other).
 * @param array $words an ordered list of words
 * @return array
 */
function _thought_search_calculate_word_assoc($words) {
  $ret = array();
  
  for ($i = 0; $i < count($words) - 1; $i++) {
    for ($j = $i + 1; $j < count($words); $j++) {
      if ($words[$i] == $words[$j])
        continue;
      
      $ret[$words[$i].' '.$words[$j]] = 1 / ($j-$i);
      $ret[$words[$j].' '.$words[$i]] = 1 / ($j-$i);
    }
  }
  
  return $ret;
}
