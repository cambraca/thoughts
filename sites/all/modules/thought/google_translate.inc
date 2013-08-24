<?php

function thought_google_translate_detect_language($text) {
  $url = 'https://www.googleapis.com/language/translate/v2/detect?key=AIzaSyBWWVCY23dk1gHB2fbf6qBtK0DfWShVf9M&q=';
  $url .= urlencode(substr(strip_tags($text), 0, 400));
  
  $ch = curl_init($url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  $response = curl_exec($ch);
  curl_close($ch);
  
  $langs = language_list();
  
  if ($response) {
    $json = json_decode($response);
    if (isset($json->data->detections[0][0]->language) && array_key_exists($json->data->detections[0][0]->language, $langs))
      return $json->data->detections[0][0]->language;
  }
  
  return key($langs);
}