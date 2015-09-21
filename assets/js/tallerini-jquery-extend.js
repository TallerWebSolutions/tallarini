/**
 * @file
 * Tallerini jQuery extensions.
 */
(function ($, Drupal, window, document, undefined) {

  var $window = $(window);

  /**
   * Create a resize:end event on the window.
   */
  $window.each(function () {
    var $window = $(this)
      , delta = $.resizeEndDelta || 500
      , timeout;

    // Bind to window resize event.
    $window.on('resize', function() {
      !timeout && (timeout = setTimeout(fire, delta));
    });

    /**
     * Fires resize:end event.
     */
    function fire() {
      timeout = false;
      $window.trigger('resize:end');
    }
  });

})(jQuery, Drupal, this, this.document);
