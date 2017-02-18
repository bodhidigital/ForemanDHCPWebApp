// js/dhcp.js

var modeSelButtons = jQuery('.mode-sel-bar a.show-table');

modeSelButtons.click(function() {
  var $jThis = jQuery(this);
  var showTable = jQuery(jQuery(this).data('target'));
  var otherTable = jQuery(modeSelButtons.not($jThis).data('target'));

  $jThis.closest('ul, ol').find('li.active').removeClass('active');
  $jThis.closest('li').addClass('active');

  otherTable.fadeOut(function() {
    showTable.fadeIn();
  });
});

// vim: set ts=2 sw=2 et syn=javascript:
