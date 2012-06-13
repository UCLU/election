<?php
/**
 * @file
 * Sort by nominations status.
 *
 * @todo
 */
class views_handler_sort_election_nstatus extends views_handler_sort {

  function query() {
    $this->query->add_orderby('rand');
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['order']['#access'] = FALSE;
  }

}
