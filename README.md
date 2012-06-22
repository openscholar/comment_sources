## Functionality available in this module

### API
* `comment_sources_sources()` - Get a list of comment sources supported by this module - currently Disqus, Facebook, and Drupal Core Comments
* `comment_sources_enabled_sources()` - Get a list of the currently enabled content sources. Takes into account whether the required modules are installed, and breaks out the individual fb_social presets, each of which can be configured independently
* `comment_sources_set_source($content_type, $source)` - Set the comment source for a content type
* `comment_sources_get_source($content_type)` - Get the comment source for a content type
* `comment_sources_recent_comments($source, $content_type, $count)` - Get recent comments

### Administration

* admin/structure/comment_sources - Configure comment sources for all content types
* admin/structure/types/manage/[content-type] - Configure comment source for a content type
* node edit page - Disable comments for disqus/Drupal/fb_social comments, depending on selected comment source for that content type. (Disabling fb_social comments depends on fb_social patch)
* comments/review - Review and approve Drupal comments, filterable by content type

## Not yet implemented

* Unit tests to validate that facebook comments are being displayed on node pages correctly
* Unit tests for saving/loading fb_social presets
* API to save comment configuration options, in addition to mapping from content type to comment type
