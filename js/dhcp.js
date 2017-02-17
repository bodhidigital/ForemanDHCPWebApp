// js/dhcp.js

var modeSelButtons = jQuery('.mode-sel-bar button');

modeSelButtons.click(function() {
  var $jThis = jQuery(this);
  var showTable = jQuery(jQuery(this).data('toggle'));
  var otherTable = jQuery(modeSelButtons.not($jThis).data('toggle'));

  otherTable.fadeOut(function() {
    showTable.fadeIn();
  });
});

// vim: set ts=2 sw=2 et syn=javascript:
