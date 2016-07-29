<?php
/**
 * @file
 * Contains MentoringTweetEntityAPIController class.
 */

/**
 * Class MentoringTweetEntityAPIController.
 */
class MentoringTweetEntityAPIController extends EntityAPIController {

  /**
   * {@inheritdoc}
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = []) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    $build['tweet_uscreen_name'] = [
      '#type' => 'markup',
      '#markup' => $entity->tweet_uscreen_name,
      '#prefix' => '<div class="uscreen_name">@',
      '#suffix' => '</div>',
    ];

    $build['tweet_text'] = [
      '#type' => 'markup',
      '#markup' => $entity->tweet_text,
      '#prefix' => '<div class="tweet--text">',
      '#suffix' => '</div>',
    ];

    $build['tweet_created'] = [
      '#type' => 'markup',
      '#markup' => date('d F, Y H:i:s', check_plain($entity->tweet_created)),
    ];
    return $build;
  }

}
