<?php
/**
 * @file
 * Contains the Tallerini theme pane related implementations and overrides.
 *
 * We implement a logic that allows panels panes to be heavily altered by
 * styles without the need for the creation of new themes. Basically, we make
 * it possible for the styles to alter a pane's structure via a customly
 * available rendereable array. This means many styles can easily be applied
 * to the same pane, which would otherwise be impossible if styles relied on
 * the alteration of themes.
 * 
 * @todo This file should be move to ttools_panels module.
 */

/**
 * Implements template_preprocess_panels_pane().
 */
function tallerini_preprocess_panels_pane(&$variables) {

  $pane = &$variables['pane'];

  if (isset($pane->style) && !empty($pane->style['style'])) {
    $variables['classes_array'][] = 'style-' . drupal_html_class($pane->style['style']);
  }

  // ENTITY FIELD PANES
  // ------------------

  if ($pane->type == 'entity_field') {

    // Hide default pane title when defined another title showing option.
    if ($pane->configuration['label'] != 'title') {
      $variables['title'] = '';
    }

    if ($pane->configuration['formatter'] == 'mini_panel_reference_default') {
      $variables['classes_array'][] = 'pane-panels-mini';
    }
  }

  // Generate default pane structure as renderable array.
  $variables['renderable'] = _tallerini_panels_pane_renderable($variables);

  // Execute any style plugin alters.
  _am_panels_styles_alter('preprocess', $variables);
}

/**
 * Implements template_process_panels_pane().
 */
function tallerini_process_panels_pane(&$variables) {
  // Fulfil post-processed pane attributes.
  $variables['renderable']['pane']['#attributes'] = $variables['renderable']['pane']['#attributes'] + array(
    'class' => $variables['classes_array'],
  ) + $variables['attributes_array'];

  // Fulfil post-processed pane content attributes.
  $variables['renderable']['pane']['content']['#attributes'] = $variables['renderable']['pane']['content']['#attributes'] + $variables['content_attributes_array'];

  // Make sure we don't bother rendering markup if everything is empty.
  $has_content = FALSE;
  foreach ($variables['renderable']['pane'] as $attr => $value) {
    if ($attr[0] !== '#' && !empty($value['#access'])) {
      $has_content = TRUE;
      break;
    }
  }

  $user_has_access = user_access('administer_panels_layouts');

  if (!$has_content && !$user_has_access) {
    $variables['renderable']['#access'] = FALSE;
  }

  // Execute any style plugin alters.
  _am_panels_styles_alter('process', $variables);
}

/**
 * Implements theme_panels_pane().
 */
function tallerini_panels_pane(&$variables) {
  return _tallerini_panels_pane($variables);
}

/**
 * Implements theme_panels_pane__entity_field().
 */
function tallerini_panels_pane__entity_field(&$variables) {
  return _tallerini_panels_pane($variables);
}

/**
 * Helper method to create a default pane structure as a renderable array.
 */
function _tallerini_panels_pane_renderable(&$variables) {

  $pane = $variables['pane'];
  $display = $variables['display'];
  $display_id = !empty($display->css_id) ? $display->css_id . '-' : '';
  $pane_id = drupal_html_id('panel-pane-container-' . $display_id . '-' . $pane->pid);
  $content = &$variables['content'];

  if (!is_array($content)) {
    $content = array(
      '#markup' => $content,
    );
  }

  if (!isset($content['#access'])) {
    $content['#access'] = TRUE;
  }

  // Basic structure.
  $structure = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'panel-pane-container',
      ),
      'id' => $pane_id,
    ),
    'pane' => array(
      '#type' => 'container',
      '#attributes' => array(),
      '#prefix' => $variables['pane_prefix'],
      '#suffix' => $variables['pane_suffix'],
      'admin_links' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('admin-links')),
        '#children' => $variables['admin_links'],
        '#access' => !empty($variables['admin_links']),
      ),
      'title_prefix' => $variables['title_prefix'],
      'title' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('pane-title-wrapper')),
        'heading' => array(
          '#type' => 'html_tag',
          '#tag' => 'h4',
          '#attributes' => empty($variables['title_attributes_array']) ? array() : $variables['title_attributes_array'],
          '#value' => $variables['title'],
          '#access' => !empty($variables['title']),
        ),
      ),
      'title_suffix' => $variables['title_suffix'],
      'feeds' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('feed')),
        '#children' => $variables['feeds'],
        '#access' => !empty($variables['feeds']),
      ),
      'content' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('pane-content')),
        '#access' => !empty($content['#access']),
        'inner' => array(
          '#type' => 'container',
          '#attributes' => array('class' => array('pane-content-inner')),
          '#access' => !empty($content['#access']),
          'renderable' => &$content,
        ),
        '#access' => $content['#access'],
      ),
      'links' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('links')),
        '#children' => $variables['links'],
        '#access' => !empty($variables['links']),
      ),
      'more' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('more-link')),
        '#children' => $variables['more'],
        '#access' => !empty($variables['more']),
      ),
    ),
  );

  return $structure;
}

/**
 * Helper function to render panels panes.
 *
 * Give it a chance to apply our styles before rendering
 */
function _tallerini_panels_pane(&$variables) {
  // Execute any style plugin alters.
  _am_panels_styles_alter('theme', $variables);
  return drupal_render($variables['renderable']);
}
