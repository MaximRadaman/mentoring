<?php
/**
 * @file
 * Contains batch operations.
 */

define('MENTORING_BLOCK_NODE_BATCH_LIMIT', 5);
define('MENTORING_BLOCK_UPLOAD_TERMS_VOCABULARY', 'tags');

/**
 * Batch operation for creating terms from Excel.
 *
 * @see mentoring_block_terms_upload_form_submit()
 */
function mentoring_block_batch_term_create($file_path, $start, $end, &$context) {

  // Load library.
  $library = libraries_load('PHPExcel');
  if (empty($library['loaded'])) {
    drupal_set_message(t("Couldn't load the PHPExcel library."), 'error');
    $context['sandbox']['finished'] = 1;
    $context['success'] = FALSE;
    return;
  }

  // Create a new Reader of the type.
  try {
    $inputFileType = \PHPExcel_IOFactory::identify($file_path);
    $objReader = \PHPExcel_IOFactory::createReader($inputFileType);
    $chunkFilter = new ChunkReadFilter();
    $objReader->setReadFilter($chunkFilter);
    $chunkFilter->setRows($start, $end);
    $objPHPExcel = $objReader->load($file_path);
  }
  catch (Exception $e) {
    watchdog_exception('mentoring_block', $e, $e->getMessage());
    return;
  }

  $sheetObj = $objPHPExcel->getActiveSheet();
  foreach ($sheetObj->getRowIterator($start, $end) as $row) {
    foreach ($row->getCellIterator() as $cell) {
      $voc = taxonomy_vocabulary_machine_name_load(MENTORING_BLOCK_UPLOAD_TERMS_VOCABULARY);
      if (!$voc) {
        return;
      }
      if ($term = taxonomy_get_term_by_name($cell->getCalculatedValue(), $voc->machine_name)) {
        continue;
      }
      $term = mentoring_block_create_term($cell->getCalculatedValue(), $voc);
      if (!$term->tid) {
        continue;
      }
      $context['results'][] = $term;
    }
  }
}

/**
 * Helper function for creating term.
 */
function mentoring_block_create_term($term_name, $vocabulary) {
  // Create term.
  $term = new stdClass();
  $term->name = $term_name;
  $term->vid = $vocabulary->vid;
  $term->parent = 0;
  taxonomy_term_save($term);
  return $term;
}

/**
 * This function runs when the batch processing is complete.
 *
 * @see mentoring_block_terms_upload_form_submit()
 */
function mentoring_block_terms_upload_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t("!count items were processed.", array(
      '!count' => count($results),
    ));
    $message .= theme('item_list', [
      'items' => array_map(function ($term) {
        return l($term->name, "taxonomy/term/{$term->tid}");
      }, $results)
    ]);
    drupal_set_message($message);
  }
}

/**
 * Batch operation for generating nodes.
 *
 * @see mentoring_block_node_generate_form_submit()
 */
function mentoring_block_batch_generate_nodes($node_type, $node_count, &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $node_count;
  }
  global $user;
  $voc = taxonomy_vocabulary_machine_name_load(MENTORING_BLOCK_UPLOAD_TERMS_VOCABULARY);
  $term_list = taxonomy_get_tree($voc->vid, 0, 1);
  $term_count = count($term_list);
  $limit = ($node_count < MENTORING_BLOCK_NODE_BATCH_LIMIT) ? $node_count : MENTORING_BLOCK_NODE_BATCH_LIMIT;
  for ($i = 0; $i < $limit; $i++) {
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
      break;
    }
    $term_key = rand(1, $term_count);
    if (!$node = mentoring_block_node_create($node_type, $user, $term_list[$term_key])) {
      continue;
    }
    else {
      $context['results'][] = $node;
      $context['sandbox']['progress']++;
    }
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * This function runs when the batch processing is complete.
 */
function mentoring_block_generate_nodes_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t("!count items were processed.", array(
      '!count' => count($results),
    ));
    $message .= theme('item_list', [
      'items' => array_map(function ($node) {
        return l($node->label(), 'node/' . $node->getIdentifier());
      }, $results)
    ]);
    drupal_set_message($message);
  }
}

/**
 * Helper function for creating node.
 */
function mentoring_block_node_create($node_type, $user, $term) {
  $e = entity_create('node', ['type' => $node_type]);
  $e->uid = $user->uid;
  $entity = entity_metadata_wrapper('node', $e);
  $entity->title = $node_type . $user->name . time();
  $entity->field_text_field = 'Test info' . time();
  $entity->body->set(['value' => "<b>Node body</b>"]);
  if (!file_exists('public://images') || !is_dir('public://images')) {
    drupal_mkdir('public://images');
  }
  $image_url = 'https://placekitten.com/' . round(rand(200, 600)) . '/' . round(rand(200, 400));
  $image_file_info = system_retrieve_file($image_url, 'public://images/', TRUE);
  if ($image_file_info) {
    $entity->field_image = ['fid' => $image_file_info->fid];
  }
  if (!file_exists('public://txt') || !is_dir('public://txt')) {
    drupal_mkdir('public://txt');
  }
  $filename = 'generated' . time() . '.txt';
  $txt_file = file_save_data(file_get_contents('http://metaphorpsum.com/paragraphs/4'), 'public://txt/' . $filename);
  if ($txt_file) {
    $entity->field_text_file->file->set($txt_file);
  }
  $entity->field_tags[] = $term->tid;
  $entity->save();
  return $entity;
}
