/**
 * @file
 * Tallerini Admin main JavaScript.
 */
(function ($, Drupal, window, document, undefined) {

  var $body, $navbar, $adminNavbar;

  /**
   * Handles navbar on administrative pages.
   */
  Drupal.behaviors.talleriniAdminNavbar = {
    attach: function (context, settings) {

      var adminNavbarHeight;

      // Make sure variables are defined.
      $adminNavbar = $adminNavbar || $('#navbar-bar');
      $navbar = $navbar || $('#header > .navbar');
      $body = $body || $('body');

      $(window).on('resize:end', recalc).trigger('resize:end');

      /**
       * Recalculates navbar sizes and body paddings.
       */
      function recalc() {
        bodyPaddingTop = parseInt($body.css('padding-top', '').css('padding-top'));
        adminNavbarHeight = $adminNavbar.outerHeight();

        $body.css('padding-top', bodyPaddingTop + adminNavbarHeight);
        $navbar.css('top', adminNavbarHeight);
      }
    }
  };

})(jQuery, Drupal, this, this.document);
