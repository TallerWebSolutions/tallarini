<?php
/**
 * @file
 * Contains the Tallarini theme implementations and overrides.
 */

require_once dirname(__FILE__) . '/tallerini.theme.navbar.inc';
require_once dirname(__FILE__) . '/tallerini.theme.menu.inc';
require_once dirname(__FILE__) . '/tallerini.theme.pane.inc';

/**
 * Implements hook_theme().
 */
function tallarini_theme() {
  return array(
    'bootstrap_navbar' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements theme_button().
 */
function tallarini_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#button_type'] = empty($element['#button_type']) ? 'default' : $element['#button_type'];

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'btn-' . $element['#button_type'];
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  // Add a btn-primary class if submit button.
  if (isset($element['#parents']) && ($element['#parents'][0] == 'submit')) {
    $element['#attributes']['class'][] = 'btn-primary';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * Implements template_process_easy_social_links().
 */
function tallarini_process_easy_social_links(&$variables) {
  $element = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => $variables['classes_array']
    ) + $variables['attributes_array'],
  );

  foreach ($variables['widgets'] as $type => $markup) {
    $element[$type] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'easy-social-link',
          drupal_html_class($type),
        ),
      ),
      'content' => array(
        '#markup' => $markup,
      ),
    );
  }

  $variables['element'] = $element;
}

/**
 * Overrides theme_easy_social_links().
 */
function tallarini_easy_social_links(&$variables) {
  return drupal_render($variables['element']);
}
