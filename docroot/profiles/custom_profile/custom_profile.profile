<?php
/**
 * @file
 * Enable modules and site configuration for Custom profile.
 */

require_once 'inc/profile.tasks.inc';

/**
 * Implements hook_install_tasks().
 */
function custom_profile_install_tasks() {
  $tasks = [
    'custom_profile_site_theme_form' => [
      'display_name' => st('Choose base site theme'),
      'display' => TRUE,
      'type' => 'form',
    ],
    'node_type_from_theme_settings' => [
      'display_name' => st('Create node type from theme settings'),
      'function' => '_custom_profile_create_node_type',
    ],
  ];
  return $tasks;
}

/**
 * Implements hook_instll_tasks_alter().
 */
function custom_profile_install_tasks_alter(&$tasks, $install_state) {
  // Set Local to be always En.
  $tasks['install_select_locale']['function'] = '_custom_profile_install_select_locale';
}

/**
 * Implements hook_form_FORM_ID_alter()
 *
 * Set default values to install_configure_form form.
 */
function custom_profile_form_install_configure_form_alter(&$form, $form_state) {
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  $form['site_information']['site_mail']['#default_value'] = 'custom_profile@profile.com';
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = 'custom_profile@profile.com';;
  $form['server_settings']['site_default_country']['#default_value'] = 'BY';
}
