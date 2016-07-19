<?php
/**
 * @file
 * mentoring_mail_mimemail.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function mentoring_mail_mimemail_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'mail_system';
  $strongarm->value = array(
    'default-system' => 'MimeMailSystem',
    'mimemail' => 'MimeMailSystem',
  );
  $export['mail_system'] = $strongarm;

  return $export;
}
