/**
 * @file
 * Overrides for CTools modal to make it use Bootstrap stylings.
 * See ctools/js/modal.js
 */

(function ($) {
  /**
   * Override CTools modal show function so it can recognize the Bootstrap modal classes correctly
   */
  Drupal.CTools.Modal.show = function(choice) {
    var opts = {};

    if (choice && typeof choice == 'string' && Drupal.settings[choice]) {
      // This notation guarantees we are actually copying it.
      $.extend(true, opts, Drupal.settings[choice]);
    }
    else if (choice) {
      $.extend(true, opts, choice);
    }

    var defaults = {
      modalTheme: 'CToolsModalDialog',
      throbberTheme: 'CToolsModalThrobber',
      animation: 'show',
      animationSpeed: 'fast',
      modalSize: {
        type: 'scale',
        width: 0.8,
        height: 0.8,
        addWidth: 0,
        addHeight: 0,
        // How much to remove from the inner content to make space for the
        // theming.
        contentRight: 25,
        contentBottom: 45
      },
      modalOptions: {
        opacity: .55,
        background: '#fff'
      }
    };

    var settings = {};
    $.extend(true, settings, defaults, Drupal.settings.CToolsModal, opts);

    if (Drupal.CTools.Modal.currentSettings && Drupal.CTools.Modal.currentSettings != settings) {
      Drupal.CTools.Modal.modal.remove();
      Drupal.CTools.Modal.modal = null;
    }

    Drupal.CTools.Modal.currentSettings = settings;

    // var resize = function(e) {
    //   // When creating the modal, it actually exists only in a theoretical
    //   // place that is not in the DOM. But once the modal exists, it is in the
    //   // DOM so the context must be set appropriately.
    //   var context = e ? document : Drupal.CTools.Modal.modal;

    //   if (Drupal.CTools.Modal.currentSettings.modalSize.type == 'scale') {
    //     var width = $(window).width() * Drupal.CTools.Modal.currentSettings.modalSize.width;
    //     var height = $(window).height() * Drupal.CTools.Modal.currentSettings.modalSize.height;
    //   }
    //   else {
    //     var width = Drupal.CTools.Modal.currentSettings.modalSize.width;
    //     var height = Drupal.CTools.Modal.currentSettings.modalSize.height;
    //   }

    //   // Use the additionol pixels for creating the width and height.
    //   $('div.ctools-modal-dialog', context).css({
    //     'width': width + Drupal.CTools.Modal.currentSettings.modalSize.addWidth + 'px',
    //     'height': height + Drupal.CTools.Modal.currentSettings.modalSize.addHeight + 'px'
    //   });
    //   $('div.ctools-modal-dialog .modal-body', context).css({
    //     'width': (width - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) + 'px',
    //     'height': (height - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) + 'px'
    //   });
    // }

    if (!Drupal.CTools.Modal.modal) {
      Drupal.CTools.Modal.modal = $(Drupal.theme(settings.modalTheme));
    }

    // First, let's get rid of the body overflow.
    $('body').addClass('modal-open');

    // resize();

    $('.modal-title', Drupal.CTools.Modal.modal).html(Drupal.CTools.Modal.currentSettings.loadingText);
    Drupal.CTools.Modal.modalContent(Drupal.CTools.Modal.modal, settings.modalOptions, settings.animation, settings.animationSpeed);
    $('#modalContent .modal-body').html(Drupal.theme(settings.throbberTheme));
  };

  Drupal.CTools.Modal.dismiss = function() {
    if (Drupal.CTools.Modal.modal) {
      Drupal.CTools.Modal.unmodalContent(Drupal.CTools.Modal.modal);
    }
  };

  /**
   * Provide the HTML for the Modal.
   */
  Drupal.theme.prototype.CToolsModalDialog = function () {
    var html = ''
    html += '<div id="ctools-modal">'
    html += '  <div class="ctools-modal-dialog modal-dialog modal-lg">'
    html += '    <div class="modal-content">'
    html += '      <div class="modal-header">';
    html += '        <button type="button" class="close ctools-close-modal" aria-hidden="true">&times;</button>';
    html += '        <h4 id="modal-title" class="modal-title">&nbsp;</h4>';
    html += '      </div>';
    html += '      <div id="modal-content" class="modal-body">';
    html += '      </div>';
    html += '    </div>';
    html += '  </div>';
    html += '</div>';

    return html;
  }

  /**
   * Provide the HTML for Modal Throbber.
   */
  Drupal.theme.prototype.CToolsModalThrobber = function () {
    var html = '';
    html += '  <div class="loading-spinner" style="position: absolute; top: 45%; left: 50%">';
    html += '    <i class="fa fa-cog fa-spin fa-3x"></i>';
    html += '  </div>';

    return html;
  };


  /**
   * modalContent
   * @param content string to display in the content box
   * @param css obj of css attributes
   */
  Drupal.CTools.Modal.modalContent = function(content, css, animation, speed) {

    var $body = $('body')
      , $document = $(document)
      , $modalBackdrop, $modalContent, $close;

    content.hide();

    // Make sure current showing modals are closed.
    // @todo: use this to allow multiple modals?
    if (($modalBackdrop = $('#modalBackdrop')).length) $modalBackdrop.remove();
    if (($modalContent = $('#modalContent')).length) $modalContent.remove();

    // Create modal markup.
    $body.append('<div id="modalBackdrop" class="modal fade"><div id="modalContent">' + $(content).html() + '</div></div>');

    $modalBackdrop = $('#modalBackdrop')
    $modalContent = $modalBackdrop.find('#modalContent')
    $close = $modalBackdrop.find('.close');

    // Bind events.
    $body.bind('focus.ctools keypress.ctools', modalEventHandler);
    $document.bind('keydown.ctools', modalEventEscapeCloseHandler);
    $close.bind('click.ctools', close);

    // Show modal.
    $modalBackdrop.css('display', 'block').addClass('in');
    $modalContent.focus();

    /**
     * Prevent loosing modal focus.
     */
    function modalEventHandler(e) {

      // Make sure event and target are set.
      var target = (e = e || window.event).target || e.srcElement;

      // var parents = $(target).parents().get()
      //   , positions = ['absolute', 'fixed'];

      // for (var i = 0; i < parents.length; ++i) {
      //   if (positions.indexOf(parents.eq(i).css('position')) !== -1) {
      //     return true;
      //   }
      // }

      if ($(target).filter(':visible').parents('#modalContent').length) return true;

      // Restore focus at modal content.
      if ($modalContent) $modalContent.get(0).focus();

      // Make sure default is prevented.
      if (e && e.preventDefault) e.preventDefault();
      return false;
    };

    /**
     * Handle ESC key event.
     */
    function modalEventEscapeCloseHandler(e) {
      if (e.keyCode == 27) return close(e);
    };

    /**
     * Close modal.
     */
    function close(e) {
      return Drupal.CTools.Modal.unmodalContent(null, null, null, e);
    };
  };

  /**
   * unmodalContent
   * @param content (The jQuery object to remove)
   * @param animation (fadeOut, slideUp, show)
   * @param speed (valid animation speeds slow, medium, fast or # in ms)
   */
  Drupal.CTools.Modal.unmodalContent = function(content, animation, speed, e) {
    var $body = $('body')
      , $document = $(document)
      , $modalBackdrop = $('#modalBackdrop')
      , $modalContent = $modalBackdrop.find('#modalContent')
      , $close = $modalBackdrop.find('.close');

    // Unbind the events
    $body.unbind('.ctools').removeClass('modal-open')
    $close.unbind('.ctools');

    // Fire CTools detaching event.
    $document.trigger('CToolsDetachBehaviors', $modalContent);

    $modalBackdrop.removeClass('in');

    setTimeout(function () {
      $modalContent.remove();
      $modalBackdrop.remove();
    }, 500);

    // If run within an event, make sure to prevent default.
    if (e && e.preventDefault) e.preventDefault();
    return false;

    // jQuery magic loop through the instances and run the animations or removal.
    // content.each(function(){
    //   if ( animation == 'fade' ) {
    //     $('#modalContent').fadeOut(speed, function() {
    //       $('#modalBackdrop').fadeOut(speed, function() {
    //         $(this).remove();
    //       });
    //       $(this).remove();
    //     });
    //   } else {
    //     if ( animation == 'slide' ) {
    //       $('#modalContent').slideUp(speed,function() {
    //         $('#modalBackdrop').slideUp(speed, function() {
    //           $(this).remove();
    //         });
    //         $(this).remove();
    //       });
    //     } else {
    //       $('#modalContent').remove();
    //       $('#modalBackdrop').remove();
    //     }
    //   }
    // });
  };

})(jQuery);
