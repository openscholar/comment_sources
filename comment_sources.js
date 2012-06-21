/**
 * @file
 * JavaScript for the comment_sources module.
 */


function disqus_track_comments(params){
  jQuery.ajax({
      type: 'GET',
      url: 'http://localhost/~jlicht/os7/comment_sources/record_comment',
      data :
        {
          nid : Drupal.settings.comment_sources.nid,
          source: 'disqus',
          external_id: params.id,
          subject: params.text.substring(0,64)
        }
  });
}

