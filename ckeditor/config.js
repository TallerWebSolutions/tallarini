/**
 * @file
 * CKEditor customization.
 */

;(function () {

  // Avoid variable undefined errors.
  if (!window.CKEDITOR) return;

  CKEDITOR.on('dialogDefinition', tableDialogDefinition);
  CKEDITOR.on('instanceReady', alignmentCommandsSetup);

  /**
   * CKEditor dialog definition callback to improve table settings with
   * Bootstrap related options.
   */
  function tableDialogDefinition(ev) {
    var dialogName = ev.data.name;

    // Customize table dialog.
    if(dialogName == 'table' || dialogName == 'tableProperties') {

      var dialogDefinition  = ev.data.definition
        , dialog            = dialogDefinition.dialog
        , infoTab           = dialogDefinition.getContents('info')
        , cellSpacing       = infoTab.get('txtCellSpace')
        , cellPadding       = infoTab.get('txtCellPad')
        , border            = infoTab.get('txtBorder')
        , width             = infoTab.get('txtWidth')
        , advancedTab       = dialogDefinition.getContents('advanced')
        , classes           = advancedTab.get('advCSSClasses');

      // Set new default.
      cellSpacing['default'] = "0";
      cellPadding['default'] = "0";
      border['default'] = "0";
      width['default'] = '100%';
      classes['default'] = 'table table-responsive';

      // Add Bootstrap based styles:
      var bootstrapStyles = {
        padding: 1,
        type: 'vbox',
        children: [{
          // Title.
          type: 'html',
          id: 'bootstrapStylesText',
          html: '<strong>' + Drupal.t('Table appearance') + '</strong>'
        }]
      };

      var bootstrapStyleOptions = [
        { name : Drupal.t('Responsive'), 'class': 'table-responsive', 'default': true }
      , { name : Drupal.t('Striped'), 'class': 'table-striped', 'default': false }
      , { name : Drupal.t('Bordered'), 'class': 'table-bordered', 'default': false }
      , { name : Drupal.t('Hover rows'), 'class': 'table-hover', 'default': false }
      , { name : Drupal.t('Condensed'), 'class': 'table-condensed', 'default': false }
      ];

      for(var o = 0, option; o < bootstrapStyleOptions.length; o++) {
        option = bootstrapStyleOptions[o];

        bootstrapStyles.children.push({
          'type': 'checkbox',
          'id': option.class,
          'label' : option.name,
          'default': option.default,
          'onClick': function onClick () {
            var checkbox = this
              , advCSSClasses = dialog.getContentElement('advanced', 'advCSSClasses')
              , classes = (advCSSClasses.getValue() || '').split(' ').filter(function (value) {
                return Boolean(value);
              });

            if (checkbox.getValue() && classes.indexOf(checkbox.id) == -1) {
              classes.push(checkbox.id);
            } else if (classes.indexOf(checkbox.id) > -1) {
              classes = classes.filter(function (className) {
                return className != checkbox.id;
              });
            }

            advCSSClasses.setValue(classes.join(' '));
          }
        });
      }

      advancedTab.elements.push(bootstrapStyles);

      dialog.on('show', function () {
        if (this.getName() == 'tableProperties') {
          var editor    = this._.editor
            , selection = editor.getSelection()
            , ranges    = selection.getRanges()
            , selected  = selection.getSelectedElement()
            , table     = selected && selected.is('table') ? selected : null
            , classes, o, checkbox;

          if (!table && ranges.length > 0) {
            // Webkit could report the following range on cell selection (#4948):
            // <table><tr><td>[&nbsp;</td></tr></table>]
            if (CKEDITOR.env.webkit) ranges[0].shrink(CKEDITOR.NODE_ELEMENT);
            
            table = editor.elementPath(ranges[0].getCommonAncestor(true)).contains('table', 1);
          }

          if (!table) return;

          classes = (table.getAttribute('class') || '').split(' ').filter(function (value) {
            return Boolean(value);
          });

          for (var o = 0, option, checkbox; o < bootstrapStyleOptions.length; o++) {
            option = bootstrapStyleOptions[o];
            checkbox = this.getContentElement('advanced', option.class);

            checkbox.setValue(classes.indexOf(option.class) > -1);
          }
        }
      });
    }
  }

  /**
   * CKEditor instance ready callback to improve alignment commands to
   * make use of Bootstrap helper classes for alignment.
   */
  function alignmentCommandsSetup(event) {
    var editor = event.editor
      , alignments = ['left', 'right', 'center', 'block']
      , sideAlignments = ['left', 'right']
      , i, command, image;

    for (i = 0; i < alignments.length; i++) setupCommand(alignments[i]);

    /**
     * Setup the command listeners.
     */
    function setupCommand(alignment) {

      // Early return on no command found.
      if (!(command = editor.getCommand('justify' + alignment))) return;

      // Listener for execution of lateral alignment methods.
      if (sideAlignments.indexOf(alignment) > -1) command.on('exec', function (event) {
        if (image = getSelectedImage()) {
          // Remove/add alignment class.
          (getAlignment(image) == alignment ? removeAlignment : setAlignment)(image, alignment);
          
          // Stop propagation.
          event.cancel();
        }
      }, null, null, 1);

      // Listener for refresh alignment.
      command.on('refresh', function (event) {
        if (image = getSelectedImage()) {
          var currentAlignment = getAlignment(image)
            , newState;

          if (currentAlignment == alignment) newState = CKEDITOR.TRISTATE_ON;
          if (newState) newState = sideAlignments.indexOf(alignment) > -1 ? CKEDITOR.TRISTATE_OFF : CKEDITOR.TRISTATE_DISABLED;

          // Update element state.
          this.setState(newState);
          
          event.cancel();
        }
      }, null, null, 1);
    }

    /**
     * Helper method to remove current alignment from image element.
     */
    function removeAlignment(image) {
      for (i = 0; i < sideAlignments.length; i++) {
        image.removeClass('pull-' + sideAlignments[i]);
      }
      image.removeStyle('float');
      image.removeAttribute('align');
    }

    /**
     * Helper method to add alignment from image element.
     */
    function setAlignment(image, alignment) {
      // Make sure other alignment methods are disabled.
      removeAlignment(image, alignment);
      image.addClass('pull-' + alignment);
    }

    /**
     * Helper method to get the current selected image.
     */
    function getSelectedImage() {
      var selection       = editor.getSelection()
        , element         = selection.getSelectedElement()
        , isImage         = element && element.is('img')
        , realElementData = element && element.data('cke-realelement')
        , readOnly        = element && element.isReadOnly();

      return isImage && !realElementData && !readOnly ? element : null;
    }

    /**
     * Helper method to find out current image alignment.
     */
    function getAlignment(image) {
      if (image.hasClass('pull-left')) return 'left';
      if (image.hasClass('pull-right')) return 'right';
      if (['inherit', 'none'].indexOf(image.getStyle('float')) > -1) return 0;

      // Fallback to default align attribute.
      return image.getAttribute('align') || null;
    }
  }
})();
