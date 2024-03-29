/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function(e,t,n,r,i){e("article.node-thought footer a.more").live("click",function(t){var n=e(this).closest("article").find(">.content");n.toggleClass("expanded");e(this).text(n.hasClass("expanded")?"-":"+");n.hasClass("expanded")||n.animate({scrollTop:0});t.preventDefault()});e("article.node-thought .content[data-link]").live("click",function(t){n.location=e(this).attr("data-link")});e("article.node-thought .content[data-link] a").live("click",function(e){e.preventBubble()});e(r).ready(function(){e("article.node-thought .content.large").each(function(){var t=e(this).find("> .wrapper").outerHeight(!0),n=parseInt(e(this).css("max-height"));t-5<n&&e(this).closest("article").find("footer a.more").remove()})});e("article.node-thought.new .add-source").live("click",function(t){t.preventDefault();e(this).closest(".node-thought").find(".source-wrapper").slideDown().find("input").focus();e(this).hide("fast")});t.behaviors.my_custom_behavior={attach:function(e,t){}}})(jQuery,Drupal,this,this.document);