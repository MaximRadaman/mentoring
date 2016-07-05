<?php
/**
 * @file
 * Forms, validations, submitions for Custom profile tasks.
 */

require_once 'helper.inc';

/**
 * Form for custom_profile_site_theme_form install task.
 */
function custom_profile_site_theme_form() {

  $form['base_site_theme'] = [
    '#title' => st('Choose base site theme'),
    '#type' => 'select',
    '#options' => [
      'bootstrap_custom' => st('Bootstrap custom'),
      'bootstrap' => st('Bootstrap'),
      'omega' => st('Omega 4'),
      'zen' => st('Zen'),
    ],
    '#default_value' => 'bootstrap_custom',
    '#require' => TRUE,
    '#description' => st('Sub theme will be created only if custom_profile theme selected(E.x: Custom Bootstrap)'),
  ];
  $form['custom_site_theme'] = [
    '#title' => st('Enter site theme name'),
    '#type' => 'textfield',
    '#size' => 30,
    '#maxlength' => 15,
    '#states' => [
      'visible' => [
        ':input[name="base_site_theme"]' => ['value' => 'bootstrap_custom']
      ],
      'required' => [
        ':input[name="custom_site_theme"]' => ['visible' => TRUE]
      ]
    ]
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => st('Save and continue')
  ];

  return $form;
}

/**
 * Create and enable sub-theme on custom_profile_site_theme_form install task form validation.
 *
 * @param $form
 * @param $form_state
 * @throws \Exception
 */
function custom_profile_site_theme_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['custom_site_theme'])) {
    $custom_theme_machine_name = custom_profile_conf_theme_machine_name($form_state['values']['custom_site_theme']);
    $base_theme_dir = DRUPAL_ROOT . '/profiles/custom_profile/themes/custom/' . $form_state['values']['base_site_theme'];
    $custom_theme_dir = DRUPAL_ROOT . '/sites/all/themes/custom/' . $custom_theme_machine_name;
    switch ($form_state['values']['base_site_theme']) {
      case 'bootstrap_custom':
        try {
          // Create directory and files for new theme.
          if (!is_dir(DRUPAL_ROOT . '/sites/all/themes/custom')) {
            drupal_mkdir(DRUPAL_ROOT . '/sites/all/themes/custom');
          }
          if (!is_dir($custom_theme_dir)) {
            drupal_mkdir($custom_theme_dir);
          }
          custom_profile_rcopy($base_theme_dir, $custom_theme_dir);
          rename($custom_theme_dir . '/bootstrap_custom.info', $custom_theme_dir . '/' . $custom_theme_machine_name . '.info');

          // Set base theme for new theme.
          $info_data = file_get_contents($custom_theme_dir . '/' . $custom_theme_machine_name . '.info');
          $info_data = preg_replace('/^name.*/', 'name = ' . $form_state['values']['custom_site_theme'], $info_data);
          $info_data = preg_replace('/.*base theme.*/', 'base theme = bootstrap_custom', $info_data);
          file_put_contents($custom_theme_dir . '/' . $custom_theme_machine_name . '.info', $info_data);
        }
        catch (Exception $e) {
          form_set_error('form', $e->getMessage());
        }
        break;
    }
  }
}

/**
 * Submit handler for custom_profile_site_theme_form.
 */
function custom_profile_site_theme_form_submit($form, &$form_state) {
  $new_theme = $form_state['values']['base_site_theme'];
  if (!empty($form_state['values']['custom_site_theme'])) {
    $custom_theme_machine_name = custom_profile_conf_theme_machine_name($form_state['values']['custom_site_theme']);
    $new_theme = $custom_theme_machine_name;
  }

  // Enable new theme.
  if ($new_theme != 'bootstrap_custom') {
    theme_enable([$new_theme]);
    variable_set('theme_default', $new_theme);
  }
}

/**
 * Install task install_select_locale function.
 *
 * Set local to always be En.
 */
function _custom_profile_install_select_locale(&$install_state) {
  $install_state['parameters']['locale'] = 'en';
}

/**
 * Install task node_type_from_theme_settings function.
 */
function _custom_profile_create_node_type(&$install_state) {
  if ($type_name = theme_get_setting('node_type', 'bootstrap_custom')) {
    if (in_array($type_name, node_type_get_names())) {
      return;
    }

    $type = array(
      'type' => $type_name,
      'name' => st('News'),
      'base' => 'node_content',
      'description' => st('Use <em>news</em> for time-sensitive content like news, press releases or blog posts.'),
      'custom' => 1,
      'modified' => 1,
      'locked' => 0,
    );
    $type = node_type_set_defaults($type);
    node_type_save($type);
    node_add_body_field($type);

    // Create field and add to news bundle.
    $field_name = $type_name . '_sign';
    if (field_info_field($field_name)) {
      return;
    }
    $field = array(
      'field_name' => $field_name,
      'type' => 'text',
    );
    $field = field_create_field($field);

    $instance = array(
      'field_name' => $field['field_name'],
      'entity_type' => 'node',
      'bundle' => $type_name,
      'description' => 'A field for testing the programmatic creation of new fields.',
      'label' => 'Sign',
      'widget' => array(
        'type' => 'textfield',
      ),
    );
    field_create_instance($instance);
  }
}
