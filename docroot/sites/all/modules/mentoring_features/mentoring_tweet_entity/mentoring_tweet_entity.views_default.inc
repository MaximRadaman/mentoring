<?php
/**
 * @file
 * mentoring_tweet_entity.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function mentoring_tweet_entity_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'tweets';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'mentoring_tweet';
  $view->human_name = 'Tweets';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Tweets';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'entity';
  $handler->display->display_options['row_options']['view_mode'] = 'time_line';
  /* Sort criterion: Mentoring tweet Entity: Tweet_created */
  $handler->display->display_options['sorts']['tweet_created']['id'] = 'tweet_created';
  $handler->display->display_options['sorts']['tweet_created']['table'] = 'mentoring_tweet';
  $handler->display->display_options['sorts']['tweet_created']['field'] = 'tweet_created';
  $handler->display->display_options['sorts']['tweet_created']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'tweets';
  $translatables['tweets'] = array(
    t('Master'),
    t('Tweets'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Page'),
  );
  $export['tweets'] = $view;

  return $export;
}
