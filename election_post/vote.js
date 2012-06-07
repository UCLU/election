/**
 * @file
 * JavaScript for the ballot paper form in the Election Post module.
 */
(function ($) {

  $(document).ready(function() {

    var inputs = $('#election-post-vote-form').find(':input[name^="candidate"]');

    inputs.bind('click change', function() {
      var elem = $(this), others = inputs.not(elem), val = elem.val();
      others.find('option[disabled]').removeAttr('disabled');
      if (val.length && val != 'NONE') {
        others.find('option[value="' + val + '"]').not(':selected').attr('disabled', 'disabled');
      }
    });

  });

})(jQuery);
