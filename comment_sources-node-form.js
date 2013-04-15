
(function ($) {

Drupal.behaviors.commentFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.comment-node-settings-form', context).drupalSetSummary(function (context) {
      var vals = [];
      var comment_status = $('.form-item-comment input:checked', context).next('label').text();
      if (comment_status) {
        vals.push(comment_status);
      }
      if ($('.form-item-fb-social-status input').is(":checked")) {
        vals.push($('.form-item-fb-social-status input', context).next('label').text());
      }

      if ($('.form-item-disqus-status input').is(":checked")) {
        vals.push($('.form-item-disqus-status input', context).next('label').text());
      }

      return Drupal.checkPlain(vals.join(', '));
    });

    // Provide the summary for the node type form.
    $('fieldset.comment-node-type-settings-form', context).drupalSetSummary(function(context) {
      var vals = [];

      var comment_source = $(".form-item-comment-source option:selected", context).text();
      vals.push($(".form-item-comment-source option:selected", context).text());

      if ($(".form-item-comment-source option:selected", context).val() == 'comment') {
          // Default comment setting.
          vals.push($(".form-item-comment select option:selected", context).text());

          // Threading.
          var threading = $(".form-item-comment-default-mode input:checked", context).next('label').text();
          if (threading) {
            vals.push(threading);
          }

          // Comments per page.
          var number = $(".form-item-comment-default-per-page select option:selected", context).val();
          vals.push(Drupal.t('@number comments per page', {'@number': number}));
      }

      return Drupal.checkPlain(vals.join(', '));
    });
  }
};

})(jQuery);
