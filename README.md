## Functionality available in this module

### API
* `comment_sources_sources()` - Get a list of comment sources supported by this module - currently Disqus, Facebook, and Drupal Core Comments
* `comment_sources_enabled_sources()` - Get a list of the currently enabled content sources. Takes into account whether the required modules are installed, and breaks out the individual fb_social presets, each of which can be configured independently
* `comment_sources_set_source($content_type, $source)` - Set the comment source for a content type

### Administration

* admin/structure/comment_sources - Configure comment sources for all content types

## Not yet implemented

* Unit tests to validate that correct comments are being displayed on node pages
* Unit tests for saving/loading fb_social presets
* API to save comment configuration options, in addition to mapping from content type to comment type
* UI to administer default comment settings for content type
* UI to administer comment settings for node
* API for recent comments
* UI to moderate comments
