<?php
/**
 * @file
 * Mentoring forms and validation.
 */

const MIN_VACATION_DAYS = 14;

/**
 * Contacts form.
 */
function mentoring_contacts_form($form, &$form_state) {
  $form['name'] = [
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 50,
    '#maxlength' => 20,
    '#required' => TRUE,
  ];

  $form['email'] = [
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#size' => 50,
    '#maxlength' => 30,
    '#required' => TRUE,
  ];

  $form['subject'] = [
    '#type' => 'textarea',
    '#title' => t('Subject'),
    '#required' => TRUE,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
  ];

  return $form;
}

/**
 * Submit handler for mentoring_contacts_form form.
 */
function mentoring_contacts_form_submit($form, &$form_state) {
  $msg = $form_state['values']['name'] . '<br />';
  $msg .= $form_state['values']['email'] . '<br />';
  $msg .= $form_state['values']['subject'] . '<br />';
  drupal_set_message($msg);
}

/**
 * Vacation form.
 */
function mentoring_vacation_form($form, &$form_state) {
  $form['vacation_days'] = [
    '#type' => 'textfield',
    '#title' => t('Min number of vacation days'),
    '#default_value' => variable_get('vacation_days', MIN_VACATION_DAYS),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t("The minimum number of vacation days during year."),
    '#required' => TRUE,
  ];

  return system_settings_form($form);
}

/**
 * Validation handler for mentoring_vacation_form form.
 */
function mentoring_vacation_form_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['vacation_days']) || $form_state['values']['vacation_days'] < 14) {
    form_set_error('vacation_days', t('@min_vacation_days days is a minimum for vacation.', ['@min_vacation_days' => MIN_VACATION_DAYS]));
  }
}
