<?php
/**
 * @file
 * Access callbacks functions.
 */

/**
 * Access callback for viewing moderators docs.
 */
function mentoring_moderators_docs_view_access() {
  return user_access('access content') && (user_has_role(MODERATOR_RID) || user_has_role(ADMINISTRATOR_RID));
}
