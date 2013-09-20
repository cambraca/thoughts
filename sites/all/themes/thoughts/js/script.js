/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

  $('article.node-thought footer a.more').live('click', function(event) {
    var content = $(this).closest('article').find('>.content');
    content.toggleClass('expanded');
    $(this).text(content.hasClass('expanded') ? '-' : '+');
    if (!content.hasClass('expanded'))
      content.animate({scrollTop: 0});
    event.preventDefault();
  });

  $('article.node-thought .content[data-link]').live('click', function(event) {
    window.location = $(this).attr('data-link');
  });

  $('article.node-thought .content[data-link] a').live('click', function(event) {
    event.preventBubble();
  });

  $(document).ready(function() {
    $('article.node-thought .content.large').each(function() {
      var contentHeight = $(this).find('> .wrapper').outerHeight(true);
      var maxHeight = parseInt($(this).css('max-height'));
      if ((contentHeight - 5) < maxHeight)
        $(this).closest('article').find('footer a.more').remove();
    })
  });
  
  $('article.node-thought.new .add-source').live('click', function(event) {
    event.preventDefault();
    $(this).closest('.node-thought').find('input[name=source]').show('fast').focus();
    $(this).hide('fast');
  });

  // To understand behaviors, see https://drupal.org/node/756722#behaviors
  Drupal.behaviors.my_custom_behavior = {
    attach: function(context, settings) {



    }
  };


})(jQuery, Drupal, this, this.document);
