/**
 * @file
 * Registers templates for CKEditor.
 */

var definition = CKEDITOR.getTemplates('default') || {
  imagesPath: Drupal.settings.basePath,
  templates: []
};

var templates = Drupal.settings.CKEditor && Drupal.settings.CKEditor.templates || [];

newTemplates:
for (n in templates) {

  // Overwrite duplicates.
  for (t in definition.templates) {
    if (definition.templates[t].title == templates[n].title) {
      definition.templates[t] = templates[n];
      continue newTemplates;
    }
  }

  definition.templates.push(templates[n]);
}

CKEDITOR.addTemplates('default', definition);
