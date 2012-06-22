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
          external_id: params.id,
          subject: params.text.substring(0,64)
        }
  });
}

/**
 * Retrieve the comment text from Facebook, and then save the comment. Requires page to be accessible by
 * Facebook to retrieve comment text.
 * @param params
 */
function fb_social_track_comments(params){
  var commentQuery = FB.Data.query("SELECT text FROM comment WHERE post_fbid='" + params.commentID +
      "' AND object_id IN (SELECT comments_fbid FROM link_stat WHERE url='" + params.href + "')");
  FB.Data.waitOn([commentQuery], function () {
    var commentRow = commentQuery.value[0];
    jQuery.ajax({
      type: 'GET',
      url: Drupal.settings.basePath + 'comment_sources/record_comment',
      data :
        {
          nid : Drupal.settings.comment_sources.nid,
          source: 'fb_social',
          external_id: params.commentID,
          subject: (commentRow != undefined) ? commentRow.text.substring(0,64) : ''
        }
    });
  });
}



