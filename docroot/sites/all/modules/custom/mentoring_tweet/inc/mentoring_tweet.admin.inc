<?php
/**
 * @file
 * Helper functions.
 */

/**
 * Add/ediit form for Mentoring Tweet Entity.
 */
function mentoring_tweet_form($form, &$form_state, $mentoring_tweet = NULL) {
  $form['tweet_id'] = [
    '#type' => 'textfield',
    '#title' => t('Tweet id'),
    '#default_value' => isset($mentoring_tweet->tweet_id) ? $mentoring_tweet->tweet_id : '',
    '#element_validate' => ['element_validate_integer_positive'],
    '#required' => TRUE,
  ];
  $form['tweet_text'] = [
    '#type' => 'textarea',
    '#title' => t('Tweet message'),
    '#default_value' => isset($mentoring_tweet->tweet_text) ? $mentoring_tweet->tweet_text : '',
    '#maxlength' => 140,
    '#required' => TRUE,
  ];
  $form['tweet_hashtag_name'] = [
    '#type' => 'textfield',
    '#title' => t('Tweet hashtag name'),
    '#default_value' => isset($mentoring_tweet->tweet_hashtag_name) ? $mentoring_tweet->tweet_hashtag_name : '',
    '#maxlength' => 30,
    '#required' => TRUE,
  ];
  $form['tweet_lang'] = [
    '#type' => 'textfield',
    '#title' => t('Tweet language'),
    '#default_value' => isset($mentoring_tweet->tweet_lang) ? $mentoring_tweet->tweet_lang : '',
    '#required' => TRUE,
  ];
  $form['tweet_uid'] = [
    '#type' => 'textfield',
    '#title' => t('Twitter user id'),
    '#default_value' => isset($mentoring_tweet->tweet_uid) ? $mentoring_tweet->tweet_uid : '',
    '#element_validate' => ['element_validate_integer_positive'],
    '#required' => TRUE,
  ];
  $form['tweet_uscreen_name'] = [
    '#type' => 'textfield',
    '#title' => t('Tweet user screen name'),
    '#default_value' => isset($mentoring_tweet->tweet_uscreen_name) ? $mentoring_tweet->tweet_uscreen_name : '',
    '#required' => TRUE,
  ];

  field_attach_form('mentoring_tweet', $mentoring_tweet, $form, $form_state);

  $form['actions'] = ['#type' => 'actions'];
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => isset($mentoring_tweet->tweet_id) ? t('Update') : t('Create'),
  ];
  return $form;
}

/**
 * Submit handler for Mentoring Tweet Entity add/edit form.
 */
function mentoring_tweet_form_submit($form, &$form_state) {
  try {
    $mentoring_tweet = entity_ui_form_submit_build_entity($form, $form_state);
    if (!isset($mentoring_tweet->tweet_created)) {
      $mentoring_tweet->tweet_created = REQUEST_TIME;
    }
    $mentoring_tweet->save();
  }
  catch (Exception $e) {
    watchdog_exception('mentoring_tweet', $e);
  }

  if (isset($mentoring_tweet) && $mentoring_tweet) {
    drupal_set_message(t('The tweet: @id has been saved.', ['@id' => $mentoring_tweet->tweet_id]));
    $form_state['redirect'] = 'admin/content/' . MENTORING_TWEET_PATH;
  }
}

/**
 * Mentoring_tweet_hashtag_form form.
 */
function mentoring_tweet_hashtag_form($form, &$form_state) {
  $form['twitter_oath_connection'] = [
    '#type' => 'fieldset',
    '#title' => t('Twitteroath connection settings'),
  ];
  $form['twitter_oath_connection']['mentoring_tweet_consumer_key'] = [
    '#type' => 'textfield',
    '#title' => t('TwitterOath Consumer key'),
    '#default_value' => variable_get('mentoring_tweet_consumer_key', 'ciNILTdNOC3uSWTuzHZ9dntNN'),
    '#required' => TRUE,
  ];
  $form['twitter_oath_connection']['mentoring_tweet_consumer_secret'] = [
    '#type' => 'textfield',
    '#title' => t('TwitterOath Consumer key secret'),
    '#default_value' => variable_get('mentoring_tweet_consumer_secret', 'DUzmeycywI2GlL7Zb3kpnpTI0YyaLqTPqETMiaGFYrNobM7LWT'),
    '#required' => TRUE,
  ];
  $form['twitter_oath_connection']['mentoring_tweet_access_token'] = [
    '#type' => 'textfield',
    '#title' => t('TwitterOath Access token'),
    '#default_value' => variable_get('mentoring_tweet_access_token', '3044704990-e7WoXYRMsaCDRhT3ZS4CIprhrWIy7hnEAeD6nkw'),
    '#required' => TRUE,
  ];
  $form['twitter_oath_connection']['mentoring_tweet_access_token_secret'] = [
    '#type' => 'textfield',
    '#title' => t('TwitterOath Access token secret'),
    '#default_value' => variable_get('mentoring_tweet_access_token_secret', 'uRQJNWW2m8DNMCtJOmj1Bcv0txwZWTAqDEO3Oh85bVZV3'),
    '#required' => TRUE,
  ];
  $form['mentoring_tweet_cron_settings'] = [
    '#type' => 'fieldset',
    '#title' => t('Cron settings for mentoring tweet job'),
  ];
  $form['mentoring_tweet_cron_settings']['mentoring_tweet_cron_interval'] = [
    '#type' => 'textfield',
    '#title' => t('Mentoring tweet cron interval in seconds'),
    '#default_value' => variable_get('mentoring_tweet_cron_interval', 3600),
    '#required' => TRUE,
  ];
  $form['mentoring_tweet_cron_settings']['mentoring_tweet_tweets_per_run'] = [
    '#type' => 'textfield',
    '#title' => t('Tweets per cron run'),
    '#default_value' => variable_get('mentoring_tweet_tweets_per_run', 100),
    '#required' => TRUE,
  ];
  $form['mentoring_tweet_cron_settings']['mentoring_tweet_hashtag'] = [
    '#type' => 'textfield',
    '#title' => t('Twitter hashtag'),
    '#description' => t('Provide tweet hashtag name without #<br />To delete all tweets place `all`'),
    '#default_value' => variable_get('mentoring_tweet_hashtag'),
    '#required' => TRUE,
  ];

  $form = system_settings_form($form);
  $form['actions']['clear'] = [
    '#type' => 'submit',
    '#value' => t('Delete tweets'),
    '#submit' => ['mentoring_tweet_hashtag_form_clear'],
  ];
  $form['actions']['reset_cron'] = [
    '#type' => 'submit',
    '#value' => t('Reset cron next execution time'),
    '#submit' => ['mentoring_tweet_hashtag_form_reset_cron_next_time'],
  ];

  return $form;
}

/**
 * Mentoring_tweet_hashtag_form form validation.
 */
function mentoring_tweet_hashtag_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['mentoring_tweet_hashtag'])) {
    $form_state['values']['mentoring_tweet_hashtag'] = str_replace('#', '', trim(check_plain($form_state['values']['mentoring_tweet_hashtag'])));
  }
}

/**
 * Delete mentoring entities by hashtagname.
 */
function mentoring_tweet_hashtag_form_clear($form, &$form_state){
  $conditions = [];
  if (!empty($form_state['values']['mentoring_tweet_hashtag'])) {
    $conditions = [
      'tweet_hashtag_name' => trim(str_replace('#', '', $form_state['values']['mentoring_tweet_hashtag'])),
    ];
    if (drupal_strtolower($form_state['values']['mentoring_tweet_hashtag']) == 'all') {
      $conditions = [];
    }
  }
  $message = '';
  try {
    $tweets = entity_load('mentoring_tweet', FALSE, $conditions);
    if (!$tweets) {
      return;
    }
    foreach ($tweets as $tweet) {
      entity_delete('mentoring_tweet', $tweet->tweet_id);
      $message .= t('Mentoring tweet with id @tweet_id has been deleted.', ['@tweet_id' => $tweet->tweet_id]) . '<br />';
    }
  }
  catch (Exception $e) {
    watchdog_exception('mentoring_tweet', $e);
  }

  variable_del('mentoring_tweet_hashtag_parsing');
  variable_del('mentoring_tweet_since_id');

  drupal_set_message($message);
  $form_state['rebuild'] = TRUE;
}

/**
 * Reset next cron execution time for mentoring_tweet_cron.
 *
 * @see mentoring_tweet_cron
 */
function mentoring_tweet_hashtag_form_reset_cron_next_time($form, &$form_state) {
  variable_set('mentoring_tweet_cron_next_execution', 0);
}
