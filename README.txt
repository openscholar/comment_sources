# Comment Sources

Makes it possible to manage multiple sources (i.e. 3rd party services) of
comments on nodes and node types.

## Credits

 - Jeffrey Licht https://github.com/jlicht
 - Ferdi Alimadhi https://github.com/Ferdi
 - Richard Brandon http://github.com/rbran100
 - Oren Robinson http://github.com/baisong
 - OpenScholar http://openscholar.harvard.edu

Comment Sources module currently integrates node comment settings with 2
additional 3rd party comment projects:

 - Facebook social plugins integration (fb_social)
 - Disqus (disqus)

Once Comment sources is enabled, users can select a different comment source
(Facebook, Disqus, or Drupal) for every node type. Currently adding the ability
to override on a per-node basis as well.

The module provides a number of namespaced hooks to allow more comment sources
to be easily provided in other modules.

## API

- `comment_sources_sources()`
  Get a list of comment sources supported by this module - currently Disqus,
  Facebook, and Drupal Core Comments
- `comment_sources_enabled_sources()`
  Get a list of the currently enabled content sources. Takes into account
  whether the required modules are installed, and breaks out the individual
  fb_social presets, each of which can be configured independently
- `comment_sources_set_source($content_type, $source)`
  Set the comment source for a content type
- `comment_sources_get_source($content_type)`
  Get the comment source for a content type
- `comment_sources_supports_recent_comments($source)`
  Whether a comment source supports the 'recent comments' feature
- `comment_sources_recent_comments($source, $count)`
  Get recent comments
- `hook_comment_sources()`
  Add additional comment sources                                                     

## Administration

* admin/config/services/comment_sources - Configure comment sources for all
  content types
* admin/structure/types/manage/[content-type] - Configure comment source for a
  content type
* node edit page - Disable comments for disqus/Drupal/fb_social comments,
  depending on selected comment source for that content type. (Disabling
  fb_social comments depends on fb_social patch)
* comments/review - Review and approve Drupal comments, filterable by content
  type

## Schema

One table: 'comment_sources_comments'

One record in the comment_sources_comments table represents an *overridden*
node-source relationship. In many cases, nodes with comments enabled will not
have records in the comment_sources_comments table.

A single node may have more than one record, one: for each comment source. There
will only ever be at most 1 active comment source for an individual node.

### Examples

- Let's say comments are set to "No comments" for blog posts. When new blog
posts are added, users have no option to turn on comments for individual nodes,
and no records are added to the database for new nodes.

- Let's say comments are set to any source, i.e. Drupal comments. When new blog
posts are added, Drupal comments will be enabled by default. If the user keeps
this default setting, no records are added for the new nodes. But, if a user
un-checks the "Drupal comments" checkbox, then a record will be added to the
database, setting the active source for the overridden node to "No comments".

- When a comment is added to a node, a record is added for that node, setting
it's overridden active source to the current source, and marking "has_comments"
as "1".

- Let's say that comments of one source have been enabled for a while, and then
the comment source is changed to something else, or turned off. Just as
expected, new nodes will take the new source by default. But any nodes that have
been previously overridden by (1) turning comments off on an individual node, or
(2) a node receiving comments, will keep their previous overridden setting,
without any change to the database.

- For overridden nodes, users will always have the option to switch to another
comment source if the current active source for that type is different from the 
overridden value. In these cases, the previous source's record for this node 
is set to "0" (inactive), and a record is added/updated for the current source
and this node set to "1" (active).
