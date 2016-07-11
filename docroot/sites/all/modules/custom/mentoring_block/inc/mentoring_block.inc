<?php
/**
 * @file
 * Contains helpers, forms, pagecallbacks and etc.
 */

define('MENTORING_BLOCK_SITE_DEFAULT_THEME', 'my_theme');
define('MENTORING_BLOCK_TERM_BATCH_LIMIT', 100);

/**
 * Mentoring_block_visibility_form Form.
 */
function mentoring_block_visibility_form($form, &$form_state) {
  module_load_include('inc', 'block', 'block.admin');
  $blocks = block_admin_display_prepare_blocks(variable_get('theme_default', MENTORING_BLOCK_SITE_DEFAULT_THEME));
  $blocks_list = [];
  foreach ($blocks as $block) {
    $blocks_list[$block['bid']] = $block['info'] . '(' . $block['module'] . '_' . $block['delta'] . ')';
  }

  $form['mentoring_block_hidden_blocks'] = [
    '#type' => 'checkboxes',
    '#title' => t('Selected blocks will be hidden'),
    '#options' => $blocks_list,
    '#default_value' => array_keys(variable_get('mentoring_block_hidden_blocks', [])),
  ];

  $form = system_settings_form($form);
  $form['#submit'][] = 'mentoring_block_visibility_form_submit';

  return $form;
}

/**
 * Validation handler for mentoring_block_visibility_form.
 */
function mentoring_block_visibility_form_validate($form, &$form_state) {
  foreach ($form_state['values']['mentoring_block_hidden_blocks'] as $key => $value) {
    if (!$value) {
      unset($form_state['values']['mentoring_block_hidden_blocks'][$key]);
      continue;
    }
    $form_state['values']['mentoring_block_hidden_blocks'][$key] = $form['mentoring_block_hidden_blocks'][$key]['#title'];
  }
}

/**
 * Submit handler for mentoring_block_visibility_form.
 */
function mentoring_block_visibility_form_submit($form, &$form_state) {
  if ($form_state['values']['mentoring_block_hidden_blocks']) {
    foreach ($form_state['values']['mentoring_block_hidden_blocks'] as $bid => $block_name) {
      if (mentoring_block_disable_block($bid)) {
        drupal_set_message(t('Block @block_name disabled', ['@block_name' => $block_name]));
      }
    }
  }
}

/**
 * Disable block by it's id.
 */
function mentoring_block_disable_block($bid) {
  try {
    db_update('block')
      ->fields([
        'status' => 0,
      ])
      ->condition('bid', (int) $bid)
      ->execute();
  }
  catch (Exception $e) {
    watchdog_exception('mentoring_block', $e);
    return FALSE;
  }
  return TRUE;
}

/**
 * Mentoring_block_terms_upload_form form.
 */
function mentoring_block_terms_upload_form($form, &$form_state) {
  $form['mentoring_block_terms_file'] = [
    '#type' => 'managed_file',
    '#title' => t('Import'),
    '#description' => t('The uploaded excel will be imported and temporarily saved.'),
    '#upload_location' => 'public://tmp/',
    '#upload_validators' => [
      'file_validate_extensions' => ['xls xlsx'],
    ],
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Import'),
  ];

  return $form;

}

/**
 * Submit handler for mentoring_block_terms_upload_form.
 */
function mentoring_block_terms_upload_form_submit($form, &$form_state) {
  // Check file upload.
  $uri = db_query('SELECT uri FROM {file_managed} WHERE fid = :fid', [
    ':fid' => $form_state['input']['mentoring_block_terms_file']['fid']
  ])->fetchField();

  if (empty($uri)) {
    drupal_set_message(t('No correct $uri'), 'error');
    return;
  }

  $file_path = drupal_realpath($uri);

  if (!file_exists($file_path)) {
    drupal_set_message(t('No file found'), 'error');
    return;
  }

  // Load library.
  $library = libraries_load('PHPExcel');
  if (empty($library['loaded'])) {
    drupal_set_message(t("Couldn't load the PHPExcel library."), 'error');
    return;
  }

  // Specify max batch operations.
  try {
    $fileObj = \PHPExcel_IOFactory::load($file_path);
  }
  catch (Exception $e) {
    watchdog_exception('mentoring_block', $e);
  }
  if ($fileObj instanceof PHPExcel) {
    $sheetObj = $fileObj->getActiveSheet();
  }
  $highestRow = $sheetObj->getHighestRow();
  if (!$highestRow) {
    return;
  }
  $operations_count = ceil($highestRow / MENTORING_BLOCK_TERM_BATCH_LIMIT);

  $operations = [];
  for ($i = 0, $start = 1, $end = MENTORING_BLOCK_TERM_BATCH_LIMIT; $i < $operations_count; $i++, $start = $end + 1, $end += MENTORING_BLOCK_TERM_BATCH_LIMIT) {
    if ($i == ($operations_count - 1) &&
      ($limit = $highestRow % MENTORING_BLOCK_TERM_BATCH_LIMIT)) {
      $end = $end - MENTORING_BLOCK_TERM_BATCH_LIMIT + $limit;
    }
    $operations[] = [
      'mentoring_block_batch_term_create',
      [$file_path, $start, $end],
    ];
  }
  $batch = [
    'title' => t('Importing Excel...'),
    'operations' => $operations,
    'finished' => 'mentoring_block_terms_upload_batch_finished',
    'error_message' => t('The installation has encountered an error.'),
    'progress_message' => t('Imported @current of @total products.'),
    'file' => drupal_get_path('module', 'mentoring_block') . '/inc/mentoring_block.batch.inc',
  ];
  batch_set($batch);

}

/**
 * Mentoring_block_node_generate_form form.
 */
function mentoring_block_node_generate_form($form, &$form_state) {
  $form['node_type'] = [
    '#type' => 'radios',
    '#title' => t('Select node type'),
    '#options' => node_type_get_names(),
    '#required' => TRUE,
  ];
  $form['node_count'] = [
    '#type' => 'textfield',
    '#title' => t('Number of nodes'),
    '#size' => 20,
    '#element_validate' => ['element_validate_integer_positive'],
    '#required' => TRUE,
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Generate'),
  ];
  return $form;
}

/**
 * Submit handler for mentoring_block_node_generate_form Form.
 */
function mentoring_block_node_generate_form_submit($form, &$form_state) {
  $batch = [
    'title' => t('Generating nodes...'),
    'operations' => [
      [
        'mentoring_block_batch_generate_nodes', [$form_state['values']['node_type'], $form_state['values']['node_count']],
      ]
    ],
    'finished' => 'mentoring_block_generate_nodes_batch_finished',
    'error_message' => t('The installation has encountered an error.'),
    'progress_message' => t('Imported @current of @total nodes.'),
    'file' => drupal_get_path('module', 'mentoring_block') . '/inc/mentoring_block.batch.inc',
  ];
  batch_set($batch);
}
