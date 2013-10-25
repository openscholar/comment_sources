/**
 * Makes option to use existing comments or hide and switch to new source.
 *
 * Desired behavior is like a radio option: only one may be selected at once.
 */
(function ($) {
    Drupal.behaviors.comment_sources = {
        attach: function (context) {
            var source = Drupal.settings.comment_sources.source;
            var type_source = Drupal.settings.comment_sources.type_source;

            if (!source.length || !type_source.length) {
                return;
            }

            var original = ':input[name="' + source + '_status"]';
            var switched = ':input[name="comment_sources_switch"]';

            if (!original.length || !switched.length) {
                return;
            }

            // Only allow the original source or the switched source to be checked.
            $(original).click(function () {
                if ($(original).attr('checked')) {
                    $(switched).attr('checked', false);
                }
            });
            $(switched).click(function () {
                if ($(switched).attr('checked')) {
                    $(original).attr('checked', false);
                }
            });
        }
    };
})(jQuery);
