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
(function(e,t,n,r,i){e("article.node-thought footer a.more").live("click",function(t){var n=e(this).closest("article").find(">.content");n.toggleClass("expanded");e(this).text(n.hasClass("expanded")?"-":"+");n.hasClass("expanded")||n.animate({scrollTop:0});t.preventDefault()});e(r).ready(function(){e("article.node-thought .content.large").each(function(){var t=e(this).find("> .wrapper").outerHeight(!0),n=parseInt(e(this).css("max-height"));t-5<n&&e(this).closest("article").find("footer a.more").remove()})});t.behaviors.my_custom_behavior={attach:function(e,t){}}})(jQuery,Drupal,this,this.document);