## Functionality available in this module

### API
* `comment_sources_sources()` - Get a list of comment sources supported by this module - currently Disqus, Facebook, and Drupal Core Comments
* `comment_sources_enabled_sources()` - Get a list of the currently enabled content sources. Takes into account whether the required modules are installed, and breaks out the individual fb_social presets, each of which can be configured independently
* `comment_sources_set_source($content_type, $source)` - Set the comment source for a content type
* `comment_sources_get_source($content_type)` - Get the comment source for a content type
* `comment_sources_supports_recent_comments($source)` - Whether a comment source supports the 'recent comments' feature
* `comment_sources_recent_comments($source, $count)` - Get recent comments
* `hook_comment_sources()` - Add additional comment sources                                                     

### Administration

* admin/config/services/comment_sources - Configure comment sources for all content types
* admin/structure/types/manage/[content-type] - Configure comment source for a content type
* node edit page - Disable comments for disqus/Drupal/fb_social comments, depending on selected comment source for that content type. (Disabling fb_social comments depends on fb_social patch)
* comments/review - Review and approve Drupal comments, filterable by content type