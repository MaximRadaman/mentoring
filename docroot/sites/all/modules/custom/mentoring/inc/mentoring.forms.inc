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

/**
 * Multistep form.
 */
function mentoring_ajax_multistep_form($form, &$form_state) {
  $form['#prefix'] = '<div id="ajax-multistep-form-wrapper">';
  $form['#suffix'] = '</div>';

  $form['#tree'] = TRUE;

  $step = empty($form_state['storage']['step']) ? 1 : $form_state['storage']['step'];
  $form_state['storage']['step'] = $step;

  switch ($step) {
    case 1:
      $form['step1'] = [
        '#type' => 'fieldset',
        '#title' => t('Step 1: Car brand'),
      ];
      $form['step1']['car'] = [
        '#type' => 'select',
        '#title' => t('Choose car brand'),
        '#options' => drupal_map_assoc([t('BMW'), t('Volvo'), t('Lada')]),
        '#default_value' => empty($form_state['values']['step1']['car']) ? '' : $form_state['values']['step1']['car'],
        '#required' => TRUE,
      ];
      break;

    case 2:
      $form['step2'] = [
        '#type' => 'fieldset',
        '#title' => t('Step 2: Car Appearence'),
      ];
      $form['step2']['color'] = [
        '#type' => 'select',
        '#title' => t('Choose color'),
        '#options' => drupal_map_assoc([t('Red'), t('Black'), t('White')]),
        '#default_value' => empty($form_state['values']['step2']['color']) ? '' : $form_state['values']['step2']['color'],
      ];
      break;

    case 3:
      $form['step3'] = array(
        '#type' => 'fieldset',
        '#title' => t('Step 3: Yout contacts'),
      );
      $form['step3']['phone'] = array(
        '#type' => 'textfield',
        '#title' => t('Your phone'),
        '#default_value' => empty($form_state['values']['step3']['phone']) ? '' : $form_state['values']['step3']['phone'],
        '#required' => TRUE,
      );
      break;
  }

  if ($step == 3) {
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Make order'),
      '#weight' => 2,
    ];
  }

  if ($step < 3) {
    $form['next'] = [
      '#type' => 'submit',
      '#value' => t('Next step'),
      '#ajax' => [
        'wrapper' => 'ajax-multistep-form-wrapper',
        'callback' => 'mentoring_ajax_multistep_form_callback',
      ],
      '#weight' => 2,
    ];
  }

  if ($step > 1) {
    $form['prev'] = [
      '#type' => 'submit',
      '#value' => t('Previous step'),
      '#limit_validation_errors' => [],
      '#submit' => array('mentoring_ajax_multistep_form_submit'),
      '#ajax' => array(
        'wrapper' => 'ajax-multistep-form-wrapper',
        'callback' => 'mentoring_ajax_multistep_form_callback',
      ),
      '#weight' => 1,
    ];
  }
  return $form;
}

/**
 * Ajax callback for mentoring_ajax_multistep_form form.
 */
function mentoring_ajax_multistep_form_callback($form, &$form_state) {
  return $form;
}

/**
 * Submit handler for mentoring_ajax_multistep_form form.
 */
function mentoring_ajax_multistep_form_submit($form, &$form_state) {
  $current_step = 'step' . $form_state['storage']['step'];
  if (!empty($form_state['values'][$current_step])) {
    $form_state['storage']['values'][$current_step] = $form_state['values'][$current_step];
  }

  if ($form_state['triggering_element']['#value'] == t('Next step')) {
    $form_state['storage']['step']++;
    $step_name = 'step' . $form_state['storage']['step'];
    if (!empty($form_state['storage']['values'][$step_name])) {
      $form_state['values'][$step_name] = $form_state['storage']['values'][$step_name];
    }
  }

  if ($form_state['triggering_element']['#value'] == t('Previous step')) {
    $form_state['storage']['step']--;
    $step_name = 'step' . $form_state['storage']['step'];
    $form_state['values'][$step_name] = $form_state['storage']['values'][$step_name];
  }

  if ($form_state['triggering_element']['#value'] == t('Make order')) {
    $value_message = t('Your information has been submitted:') . ' ';
    foreach ($form_state['storage']['values'] as $step => $values) {
      $value_message .= "$step: ";
      foreach ($values as $key => $value) {
        $value_message .= "$key=$value, ";
      }
    }
    drupal_set_message($value_message);
    $form_state['rebuild'] = FALSE;
    return;
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Mentoring_ajax_add_more_node_form form.
 */
function mentoring_ajax_add_more_node_form($form, &$form_state) {
  $form['#tree'] = TRUE;
  $form['nodes_fieldset'] = [
    '#type' => 'fieldset',
    '#title' => t('Select published nodes'),
    '#prefix' => '<div id="add-more-node-form">',
    '#suffix' => '</div>',
  ];

  if (empty($form_state['num_nodes'])) {
    $form_state['num_nodes'] = 1;
  }
  for ($i = 0; $i < $form_state['num_nodes']; $i++) {
    $autocomplete_path = 'nodes/published/autocomplete/0';
    if ($i > 0 && $selected_nodes = $form_state['values']['nodes_fieldset']['node']) {
      $exclude_nids = [];
      foreach ($selected_nodes as $key => $nid) {
        $exclude_nids[] = $nid;
      }
      if ($exclude_nids) {
        $autocomplete_path = 'nodes/published/autocomplete/' . implode('-', $exclude_nids);
      }
    }
    $form['nodes_fieldset']['node'][$i] = [
      '#type' => 'textfield',
      '#title' => t('Published nodes'),
      '#size' => 60,
      '#maxlength' => 40,
      '#autocomplete_path' => $autocomplete_path,
    ];
  }
  $form['nodes_fieldset']['node'][0]['#required'] = TRUE;
  $form['nodes_fieldset']['add_node'] = [
    '#type' => 'submit',
    '#value' => t('Add more node'),
    '#submit' => ['mentoring_ajax_add_more_node_form_add_one'],
    '#ajax' => [
      'wrapper' => 'add-more-node-form',
      'callback' => 'mentoring_ajax_add_more_node_form_callback'
    ],
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Submit'),
  ];

  return $form;
}

/**
 * Submit handler for 'Add more node' button.
 */
function mentoring_ajax_add_more_node_form_add_one($form, &$form_state) {
  $form_state['num_nodes']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Callback for both ajax-enabled buttons.
 */
function mentoring_ajax_add_more_node_form_callback($form, &$form_state) {
  return $form['nodes_fieldset'];
}

/**
 * Validation callback for mentoring_ajax_add_more_node_form form.
 */
function mentoring_ajax_add_more_node_form_validate(&$form, &$form_state) {
  $nodes_titles = $form_state['values']['nodes_fieldset']['node'];
  foreach ($nodes_titles as $key => $title) {
    if ($key > 0 && !$title) {
      unset($form['nodes_fieldset']['node'][$key]);
      unset($form_state['values']['nodes_fieldset']['node'][$key]);
      continue;
    }
    $matches = array();
    $result = preg_match('/\(([0-9]+)\)$/', $title, $matches);

    if ($result > 0) {
      $nid = $matches[$result];
      $node = node_load($nid);
      if (empty($node)) {
        form_error($form['nodes_fieldset']['node'][$key], t('Sorry, no node with nid %nid can be found', array('%nid' => $nid)));
        return;
      }
    }
    else {
      $nid = db_select('node')
        ->fields('node', array('nid'))
        ->condition('title', '%' . db_like($title) . '%', 'LIKE')
        ->range(0, 1)
        ->execute()
        ->fetchField();
    }

    if (!empty($nid)) {
      $form_state['values']['nodes_fieldset']['node'][$key] = $nid;
    }
    else {
      form_error($form['nodes_fieldset']['node'][$key], t('Sorry, no node starting with %title can be found', array('%title' => $title)));
    }
  }
}

/**
 * Submit handler for mentoring_ajax_add_more_node_form form.
 */
function mentoring_ajax_add_more_node_form_submit($form, &$form_state) {
  if ($form_state['values']['nodes_fieldset']['node']) {
    $msg = t('Nodes nids: @nids', ['@nids' => implode(', ', array_values($form_state['values']['nodes_fieldset']['node']))]);
  }
  else {
    $msg = t('No nids.');
  }
  drupal_set_message($msg);
}

/**
 * Mentoring_lottery_ticket_form form.
 */
function mentoring_lottery_ticket_form($form, &$form_state) {
  $form['#prefix'] = '<div id="lottery-form-wrap">';
  $form['#suffix'] = '</div>';
  $form['name'] = [
    '#type' => 'textfield',
    '#title' => t('Enter your First Name'),
    '#required' => TRUE,
  ];

  $form['win_lottery'] = [
    '#type' => 'checkbox',
    '#title' => t('Try to win Lottery without ticket'),
    '#prefix' => '<div id="win-lottery-attempt">',
    '#suffix' => '</div>',
    '#ajax' => [
      'wrapper' => 'lottery-form-wrap',
      'callback' => 'mentoring_win_lottery_ajax_callback',
    ],
    '#default_value' => !empty($form_state['values']['win_lottery']) ? $form_state['values']['win_lottery'] : FALSE,
  ];
  if (isset($form_state['values']['win_lottery']) && $form_state['values']['win_lottery']) {
    $form['win_lottery']['#disabled'] = TRUE;
  }

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Buy Lottery ticket'),
  ];

  return $form;
}

/**
 * Ajax callback on form element win_lottery in mentoring_lottery_ticket_form.
 */
function mentoring_win_lottery_ajax_callback(&$form, &$form_state) {
  $commands = [];
  if ($form_state['values']['name'] && $form_state['values']['name'] == 'Max') {
    $commands[] = ajax_command_replace('#lottery-form-wrap', t('Congratulations!!! You win!!!'));
  }
  else {
    $commands[] = ajax_command_replace('#win-lottery-attempt', drupal_render($form['win_lottery']));
    $commands[] = ajax_command_append('#win-lottery-attempt', t('Attempt failed. You should buy a ticket.'));
  }
  return [
    '#type' => 'ajax',
    '#commands' => $commands,
  ];
}
