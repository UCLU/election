/**
 * @file
 * JavaScript for the ballot paper form in the Election Post module.
 */
(function ($) {

  $(document).ready(function () {

    var select_elems = $('.election-candidate-preference-select'),
      disableOptions = function (triggering_elem, others) {
        var val = triggering_elem.val(), prev = triggering_elem.data('previous_value');
        if (typeof prev !== undefined) {
          others.find('option[value="' + prev + '"][disabled]').removeAttr('disabled');
        }
        if (val !== '' && val !== 'NONE') {
          others.find('option[value="' + val + '"]').not(':selected').attr('disabled', 'disabled');
        }
        triggering_elem.data('previous_value', val);
      };

    select_elems.each(function () {
      var this_elem = $(this), others = select_elems.not(this_elem);
      disableOptions(this_elem, others);
      this_elem.change(function () {
        disableOptions(this_elem, others);
      });
    });

  });

}(jQuery));
