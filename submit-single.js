/**
 * @file
 * JavaScript to prevent multiple clicks of a submit button for a given form.
 */
(function ($) {

  Drupal.behaviors.election_submit_single = {

    attach: function (context, settings) {

      $('form', context).submit(function () {
        $(this).find(':submit').attr('disabled', 'disabled');
      });

    }

  };

}(jQuery));

