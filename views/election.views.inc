<?php


function election_views_data_alter(&$data) {

  $data = array();

  $data['election'] = array(
    'table' => array(
      'group' => t('Elections'),
      'base' => array(
        'field' => 'election_id',
        'title' => t('Election'),
      ),
    ),
    'title' => array(
      'title' => t('Election title'),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
    'created' => array(
      'title' => t('Creation time'),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
    'changed' => array(
      'title' => t('Last modified time'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
  );

  $data['election_post'] = array(
    'table' => array(
      'group' => t('Elections'),
      'join' => array(
        'left_table' => 'election_election',
        'left_field' => 'election_id',
        'field' => 'election_id',
        'table' => 'election',
        'handler' => 'views_join', // @todo extend this
      ),
    ),
    'title' => array(
      'title' => t('Post title'),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
    'post_id' => array(
      'title' => t('Post ID'),
      'help' => t('The post ID of the post'),
      'argument' => array(
        'name field' => 'title',
        'numeric' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
  );

  return $data;

}
