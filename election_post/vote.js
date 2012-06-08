/**
 * @file
 * JavaScript for the ballot paper form in the Election Post module.
 */
(function ($) {

  $(document).ready(function() {

    var inputs = $('#election-post-vote-form').find(':input[name^="candidate"]'),
      disableOptions = function(e, others) {
        var val = e.val(), prev = e.data('previous_value');
        if (typeof prev != undefined) {
          others.find('option[value="' + prev + '"][disabled]').removeAttr('disabled');
        }
        if (val != '' && val != 'NONE') {
          others.find('option[value="' + val + '"]').not(':selected').attr('disabled', 'disabled');
        }
        e.data('previous_value', val);
      };

    inputs.each(function() {
      var e = $(this), others = inputs.not(e);
      disableOptions(e, others);
      e.change(function() {
        disableOptions(e, others);
      });
    });

  });

})(jQuery);
