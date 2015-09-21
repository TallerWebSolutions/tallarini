/**
 * @file
 * Tallerini main JavaScript.
 */
(function ($, Drupal, window, document, undefined) {

  /**
   * Adds context to Bootstrap data-target.
   */
  Drupal.behaviors.talleriniDataToggleContext = {
    attach: function (context, settings) {
      $('[tallerini-data-target-context]', context).each(talleriniDataTargetContext);
    }
  };

  /**
   * Attach dropdown toggler on dropdown menus.
   */
  Drupal.behaviors.talleriniDropdown = {
    attach: function(context, setting) {
      $('.dropdown > a.dropdown-toggle', context).each(talleriniDropdownToggle);
    }
  };

  /**
   * Attach clickable areas.
   */
  Drupal.behaviors.talleriniDataClick = {
    attach: function (context, settings) {
      $('[data-click]', context).each(talleriniDataClick);
    }
  };

  /**
   * Attach Bootstrap tooltips.
   */
  Drupal.behaviors.talleTooltip = {
    attach: function(context, setting) {
      $("[data-toggle='tooltip']").tooltip();
    }
  }

  /**
   * Attach Bootstrap popovers.
   */
  Drupal.behaviors.tallePopover = {
    attach: function(context, setting) {
      $("[data-toggle='popover']").popover();
    }
  }

  /**
   * Contextualizes collapsible navbars.
   */
  Drupal.behaviors.talleriniNavbarCollapse = {
    attach: function (context, settings) {
      $(context).find('.navbar-fixed-top .navbar-collapse').each(talleriniNavbarCollapse);
    }
  };

  /**
   * Helper method to call all callbacks from a given array, optionally
   * with attributes.
   */
  function talleriniCallAll(callbacks) {
    var args = [].slice.call(arguments, 1);
    callbacks.forEach(function (callback) {
      callback.apply(null, args);
    });
  }

  /**
   * Make it possible for unique data-target attributes.
   */
  function talleriniDataTargetContext() {
    var $toggler      = $(this)
      , target        = $toggler.attr('data-target')
      , targetContext = $toggler.attr('tallerini-data-target-context')
      , $context, id, targetContextSelector;

    $context = $toggler.closest(targetContext);
    id = unique();
    targetContextSelector = '[tallerini-data-target-context-wrapper="' + id + '"]';

    // Mark context.
    $context.attr('tallerini-data-target-context-wrapper', id);

    // Update target.
    $toggler.attr('data-target', targetContextSelector + ' ' + target);
  }

  var talleriniDropdownToggleAttachListeners = []
    , talleriniDropdownToggleDettachListeners = [];

  $(function () {
    Drupal.Breakpoints.register(['xs', 'sm'], function () {
      talleriniCallAll(talleriniDropdownToggleDettachListeners);
    });

    Drupal.Breakpoints.register(['md', 'lg'], function () {
      talleriniCallAll(talleriniDropdownToggleAttachListeners);
    });
  });

  /**
   * Activate dropdown toggling on a navbar menu item.
   */
  function talleriniDropdownToggle() {
    var $toggler = $(this)
      , $item = $toggler.parent()
      , action = $toggler.attr('data-dropdown-action') || 'hover'
      , target = $toggler.attr('target') || '_self'
      , event = action + '.tallerini.dropdown'
      , href;

    // Always handle clicks.
    $toggler.on('click.tallerini.dropdown', navigate);

    // At least handle something buggy when no Breakpoints are available.
    if (!Drupal.Breakpoints || Drupal.Breakpoints.isCurrent(['md', 'lg'])) attach();

    talleriniDropdownToggleAttachListeners.push(attach);
    talleriniDropdownToggleDettachListeners.push(dettach);

    /**
     * Attach listeners.
     */
    function attach() {
      switch (action) {
        case 'hover':
          $item.on('mouseenter.tallerini.dropdown', show);
          $item.on('mouseleave.tallerini.dropdown', hide);
        case 'focus':
          $item.on('focusin.tallerini.dropdown', show);
          $item.on('focusout.tallerini.dropdown', hide);
          break;
        default:
          $toggler.on(event, toggle);
          break;
      }
    }

    /**
     * Remove listeners.
     */
    function dettach() {
      $item.off('.tallerini.dropdown');
      $toggler.off(event);
    }

    /**
     * Toggle dropdown state.
     */
    function toggle() {
      // debugger;
      $toggler.attr('aria-expanded', $item.toggleClass('open').hasClass('open'));
    }

    /**
     * Show dropdown.
     */
    function show() {
      $item.addClass('open');
      $toggler.attr('aria-expanded', true);
    }

    /**
     * Show dropdown.
     */
    function hide(e) {
      // Avoid closing when focus/pointer change to an insider element.
      if (e.relatedTarget && $item.find(e.relatedTarget).length) return;

      $item.removeClass('open');
      $toggler.attr('aria-expanded', false);
    }

    /**
     * Navigates main dropdown link, when desired.
     */
    function navigate(e) {
      e.preventDefault();
      if ((action !== 'click' || e.ctrlKey) && (href = $(this).attr('href'))) {
        e.stopImmediatePropagation();
        window.open(href, target);
        return false;
      }
    }
  }

  /**
   * Handles click on wrapping containers.
   */
  function talleriniDataClick() {
    var $clickable = $(this)
      , $links = $clickable.find('a')
      , href = $clickable.attr('data-click') || $clickable.attr('href')
      , target = $clickable.attr('data-click-target') || '_self';

    if (href) {
      $links.on('click', function (e) {
        e.stopPropagation();
      });

      $clickable.css('cursor', 'pointer').on('click', function (e) {
        window.open(href, e.ctrlKey || e.which == 2 ? '_blank' : target);
      });
    }
  }

  /**
   * Add contextual classes for collapsible navbar states.
   */
  function talleriniNavbarCollapse() {
    var $collapsible = $(this)
      , $navbar = $collapsible.closest('.navbar')
      , $body = $('body');

    $collapsible.on('show.bs.collapse hide.bs.collapse', function (e) {
      $body.toggleClass('navbar-collapse-in', e.type == 'show');
      $navbar.toggleClass('navbar-collapse-in', e.type == 'show');
    });
  }

  /**
   * Generates a unique ID.
   */
  function unique() {
    return Math.round(new Date().getTime() + (Math.random() * 100));
  }

})(jQuery, Drupal, this, this.document);
