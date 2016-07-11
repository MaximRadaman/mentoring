<?php
/**
 * @file
 * Contains batch operations.
 */

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