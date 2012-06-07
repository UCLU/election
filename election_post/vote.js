/**
 * @file
 * JavaScript for the ballot paper form in the Election Post module.
 */
(function ($) {

  $(document).ready(function() {

    var inputs = $('#election-post-vote-form').find(':input[name^="candidate"]'),
      disableOtherOptions = function(select_element, others) {
        var val = select_element.val();
        if (val.length && val != 'NONE') {
          others.find('option[value="' + val + '"]').not(':selected').attr('disabled', 'disabled');
        }
      };

    inputs.each(function() {
      var e = $(this), others = inputs.not(e);
      disableOtherOptions(e, others);
      e.change(function() {
        others.find('option[disabled]').removeAttr('disabled');
        disableOtherOptions(e, others);
      });
    });

  });

})(jQuery);
