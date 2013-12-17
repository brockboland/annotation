<?php

$plugin = array(
  'name' => 'processed',
  'label' => t('Mark Processed'),
  'module' => 'annotator',
  'handler' => array(
    'class' => 'ProcessedAnnotatorPlugin',
  ),
);
