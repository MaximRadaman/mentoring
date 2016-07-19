<?php
/**
 * @file
 * mentoring_mail_smtp.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function mentoring_mail_smtp_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
