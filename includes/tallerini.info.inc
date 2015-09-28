<?php
/**
 * @file
 * Contains the theme's info implementations.
 */

/**
 * Implements hook_libraries_info().
 */
function tallerini_libraries_info() {
  return array(
    'touche' => array(
      'name' => 'Touche.js',
      'vendor url' => 'http://benhowdle.im/touche/',
      'download url' => 'https://github.com/benhowdle89/touche/archive/master.zip',
      'version callback' => 'tallerini_libraries_touche_version_callback',
      'files' => array(
        'js' => array(
          'dist/touche.min.js',
        ),
      ),
    ),
  );
}

/**
 * Version callback for the Touche.js library.
 */
function tallerini_libraries_touche_version_callback($library = array()) {
  return tallerini_get_package_version(libraries_get_path('touche') . '/package.json');
}
