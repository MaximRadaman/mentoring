<?php
/**
 * @file
 * Menutoring menu page callbacks and forms.
 */

/**
 * Page callback for '/news'.
 */
function _mentoring_news_page() {
  return views_embed_view('news', 'all_news');
}

/**
 * Page callback for '/news/%'.
 */
function _mentoring_news_dynamic_title_page() {
  return t('Page content for dynamic title.');
}

/**
 * Page callback for '/greeting/%/%'.
 */
function _mentoring_greeting_page($first_name = 'Guest', $last_name = '') {
  return t('Hello @first_name @last_name', ['@first_name' => $first_name, '@last_name' => $last_name]);
}

/**
 * Page callback for '/user/%user/news'.
 */
function _mentoring_user_published_news($user) {
  $content = '<h2>' . t('@name news:', ['@name' => $user->name]) . '</h2>';
  $content .= views_embed_view('news', 'user_news');
  return $content;
}

/**
 * Page callback for '/moderators/docs'.
 */
function _mentoring_moderators_docs() {
  return t('Content only for moderators and administrators');
}

/**
 * Page callback for '/moderators'.
 */
function _mentoring_moderators_page() {
  return t('All moderators will be listed here in future');
}

/**
 * Page callback for /delivery/json.
 */
function _mentoring_page_delivery_json() {
  drupal_add_http_header('Content-Type', 'application/json');
  return array(t('The current timestamp is') . ' : ' . time());
}

/**
 * Page callback for nodes/published/autocomplete.
 */
function mentoring_nodes_published_autocomplete($exclude, $string) {
  $matches = [];
  $query = db_select('node', 'n');
  $query->fields('n', array('nid', 'title'));
  $query->condition('n.status', 1);
  $query->condition('n.title', '%' . db_like($string) . '%', 'LIKE');

  if ($exclude) {
    $exclude_nids = explode('-', $exclude);
    $query->condition('n.nid', $exclude_nids, 'NOT IN');
  }

  $nodes = $query->execute()->fetchAll();

  foreach ($nodes as $node) {
    $matches[check_plain($node->title) . '(' . $node->nid . ')'] = check_plain($node->title) . '(' . $node->nid . ')';
  }

  $selected_nids[] = $node->nid;

  drupal_json_output($matches);
}

/**
 * Page callback for 'last-node'.
 */
function mentoling_ajax_link_last_node($type) {
  if ($type == 'ajax') {
    $last_node = node_view(current(node_get_recent(1)));
    $last_node = drupal_render($last_node);
    return [
      '#type' => 'ajax',
      '#commands' => [
        ajax_command_html('#last-node', $last_node),
        mentoring_ajax_command_reload(5000),
      ],
    ];
  }
  else {
    drupal_set_message('Turn Javascript');
    drupal_goto(isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '<front>');
  }
}

/**
 * Page callback for 'last-node-page'.
 */
function mentoling_ajax_link_last_node_page() {
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'jquery.form');
  drupal_add_js(drupal_get_path('module', 'mentoring') . '/js/mentoring.js');
  return '<div id="last-node"></div>' . l(t('Last published node'), 'node/last-node/nojs', ['attributes' => ['class' => 'use-ajax']]);
}

/**
 * Ajax command to reload page after offset.
 */
function mentoring_ajax_command_reload($offset = 5000) {
  return [
    'command' => 'mentaringReload',
    'mentoringOffset' => $offset,
  ];
}
