<?php
/**
 * @file
 * Mentoring tweet Admin controller.
 */

/**
 * MentoringTweetUIController.
 */
class MentoringTweetUIController extends EntityDefaultUIController {

  /**
   * {@inheritdoc}
   */
  public function hook_menu() {
    $menu = parent::hook_menu();
    $menu['admin/content/' . MENTORING_TWEET_PATH]['type'] = MENU_LOCAL_TASK;
    return $menu;
  }

}
