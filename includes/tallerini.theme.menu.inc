<?php
/**
 * @file
 * Contains the Tallerini theme menu related implementations and overrides.
 */

/**
 * Overrides theme_menu_link().
 */
function tallerini_menu_link(&$variables) {
  global $language_url;

  $element = &$variables['element'];

  $sub_menu = '';
  $has_mini_panel = !empty($element['#show_mini_panel']) && !empty($element['#localized_options']['minipanel']['name']);

  /*
   * Handle submenus.
   */
  if (!empty($element['#below']) || $has_mini_panel) {

    // Wrap in dropdown-menu.
    unset($element['#below']['#theme_wrappers']);
    $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
    $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';

    $sub_menu = array(
      '#type' => 'html_tag',
      '#tag' => 'ul',
      '#attributes' => array('class' => array('dropdown-menu')),
    );

    // Default sub-menu list.
    if (!$has_mini_panel) {
      $sub_menu['#value'] = drupal_render($element['#below']);
    }

    // Mini panel sub-menu.
    else {
      $panel_options = $element['#localized_options']['minipanel'];
      $panel = panels_mini_block_view($panel_options['name']);

      $sub_menu['#tag'] = 'div';
      $sub_menu['#value'] = $panel['content'];
      $sub_menu['#attributes']['class'][] = 'dropdown-mini-panel-menu';

      // Make sure parent is aware of mini-panel sub-menu.
      $element['#attributes']['class'][] = 'mini-panel-item';
      $element['#localized_options']['attributes']['data-target'] = '#';

      // Parse and set size.
      if (!empty($panel_options['size'])) {
        $panel_options['size'] = array(
          'raw' => str_replace(',', '.', $panel_options['size']),
        );
        $panel_options['size']['number'] = floatval($panel_options['size']['raw']);
        $panel_options['size']['unit'] = str_replace($panel_options['size']['number'], '', $panel_options['size']['raw']);
      
        $sub_menu['#attributes']['style'][] = 'width: ' . $panel_options['size']['raw'] . ';';
      }

      // Set alignment.
      switch ($panel_options['align']) {
        case 'left':
        case 'right':
          $sub_menu['#attributes']['class'][] = 'dropdown-align-' . $panel_options['align'];
          break;
        case 'center':
          if (!empty($panel_options['size']['number'])) {
            $margin_left  = 'margin-left: -';
            $margin_left .= $panel_options['size']['number'] / 2;
            $margin_left .= $panel_options['size']['unit'];

            $sub_menu['#attributes']['style'][] = 'left: 50%;';
            $sub_menu['#attributes']['style'][] = $margin_left;
          }
          break;
      }

      // Set relative parent.
      if ($panel_options['relative_to'] == 'navbar') {
        $element['#attributes']['class'][] = 'dropdown-static';
      }

      // Clear configuration.
      unset($element['#localized_options']['minipanel']);
    }

    // Nested dropdowns.
    if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] > 1)) {
      $element['#attributes']['class'][] = 'dropdown-submenu';
    }

    // Main level dropdowns.
    else {
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['html'] = TRUE;
      $element['#title'] .= '<span class="caret"></span>';
    }

    // Render sub-menu items or panel.
    $sub_menu = drupal_render($sub_menu);
  }

  // Fix for active class.
  if (($element['#href'] == current_path() || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']) || $element['#localized_options']['language']->language == $language_url->language)) {
    $element['#attributes']['class'][] = 'active';
  }

  // Avoid active trail on fragmented links.
  if (!empty($element['#localized_options']['fragment'])) {
    foreach (array('active', 'active-trail') as $class_name) {
      foreach (array_keys($element['#attributes']['class'], $class_name) as $key) {
        unset($element['#attributes']['class'][$key]);
      }
    }
  }

  // Add active class to list item if active trail.
  if (in_array('active-trail', $element['#attributes']['class'])) {
    $element['#attributes']['class'][] = 'active';
  }

  // Add a unique class using the title.
  $title = strip_tags($element['#title']);
  $element['#attributes']['class'][] = 'menu-link-' . drupal_html_class($title);

  // Render menu link.
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);

  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
/**
 * Implements theme_menu_link__link(&$variables).
 * 
 * @todo : Lots of copied code here. Make up some logic to inherit at least
 * part of the tallerini_menu_link implementation.
 */
function tallerini_menu_link__link(&$variables) {
  global $language_url;

  $element = &$variables['element'];

  $sub_menu = '';
  $has_mini_panel = !empty($element['#show_mini_panel']) && !empty($element['#localized_options']['minipanel']['name']);

  if (!empty($element['#below'])) {
    unset($element['#below']['#theme_wrappers']);

    $sub_menu = array(
      '#type' => 'html_tag',
      '#tag' => 'ul',
      '#attributes' => array(
        'class' => array('submenu'),
      ),
    );

    // Default sub-menu list.
    if (!$has_mini_panel) {
      $sub_menu['#value'] = drupal_render($element['#below']);
    }

    // Mini panel sub-menu.
    else {
      $panel_options = $element['#localized_options']['minipanel'];
      $panel = panels_mini_block_view($panel_options['name']);

      $sub_menu['#tag'] = 'div';
      $sub_menu['#value'] = $panel['content'];
      $sub_menu['#attributes']['class'][] = 'mini-panel-menu';

      // Make sure parent is aware of mini-panel sub-menu.
      $element['#attributes']['class'][] = 'mini-panel-item';
      $element['#localized_options']['attributes']['data-target'] = '#';

      // Clear configuration.
      unset($element['#localized_options']['minipanel']);
    }

    // Render sub-menu items or panel.
    $sub_menu = drupal_render($sub_menu);
  }

  // Fix for active class.
  if (($element['#href'] == current_path() || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']) || $element['#localized_options']['language']->language == $language_url->language)) {
    $element['#attributes']['class'][] = 'active';
  }

  if (!empty($element['#localized_options']['fragment'])) {
    foreach (array('active', 'active-trail') as $class_name) {
      foreach (array_keys($element['#attributes']['class'], $class_name) as $key) {
        unset($element['#attributes']['class'][$key]);
      }
    }
  }

  // Add active class to li if active trail.
  if (in_array('active-trail', $element['#attributes']['class'])) {
    $element['#attributes']['class'][] = 'active';
  }

  // Add a unique class using the title.
  $title = strip_tags($element['#title']);
  $element['#attributes']['class'][] = 'menu-link-' . drupal_html_class($title);

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Implements theme_menu_tree__dropdown().
 */
function tallerini_menu_tree__dropdown(&$variables) {
  return '<ul class="dropdown-menu standalone">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_menu_tree__links().
 */
function tallerini_menu_tree__links(&$variables) {
  return '<ul class="menu nav-links">' . $variables['tree'] . '</ul>';
}
