<?php
/**
 * @file
 * Referendum-specific alterations to the 'election_posts' default View (provided by the Election Post module).
 *
 * This is part of an implementation of hook_views_default_views_alter().
 *
 * @see election_referendum_views_default_views_alter()
 */

/* Display: Referendum motions */
$handler = $view->new_display('block', 'Referendum motions', 'block_referendum');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Motions';
$handler->display->display_options['defaults']['empty'] = FALSE;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'There are currently no motions in this referendum';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'There are currently no motions in this referendum.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
$handler->display->display_options['empty']['area']['tokenize'] = 0;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Election: Published or admin */
$handler->display->display_options['filters']['published_or_admin']['id'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['table'] = 'election';
$handler->display->display_options['filters']['published_or_admin']['field'] = 'published_or_admin';
$handler->display->display_options['filters']['published_or_admin']['group'] = 1;
/* Filter criterion: Election: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'election';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = 'referendum';
$handler->display->display_options['filters']['type']['group'] = 1;
