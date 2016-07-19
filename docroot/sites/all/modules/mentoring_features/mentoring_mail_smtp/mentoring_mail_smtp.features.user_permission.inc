<?php
/**
 * @file
 * mentoring_mail_smtp.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function mentoring_mail_smtp_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer smtp module'.
  $permissions['administer smtp module'] = array(
    'name' => 'administer smtp module',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'smtp',
  );

  return $permissions;
}
