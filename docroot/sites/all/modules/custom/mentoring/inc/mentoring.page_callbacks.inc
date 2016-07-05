<?php
/**
 * @file
 * Menutoring menu page callbacks and forms.
 */

/**
 * Page callback for '/news'.
 */
function _mentoring_news_page() {
  return views_embed_view('news', 'all_news');
}

/**
 * Page callback for '/greeting/%/%'.
 */
function _mentoring_greeting_page($first_name = 'Guest', $last_name = '') {
  return t('Hello @first_name @last_name', ['@first_name' => $first_name, '@last_name' => $last_name]);
}

/**
 * Page callback for '/user/%user/news'.
 */
function _mentoring_user_published_news($user) {
  $content = '<h2>' . t('@name news:', ['@name' => $user->name]) . '</h2>';
  $content .= views_embed_view('news', 'user_news');
  return $content;
}

/**
 * Contacts form.
 */
function mentoring_contacts_form() {
  $form = [
    'name' => [
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#size' => 50,
      '#maxlength' => 20,
      '#required' => TRUE,
    ],
    'email' => [
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#size' => 50,
      '#maxlength' => 30,
      '#required' => TRUE,
    ],
    'subject' => [
      '#type' => 'textarea',
      '#title' => t('Subject'),
      '#required' => TRUE,
    ]
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
function mentoring_vacation_form(){
  $form = [
    'vacation_days' => [
      '#type' => 'textfield',
      '#title' => t('Min number of vacation days'),
      '#default_value' => variable_get('vacation_days', 14),
      '#size' => 2,
      '#maxlength' => 2,
      '#description' => t("The minimum number of vacation days during year."),
      '#required' => TRUE,
    ]
  ];

  return system_settings_form($form);
}

/**
 * Validation handler for mentoring_vacation_form form.
 */
function mentoring_vacation_form_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['vacation_days']) || $form_state['values']['vacation_days'] < 14) {
    form_set_error('vacation_days', t('14 days is a minimum for vacation.'));
  }
}

/**
 * Page callback for '/moderators/docs'.
 */
function _mentoring_moderators_docs() {
  return t('Content only for moderators and administrators');
}

/**
 * Page callback for '/moderators'.
 */
function _mentoring_moderators_page() {
  return t('All moderators will be listed here in future');
}

/**
 * Page callback for /news/*.
 */
function _mentoring_news_dynamic_title_page() {
  return t('Page content for dynamic title.');
}

/**
 * Page callback for /delivery/json.
 */
function _mentoring_page_delivery_json() {
  drupal_add_http_header('Content-Type', 'application/json');
  return array(t('The current timestamp is') . ' : ' . time());
}

/**
 * Custom page delivery callback.
 */
function _mentoring_page_delivery($page_callback_result) {
  $content_type = drupal_get_http_header('Content-Type');

  if (is_null($content_type)) {
    drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  }

  // Send appropriate HTTP-Header for browsers and search engines.
  global $language;
  drupal_add_http_header('Content-Language', $language->language);

  switch ($content_type) {
    case 'application/json':
      print json_encode($page_callback_result);
      break;

    case 'application/xml':
      print '<?xml version="1.0" encoding="UTF-8"><root>' . render($page_callback_result) . '</root>';
      break;

    default:
      print '<html><head></head><body>';
      print render($page_callback_result);
      print '</body></html>';
  }
  drupal_page_footer();
}
