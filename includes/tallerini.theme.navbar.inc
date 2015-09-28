<?php
/**
 * @file
 * Contains the Tallerini theme Navbar implementations and overrides.
 */

/**
 * Implements template_process_bootstrap_navbar().
 */
function tallerini_process_bootstrap_navbar(&$variables) {

  // Avoid unsupported operations.
  if (empty($variables['element'])) $variables['element'] = array();

  $element = &$variables['element'];

  // Inherit defaults.
  $element += array(
    'navbar' => array(),
    'content' => array(),
    'header' => array(),
    'type' => 'default',
    'position' => 'static-top',
  );

  $navbar_id = !empty($element['navbar']['#attributes']['id']) ? $element['navbar']['#attributes']['id'] : drupal_html_id('navbar');

  $element['navbar'] = array_merge_recursive(array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'navbar',
        'navbar-' . $element['type'],
        'navbar-' . $element['position'],
      ),
      'id' => $navbar_id,
    ),
    'container' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'container',
        ),
      ),
      'header' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array(
            'navbar-header',
          ),
        ),
        'toggle' => array(
          '#type' => 'html_tag',
          '#tag' => 'button',
          '#attributes' => array(
            'type' => 'button',
            'class' => array('navbar-toggle'),
            'data-toggle' => 'collapse',
            'data-target' => '.navbar-collapse',
            'tallerini-data-target-context' => '#' . $navbar_id,
          ),
          '#value' => 
            '<span class="sr-only">' . t('Toggle navigation') . '</span>' .
            '<span class="icon-bar"></span>' .
            '<span class="icon-bar"></span>' .
            '<span class="icon-bar"></span>',
        ),
      ),
      'collapse' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('navbar-collapse', 'collapse'),
        ),
      ),
    ),
  ), $element['navbar']);

  // Merge pre-content.
  $element['navbar']['container']['collapse'] = array_merge_recursive(
    $element['navbar']['container']['collapse'],
    $element['content']
  );

  // Merge pre-content.
  $element['navbar']['container']['header'] = array_merge_recursive(
    $element['navbar']['container']['header'],
    $element['header']
  );

  // Make easy access.
  $element['content'] = &$element['navbar']['container']['collapse'];
  $element['header'] = &$element['navbar']['container']['header'];
}

/**
 * Implements theme_bootstrap_navbar().
 */
function tallerini_bootstrap_navbar($variables) {
  $element = &$variables['element'];
  $brand = &$element['navbar']['container']['header']['brand'];

  // Avoid brand part with no link.
  if (empty($brand['#text'])) {
    unset($brand);
  } elseif (empty($brand['#path'])) {
    $brand['#type'] = 'html_tag';
    $brand['#tag'] = 'span';
    $brand['#value'] = empty($brand['#text']) ? '' : $brand['#text'];
    $brand['#attributes'] = empty($brand['#options']['attributes']) ? array() : $brand['#options']['attributes'];
  }

  return drupal_render($element['navbar']);
}
