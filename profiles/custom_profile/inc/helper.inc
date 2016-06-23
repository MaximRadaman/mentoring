<?php
/**
 * @file
 * Hepler functions.
 */

/**
 * Recursively copy files from one directory to another
 *
 * @param String $src - Source of files being moved
 * @param String $dest - Destination of files being moved
 *
 * @return bool
 */
function custom_profile_rcopy($src, $dest){
  // If source is not a directory stop processing.
  if (!is_dir($src)) {
    return FALSE;
  }

  // If the destination directory does not exist create it.
  if (!is_dir($dest)) {
    if (!mkdir($dest)) {
      // If the destination directory could not be created stop processing.
      return FALSE;
    }
  }

  // Open the source directory to read in files.
  $i = new DirectoryIterator($src);
  foreach ($i as $f) {
    if ($f->isFile()) {
      copy($f->getRealPath(), $dest . '/' . $f->getFilename());
    }
    elseif (!$f->isDot() && $f->isDir()) {
      custom_profile_rcopy($f->getRealPath(), "$dest/$f");
    }
  }

  return TRUE;
}

/**
 * Make proper theme machine name from theme name.
 *
 * @param $theme_name
 * @return mixed
 */
function custom_profile_conf_theme_machine_name($theme_name) {
  return preg_replace('/\s+/', '_', drupal_strtolower($theme_name));
}
