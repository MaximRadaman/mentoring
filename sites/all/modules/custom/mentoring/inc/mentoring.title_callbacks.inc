<?php
/**
 * @file
 * Menu title callbacks.
 */

/**
 * Title callback for /news/%.
 */
function _mentoring_news_dynamic_title($title) {
  return t('Awesome @title', ['@title' => $title]);
}
