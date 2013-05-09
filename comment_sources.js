/**
 * @file
 * JavaScript for the comment_sources module.
 */

(function ($) {
  Drupal.behaviors.comment_sources_disqus_track_comments = function (params) {
      /**
       * Records when a Disqus comment is made.
       */
      $.ajax({
            type: 'GET',
            url: Drupal.settings.basePath + 'comment_sources/record_comment',
            data :
              {
                nid : Drupal.settings.comment_sources.nid,
                source: 'disqus',
              }
        });
      }

  Drupal.behaviors.comment_sources_fb_social_track_comments = function (params) {
      /**
       * Records when a Facebook comment is made.
       *
       * Note: Requires page to be accessible by Facebook to retrieve comment text.
       */
      $.ajax({
        type: 'GET',
        url: Drupal.settings.basePath + 'comment_sources/record_comment',
        data :
          {
            nid : Drupal.settings.comment_sources.nid,
            source: 'fb_social'
          }
      });
  }
})(jQuery);

