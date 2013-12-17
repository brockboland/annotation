<?php

/**
 * Annotation entity api controller class
 */
class AnnotationAPIController extends EntityAPIController {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function load($ids = array(), $conditions = array()) {
    $entities = parent::load($ids, $conditions);
    foreach ($entities as $id => $entity) {
      //$entity->annotator_schema_version = ANNOTATOR_SCHEMA_VERSION;
      $op = 'load';
      drupal_alter('annotation', $entity, $op);
      $entities[$id] = $entity;
    }
    return $entities;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $op = 'save';
    drupal_alter('annotation', $entity, $op);
    parent::save($entity, $transaction);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('annotation', $entity);

    $account = $wrapper->uid->value();
    // Since uid is configured as a user property, $account should be an
    // account object, but just in case
    if (is_numeric($account)) {
      $account = user_load($account);
    }
    if (is_object($account)) {
      $uri = $wrapper->uri->value(array('sanitize' => TRUE));

      $content['author'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' =>t('Created by'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_author',
        '#field_type' => 'text',
        '#entity_type' => 'annotation',
        '#bundle' => $entity->type,
        '#items' => array(
          array('value' => theme('username', array('account' => $account)))
        ),
        '#formatter' => 'text_default',
        0 => array(
          '#markup' => theme('username', array('account' => $account))
        )
      );
    }

    $node = $wrapper->nid->value();
    // nid property is configured as a node, so this should be a node object,
    // but just in case
    if (is_numeric($node)) {
      $node = node_load($node);
    }

    if (is_object($node)) {
      $vid = $wrapper->vid->value();
      if ($vid > 0) {
        $link = l($node->title, 'node/' . $node->nid . '/revisions/' . $vid . '/view');
      }
      else {
        $link = l($node->title, 'node/' . $node->nid);
      }
      $content['node'] = array(
        '#theme' => 'field',
        '#weight' => 0,
        '#title' =>t('Node'),
        '#access' => TRUE,
        '#label_display' => 'above',
        '#view_mode' => 'full',
        '#language' => LANGUAGE_NONE,
        '#field_name' => 'field_fake_node',
        '#field_type' => 'text',
        '#entity_type' => 'annotation',
        '#bundle' => $entity->type,
        '#items' => array(
          array('value' => $link)
        ),
        '#formatter' => 'text_default',
        0 => array(
          '#markup' => $link
        )
      );
    }

    $formatted_date = format_date($wrapper->created->value(array('sanitize' => TRUE)));
    $content['date'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Date'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_author',
      '#field_type' => 'text',
      '#entity_type' => 'annotation',
      '#bundle' => $entity->type,
      '#items' => array(
        array('value' => $formatted_date)
      ),
      '#formatter' => 'text_default',
      0 => array(
        '#markup' => $formatted_date
      )
    );


    $content['quote'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Quote'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_quote',
      '#field_type' => 'text',
      '#entity_type' => 'annotation',
      '#bundle' => $entity->type,
      '#items' => array(
        array('value' => $entity->quote)
      ),
      '#formatter' => 'text_default',
      0 => array(
        '#markup' => '<blockquote cite="' . check_plain($entity->uri) . '">' . check_plain($entity->quote) . '</blockquote>'
      )
    );
    $content['text'] = array(
      '#theme' => 'field',
      '#weight' => 0,
      '#title' =>t('Annotation'),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_text',
      '#field_type' => 'text',
      '#entity_type' => 'annotation',
      '#bundle' => $entity->type,
      '#items' => array(
        array('value' => $entity->text)
      ),
      '#formatter' => 'text_default',
      0 => array(
        '#markup' => check_plain($entity->text)
      )
    );

    $content['#view_mode'] = $view_mode;
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}

function get_properties($obj, $values=false) {
  $obj_dump  = print_r($obj, 1);
  $matches =  array();
  preg_match_all('/^\s+\[(\w+).*\] => (\w*)/m', $obj_dump, $matches);
  if ($values) {
    $output = array();
    foreach ($matches[1] as $key => $property) {
      $output[$property] = $matches[2][$key];
    }
    return $output;
  }
  else {
    return $matches[1];
  }
}
