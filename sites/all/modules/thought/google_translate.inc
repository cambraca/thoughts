<?php

function thought_google_translate_detect_language($text) {
  $url = 'https://www.googleapis.com/language/translate/v2/detect?key=AIzaSyBWWVCY23dk1gHB2fbf6qBtK0DfWShVf9M';
   
  $ch = curl_init($url);
  
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, array('q' => strip_tags($text)));
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  
  $response = curl_exec($ch);
  curl_close($ch);
  
  if ($response) {
    $json = json_decode($response);
    if (isset($json->data->detections[0]->language))
      return $json->data->detections[0]->language;
  }
  
  return 'en';
}