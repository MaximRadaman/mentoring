<?php
/**
 * @file
 * Contains MentoringTweetEntity class.
 */

/**
 * Class MentoringTweetEntity.
 */
class MentoringTweetEntity extends Entity {

  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => MENTORING_TWEET_PATH . '/' . $this->identifier());
  }

  /**
   * {@inheritdoc}
   */
  protected function defaultLabel() {
    return isset($this->tweet_text) ? truncate_utf8($this->tweet_text, 50, TRUE, TRUE) : t('New Tweet Entity');;
  }

}
