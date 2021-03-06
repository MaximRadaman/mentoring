<?php
/**
 * @file
 * Contains ChankReadFilter class.
 */

/**
 * Class ChankReadFilter.
 */
class ChunkReadFilter implements PHPExcel_Reader_IReadFilter {

  protected $start = 0;
  protected $end = 0;

  /**
   * Set rows.
   */
  public function setRows($start, $end) {
    $this->start = $start;
    $this->end = $end;
  }

  /**
   * {@inheritdoc}
   */
  public function readCell($column, $row, $worksheetName = '') {
    // Only read the heading row, and the rows that are between
    // $this->start and $this->end.
    if ($row >= $this->start && $row <= $this->end) {
      return TRUE;
    }

    return FALSE;
  }

}
