(function ($) {
  Drupal.behaviors.annotatorProcessed = {
    attach: function (context, settings) {

      Annotator.Plugin.Processed = (function() {
        function Processed(element, options) {
          this.element = element;
          this.options = options;
        }

        Processed.prototype.pluginInit = function() {
          // Add checkbox for processed status on the annotation edit form
          this.annotator.editor.addField({
            type:'checkbox',
            label: Drupal.t('Mark as processed'),

            // When loading the form, check or uncheck the box
            load: function(field, annotation) {
              var input = $(field).find(':input');
              input.prop('checked', annotation.status > 0);
            },

            // When saving, set the status flag on the annotation
            submit: function(field, annotation) {
              var input = $(field).find(':input');
              annotation.status = input.prop('checked') ? 1 : 0;
            }
          });

          // Add processed status to the annotation modal view
          this.annotator.viewer.addField({
            load: function(field, annotation) {
              if (annotation.status > 0)
                $(field).html("<strong>Marked as processed</strong>");
              else
                $(field).html("<strong>NOT</strong> marked as processed");
            }
          });


          // Update the highlight classes on any annotation change events
          this.annotator.subscribe('annotationsLoaded', this.updateHighlights);
          this.annotator.subscribe('annotationCreated', this.updateHighlights);
          this.annotator.subscribe('annotationUpdated', this.updateHighlights);
          this.annotator.subscribe('annotationDeleted', this.updateHighlights);
        };


        // Update the hightlight classes on items that are processed
        Processed.prototype.updateHighlights = function(annotations) {
          // Some events pass an array of annotations, some pass just one
          if (!(annotations instanceof Array)) {
            annotations = [annotations];
          }

          for (i in annotations) {
            annotation = annotations[i];

            // Loop over the highlights included in the annotation
            // (should only be 1)
            $.each(annotation.highlights, function(index, highlight) {
              // Add a class for the annotation ID, while we're at it
              if (!$(highlight).hasClass('annotation-id-' + annotation.id)) {
                $(highlight).addClass('annotation-id-' + annotation.id)
              }

              // If processed, add processed class
              if (annotation.status > 0) {
                $(highlight).addClass('highlight-processed');
              }
              // If not, make sure it's removed (necessary for updating
              // annotatoins that are already displayed)
              else {
                $(highlight).removeClass('highlight-processed');
              }
            });
          }
        };

        return Processed;
      })();

      Drupal.Annotator.annotator('addPlugin', 'Processed');

    }
  };
})(jQuery);
