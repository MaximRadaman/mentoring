<?php
/**
 * @file
 * mentoring_tweet_entity.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function mentoring_tweet_entity_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer mentoring tweets'.
  $permissions['administer mentoring tweets'] = array(
    'name' => 'administer mentoring tweets',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'mentoring_tweet',
  );

  return $permissions;
}
