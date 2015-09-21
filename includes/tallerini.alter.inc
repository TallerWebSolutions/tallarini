<?php
/**
 * @file
 * Contains the theme's alter functions.
 */

/**
 * Implements hook_form_sitewide_newsletter_entityform_edit_form_alter().
 */
function tallarini_form_sitewide_newsletter_entityform_edit_form_alter(&$form, &$form_state, $form_id) {
  $form['#attributes']['class'][] = 'site-wide-newsletter-form';
  $form['#attributes']['class'][] = 'form-group';
  $form['#attributes']['class'][] = 'form-group-sm';
  $form['#attributes']['class'][] = 'form-inline';
  $form['actions']['submit']['#attributes']['class'][] = 'btn-sm';
  $form['actions']['submit']['#button_type'] = 'primary';
  $form['actions']['#weight'] = 50;

  $form['actions']['submit']['#value'] = t('Subscribe');

  $form['newsletter'] = array(
    '#type' => "container",
    '#attributes' => array('class' => array('input-group')),
    'field_email' => $form['field_email'],
    'actions' => $form['actions'],
  );

  $form['newsletter']['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('input-group-btn')),
    'button' => $form['actions']['submit'],
    '#weight' => 50,
  );

  unset($form['field_email']);
  unset($form['actions']);
}

/**
 * Implements hook_page_alter().
 *
 * @todo any better place to add libraries to all pages?
 */
function tallarini_page_alter() {
  libraries_load('touche');
  $subtheme_settings = tallarini_get_setting('tallarini');
  $inherited_styles = !empty($subtheme_settings['sass']['inherited']);

  if (!$inherited_styles) {
    tallarini_add_css('tallarini', 'tallarini');
  }

  if (user_is_logged_in()) {
    tallarini_add_js('tallerini-admin', 'tallarini');

    if ($inherited_styles) {
      tallarini_add_css('tallerini-admin', 'tallarini');
    }
  }
}

/**
 * Implements hook_js_alter().
 */
function tallarini_js_alter(&$javascript) {
  $override_map = array(
    tallarini_js_path('radix-modal', 'radix', 'assets/javascripts') => tallarini_js_path('tallerini-ctools-modal', 'tallarini'),
    drupal_get_path('module', 'ctools') . '/js/dependent.js' => tallarini_js_path('tallerini-dependent', 'tallarini'),
  );

  foreach ($override_map as $from => $to) {
    if (!empty($javascript[$from])) {
      $javascript[$to] = array_merge(
        drupal_js_defaults(), array(
          'group' => JS_THEME,
          'data' => $to
        )
      );

      unset($javascript[$from]);
    }
  }
}
