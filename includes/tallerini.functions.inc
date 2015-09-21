<?php
/**
 * @file
 * Contains the theme's helper functions.
 */

/**
 * Include css files as necessary.
 *
 * Helper method to load theme CSS files. Based on CTools 'ctools_add_css'.
 */
function tallarini_add_css($file, $theme = NULL, $base_path = 'assets/css') {
  drupal_add_css(tallarini_theme_path($theme) . "/$base_path/$file.css");
}

/**
 * Helper method to retrive a theme's CSS file path.
 */
function tallarini_css_path($file, $theme = NULL, $base_path = 'assets/css') {
  return tallarini_theme_path($theme) . "/$base_path/$file.css";
}

/**
 * Include js files as necessary.
 *
 * Helper method to load theme CSS files. Based on CTools 'ctools_add_js'.
 */
function tallarini_add_js($file, $theme = NULL, $base_path = 'assets/js') {
  drupal_add_js(tallarini_js_path($file, $theme, $base_path));
}

/**
 * Helper method to retrive a theme's JS file path.
 */
function tallarini_js_path($file, $theme = NULL, $base_path = 'assets/js') {
  return tallarini_theme_path($theme) . "/$base_path/$file.js";
}

/**
 * Helper method to get current theme name.
 */
function tallarini_get_theme($theme = NULL, $default = 'tallarini') {
  return empty($theme) ? variable_get('theme_default', $default) : $theme;
}

/**
 * Helper method to get theme setting.
 */
function tallarini_get_setting($setting_name, $theme = NULL) {
  return theme_get_setting($setting_name, tallarini_get_theme($theme));
}

/**
 * Helper method to get version from JSON package definition (NPM or Bower).
 * 
 * @todo should be moved to tallarini_tools module.
 */
function tallarini_get_package_version($file = '') {
  if (!file_exists($file)) {
    return NULL;
  }

  $decoded_json = drupal_json_decode(file_get_contents($file, TRUE));

  return empty($decoded_json['version']) ? NULL : $decoded_json['version'];
}

/**
 * Returns the path to the theme.
 */
function tallarini_theme_path($theme = NULL, $absolute = FALSE) {
  $path = drupal_get_path('theme', tallarini_get_theme($theme));

  if ($absolute) {
    global $base_path;
    return $base_path . $path;
  }

  return $path;
}

/**
 * Returns the path to the assets directory.
 */
function tallarini_assets_path($theme = NULL, $absolute = TRUE, $path = 'assets/') {
  return tallarini_theme_path($theme, $absolute) . $path;
}

/**
 * Returns the absolute path to an image (or the images directory, if $image
 * is empty).
 */
function tallarini_image_path($image = '', $theme = NULL, $images_dir = NULL, $absolute = TRUE) {
  return tallarini_assets_path($theme, $absolute, empty($images_dir) ? '/assets/images/' : $images_dir) . $image;
}

/**
 * Walks every item in a menu tree and applies the callback to each.
 */
function tallarini_walk_children(&$items, $callback, $recursive = FALSE, $sort = TRUE, $level = 0) {
  foreach (element_children($items, $sort) as $key) {
    $callback($key, $items[$key], $level, $items);
    if ($recursive && !empty($items[$key]['#below'])) {
      tallarini_walk_children($items[$key]['#below'], $callback, $sort, ++$level);
    }
  }
}

/**
 * Helper function to enable mini panels sub-menus on a menu tree.
 */
function tallarini_mini_panel_menu_status(&$tree, $status = TRUE, $recursive = TRUE) {
  tallarini_walk_children($tree, function ($key, &$item) use ($status) {
    $item['#show_mini_panel'] = $status;
  }, $recursive);
}

/**
 * Helper method to cache results for a themes implemented functions.
 */
function tallarini_theme_implements($function, $theme = NULL) {
  static $implemented = array();
  $theme = tallarini_get_theme($theme);

  if (!isset($implemented[$theme][$function])) {
    $implemented[$theme][$function] = function_exists($function);
  }

  return $implemented[$theme][$function];
}
