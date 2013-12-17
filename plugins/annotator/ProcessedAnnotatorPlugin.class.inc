<?php

/**
 * Annotator ctools plugin
 */
class ProcessedAnnotatorPlugin extends AnnotatorPlugin {
  public function setup() {
    drupal_add_js(drupal_get_path('module', 'annotation') . '/js/annotator_processed.js');
  }
}
