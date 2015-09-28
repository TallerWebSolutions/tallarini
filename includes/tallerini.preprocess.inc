<?php
/**
 * @file
 * Contains the theme's preprocess functions.
 */

/**
 * Implements template_preprocess_container().
 * 
 * Fix containers empty attributes issue.
 * 
 * @todo should be moved to tallerini_tools module.
 */
function tallerini_preprocess_container(&$variables) {
  if (!isset($variables['element']['#attributes'])) {
    $variables['element']['#attributes'] = array();
  }
}

/**
 * Implements template_preprocess_field().
 * 
 * Ensure a format based field class is added.
 */
function tallerini_preprocess_field(&$variables) {
  if (!empty($variables['element']['#formatter'])) {
    $variables['classes_array'][] = 'field-format-' . drupal_html_class($variables['element']['#formatter']);
  }
}

/**
 * Implements template_preprocess_node().
 */
function tallerini_preprocess_node(&$variables) {

  /*
   * Handle view mode suggestions and classes.
   */

  $variables['classes_array'][] = 'view-mode-' . drupal_html_class($variables['view_mode']);


  /*
   * Add hook suggestions based on view mode.
   */

  if (!in_array('node__' . $variables['view_mode'], $variables['theme_hook_suggestions'])) {
    $new_suggestions = array();

    foreach ($variables['theme_hook_suggestions'] as $suggestion) {
      $new_suggestions[] = $suggestion;

      // Add view mode suggestion after first type suggestion.
      if ($suggestion == 'node__' . $variables['type']) {
        $new_suggestions[] = 'node__' . $variables['view_mode'];
      }

      $new_suggestions[] = $suggestion . '__' . $variables['view_mode'];
    }

    $variables['theme_hook_suggestions'] = $new_suggestions;
  }
}

/**
 * Implements template_preprocess_page().
 */
function tallerini_preprocess_page(&$variables) {

  $theme_default = variable_get('theme_default', 'tallerini');
  $navbar_settings = theme_get_setting('navbar', $theme_default);

  /*
   * Prepare main navbar for rendering.
   */

  // Create basic main navbar.
  $variables['main_navbar'] = array(
    '#theme' => 'bootstrap_navbar',
  );

  // Setup positioning.
  $variables['main_navbar']['position'] = empty($navbar_settings['fixed']) ? 'static' : 'fixed-' . $navbar_settings['fixed'];

  // Handle navbar brand.
  if (!empty($navbar_settings['logo'])) {
    $brand_logo = $navbar_settings['logo'];

    if (is_string($brand_logo)) {
      $brand_logo = array(
        'path' => $brand_logo,
      );
    }

    $brand_logo['path'] = tallerini_image_path($brand_logo['path'], $theme_default, '/');

    $variables['main_navbar']['header'] = array(
      'logo' => array(
        '#type' => 'link',
        '#title' => theme('image', $brand_logo),
        '#href' => '<front>',
        '#options' => array(
          'html' => TRUE,
          'attributes' => array(
            'id' => 'main-navbar-brand',
            'class' => array('navbar-brand', 'navbar-brand-image'),
          ),
        ),
      ),
    );
  }

  // Garantee the menu is present. Useful for error pages.
  if (empty($variables['main_menu']) || count($variables['main_menu']) == 1) {
    $tree = menu_tree_all_data(variable_get('menu_main_links_source', 'main-menu'));
    menu_tree_add_active_path($tree);
    $variables['main_menu'] = menu_tree_output($tree);
  }

  // Enable mini panels sub-menu items.
  tallerini_mini_panel_menu_status($variables['main_menu']);

  // Add main menu to main navbar:
  if (!empty($variables['main_menu'])) {
    $variables['main_menu']['#theme_wrappers'] = array('menu_tree__navbar_nav');
    $variables['main_navbar']['content']['main-menu-container'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'main-menu-container',
      ),
      'main-menu' => $variables['main_menu'],
    );
  }


  /*
   * Prepare sitewide newsletter block.
   */

  if (module_exists('tallerini_newsletter') && $newsletter = module_invoke('entityform_block', 'block_view', 'sitewide_newsletter')) {
    $variables['newsletter'] = $newsletter;
  }
}

/**
 * Implements template_preprocess_views_view().
 */
function tallerini_preprocess_views_view(&$variables) {

  /*
   * Make sure we have access to the view object from the pane processing.
   */

  $view = $variables['view'];

  // Make sure we are dealing with a pane display for views.
  if (!empty($view->display_handler) && $view->display_handler instanceof views_content_plugin_display_panel_pane) {


    // Add view mode based class.
    if (!empty($view->style_plugin->row_plugin->options['view_mode'])) {
      $view_mode = drupal_html_class($view->style_plugin->row_plugin->options['view_mode']);
      $variables['classes_array'][] = 'view-view-mode-' . $view_mode;
    }
  }
}
