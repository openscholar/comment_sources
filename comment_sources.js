/**
 * @file
 * JavaScript for the comment_sources module.
 */

function disqus_track_comments(params){
  jQuery.ajax({
      type: 'GET',
      url: Drupal.settings.basePath + 'comment_sources/record_comment',
      data :
        {
          nid : Drupal.settings.comment_sources.nid,
          source: 'disqus',
        }
  });
}

/**
 * Retrieve the comment text from Facebook, and then save the comment. Requires page to be accessible by
 * Facebook to retrieve comment text.
 * @param params
 */
function fb_social_track_comments(params){
  jQuery.ajax({
    type: 'GET',
    url: Drupal.settings.basePath + 'comment_sources/record_comment',
    data :
      {
        nid : Drupal.settings.comment_sources.nid,
        source: 'fb_social'
      }
  });
}



