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

/**
 * Personal data form.
 */
function mentoring_personal_form($form, &$form_state) {
  $form['enter_first_name'] = [
    '#type' => 'checkbox',
    '#title' => t('Enter first name'),
  ];
  $form['first_name'] = [
    '#type' => 'textfield',
    '#title' => t('Fist name'),
    '#size' => 30,
    '#maxlength' => 20,
    '#states' => [
      'visible' => [
        ':input[name="enter_first_name"]' => ['checked' => TRUE],
      ],
    ],
  ];
  $form['enter_last_name'] = [
    '#type' => 'checkbox',
    '#title' => t('Enter last name'),
  ];
  $form['last_name'] = [
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#size' => 30,
    '#maxlength' => 20,
    '#states' => [
      'visible' => [
        ':input[name="enter_last_name"]' => ['checked' => TRUE],
      ],
    ],
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
  ];

  return $form;
}

/**
 * Personal data form.
 */
function mentoring_base_ajax_form($form, &$form_state) {
  $form['car'] = [
    '#type' => 'select',
    '#title' => t('Choose car'),
    '#options' => [
      'bmw' => t('BMW'),
      'opel' => t('Opel'),
      'vw' => t('VW'),
    ],
    '#required' => TRUE,
  ];
  $form['year'] = [
    '#type' => 'select',
    '#title' => t('Choose year'),
    '#options' => drupal_map_assoc(range(1986, 2016, 1)),
    '#required' => TRUE,
  ];
  $form['#prefix'] = '<div id="choose-car-form">';
  $form['#suffix'] = '</div>';

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#ajax' => [
      'callback' => 'mentoring_base_ajax_form_callback',
      'wrapper' => 'choose-car-form'
    ],
  ];

  return $form;
}

/**
 * Ajax callback for mentoring_base_ajax_form form.
 */
function mentoring_base_ajax_form_callback($form, &$form_state) {
  if ($form_errors = form_get_errors($form)) {
    return $form;
  }
  else {
    return t('Car: @car - @year', ['@car' => $form_state['values']['car'], '@year' => $form_state['values']['year']]);
  }
}

/**
 * Ajax_graceful_degradation_form form.
 */
function mentoring_ajax_graceful_degradation_form($form, &$form_state) {
  $country_options = ['' => t('Select country')] + mentoring_ajax_get_all_countries();
  $city_options = ['' => t('Select city')];
  if (isset($form_state['values']['country']) && $form_state['values']['country']) {
    $city_options += mentoring_get_cites_by_country_code($form_state['values']['country']);
  }
  $street_options = ['' => t('Select street')];
  if (isset($form_state['values']['city']) && $form_state['values']['city']) {
    $street_options += mentoring_get_streets_by_city_id($form_state['values']['city']);
  }

  $form['country_fieldset'] = [
    '#type' => 'fieldset',
  ];

  $form['country_fieldset']['country'] = [
    '#type' => 'select',
    '#title' => t('Choose country'),
    '#options' => $country_options,
    '#ajax' => [
      'callback' => 'mentoring_ajax_degradation_country_callback',
      'wrapper' => 'city-wrapper',
    ],
  ];

  $form['country_fieldset']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Choose Country'),
    '#attributes' => array('class' => array('next-button')),
  );

  $form['city_fieldset'] = [
    '#type' => 'fieldset',
    '#states' => [
      'visible' => [
        ':input[name="country"]' => ['!value' => ''],
      ],
    ],
  ];

  $form['city_fieldset']['city'] = [
    '#type' => 'select',
    '#title' => t('Choose city'),
    '#prefix' => '<div id="city-wrapper">',
    '#suffix' => '</div>',
    '#options' => $city_options,
    '#ajax' => [
      'callback' => 'mentoring_ajax_degradation_city_callback',
      'wrapper' => 'street-wrapper',
    ],
  ];
  $form['city_fieldset']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Choose city'),
    '#attributes' => array('class' => array('next-button')),
  );

  $form['street_fieldset'] = [
    '#type' => 'fieldset',
    '#states' => [
      'visible' => [
        ':input[name="country"]' => ['!value' => ''],
        ':input[name="city"]' => ['!value' => ''],
      ],
    ],
  ];

  $form['street_fieldset']['street'] = [
    '#type' => 'select',
    '#title' => t('Choose street'),
    '#prefix' => '<div id="street-wrapper">',
    '#suffix' => '</div>',
    '#options' => $street_options,
  ];

  if (empty($form_state['values']['country'])) {
    $form['city_fieldset']['city']['#disabled'] = TRUE;
  }
  if (empty($form_state['values']['city'])) {
    $form['street_fieldset']['street']['#disabled'] = TRUE;
  }
  $form['#attached']['css'] = [
    drupal_get_path('module', 'mentoring') . '/css/mentoring--no-js.css',
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
  ];

  return $form;
}

/**
 * Get all countries.
 */
function mentoring_ajax_get_all_countries() {
  return [
    'BY' => t('Belarus'),
    'DE' => t('Germany'),
  ];
}

/**
 * Get Cities by county code.
 *
 * @param string $country_code
 *    E.x. 'BY'.
 *
 * @return array $cities
 *    Cities of given country code.
 */
function mentoring_get_cites_by_country_code($country_code) {
  $cities = [];
  $all_cites_by_country_codes = [
    'BY' => [
      1 => t('Brest'),
      2 => t('Vitebsk'),
    ],
    'DE' => [
      3 => t('Berlin'),
      4 => t('Koln'),
    ],
  ];
  if (!empty($all_cites_by_country_codes[$country_code])) {
    $cities = $all_cites_by_country_codes[$country_code];
  }

  return $cities;
}

/**
 * Get Streets by city id.
 *
 * @param string $city_id
 *    E.x. 'BY'.
 *
 * @return array $streets
 *    Streets of given city id.
 */
function mentoring_get_streets_by_city_id($city_id) {
  $streets = [];
  $all_streets_by_city_id = [
    '1' => [
      1 => t('Brest street 1'),
      2 => t('Brest street 2'),
    ],
    '2' => [
      3 => t('Vitebsk street 1'),
      4 => t('Vitebsk street 2'),
    ],
    '3' => [
      5 => t('Berlin street 1'),
      6 => t('Berlin street 2'),
    ],
    '4' => [
      7 => t('Koln street 1'),
      8 => t('Koln street 2'),
    ],
  ];
  if (!empty($all_streets_by_city_id[$city_id])) {
    $streets = $all_streets_by_city_id[$city_id];
  }

  return $streets;
}

/**
 * Selects city to be returned for re-rendering.
 *
 * @return array
 *   Renderable array (city dropdown).
 */
function mentoring_ajax_degradation_country_callback($form, $form_state) {
  return $form['city_fieldset']['city'];
}

/**
 * Selects street to be returned for re-rendering.
 *
 * @return array
 *   Renderable array (street dropdown).
 */
function mentoring_ajax_degradation_city_callback($form, $form_state) {
  return $form['street_fieldset']['street'];
}

/**
 * Submit function for mentoring_ajax_graceful_degradation_form form.
 */
function mentoring_ajax_graceful_degradation_form_submit($form, &$form_state) {
  switch ($form_state['triggering_element']['#value']) {
    case t('Submit'):
      drupal_set_message(t("Country code: @country <br />City id: @city<br />Street id: @street", [
        '@country' => $form_state['values']['country'],
        '@city' => $form_state['values']['city'],
        '@street' => $form_state['values']['street'],
      ]));

      return;
  }
  $form_state['rebuild'] = TRUE;
}
