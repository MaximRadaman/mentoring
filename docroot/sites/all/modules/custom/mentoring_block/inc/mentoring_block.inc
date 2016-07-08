<?php
/**
 * @file
 * Contains helpers, forms, pagecallbacks and etc.
 */

define('MENTORING_BLOCK_SITE_DEFAULT_THEME', 'my_theme');

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
