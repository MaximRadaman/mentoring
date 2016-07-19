<?php
/**
 * @file
 * Contains forms, validators, submit callbacks and helpers.
 */

/**
 * Mentoring_mail_simple_form form.
 */
function mentoring_mail_simple_form($form, &$form_state) {
  $form['email'] = [
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#description' => t('Comma separated emails'),
    '#element_validate' => ['mentoring_mail_email_validation'],
    '#required' => TRUE,
  ];

  $form['params'] = [
    '#tree' => TRUE,
    'message' => [
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#required' => TRUE,
    ],
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Add your response'),
  ];

  return $form;
}

/**
 * Mentoring_mail_simple_form validation.
 */
function mentoring_mail_simple_form_submit($form, &$form_state) {
  mentoring_mail_send_email($form_state['values'], 'mentoring_mail', 'mentoring_mail_simple_form');
}

/**
 * Sending emails.
 */
function mentoring_mail_send_email($form_values, $module, $key) {
  // Specify 'to' and 'from' addresses.
  $to = $form_values['email'];
  $from = variable_get('site_mail', 'Maksim_Radaman@epam.com');

  $params = $form_values['params'];
  $language = language_default();

  $result = drupal_mail($module, $key, $to, $language, $params, $from);
  if ($result['result'] == TRUE) {
    drupal_set_message(t('Your message has been sent.'));
  }
  else {
    drupal_set_message(t('There was a problem sending your message and it was not sent.'), 'error');
  }
}

/**
 * Mentoring_mail_attachment_form form.
 */
function mentoring_mail_attachment_form($form, &$form_state) {
  $form['email'] = [
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#description' => t('Comma separated emails'),
    '#element_validate' => ['mentoring_mail_email_validation'],
    '#required' => TRUE,
  ];

  $form['params'] = [
    '#tree' => TRUE,
    'subject' => [
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#required' => TRUE,
    ],
    'message' => [
      '#type' => 'text_format',
      '#title' => t('Message'),
      '#required' => TRUE,
      '#format' => 'mail_format',
    ],
    'logo' => [
      '#type' => 'managed_file',
      '#title' => t('Logo'),
      '#description' => t('Choose picture for logo. Image allowed extensions jpg, jpeg, png или gif.'),
      '#upload_location' => 'public://tmp/',
      '#upload_validators' => [
        'file_validate_extensions' => ['jpg jpeg png gif'],
      ],
    ],
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Add your response'),
  ];

  return $form;
}

/**
 * Mentoring_mail_attachment_form validation.
 */
function mentoring_mail_attachment_form_submit($form, &$form_state) {
  mentoring_mail_send_email($form_state['values'], 'mentoring_mail', 'mentoring_mail_attachment_form');
}

/**
 * Mentoring mail email validation helper.
 */
function mentoring_mail_email_validation($element, &$form_state) {
  $emails = explode(',', $form_state['values']['email']);
  foreach ($emails as $email) {
    if (!valid_email_address(trim($email))) {
      $incorrect_emails[] = $email;
    }
  }
  if (isset($incorrect_emails) && $incorrect_emails) {
    form_error($element, t('These @emails are incorrect.', ['@emails' => implode(', ', $incorrect_emails)]));
  }
}
