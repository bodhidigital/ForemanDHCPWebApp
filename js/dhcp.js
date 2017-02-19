// js/dhcp.js

jQuery(document).ready(function() {
  var recordsNav      = jQuery('#records-nav').find('ul.nav, ol.nav');
  var modeSelButtons  = recordsNav.find('.show-records');
  var allRecords      = jQuery('.record');

  modeSelButtons.click(function() {
    var $jThis = jQuery(this);

    var closestLi = $jThis.closest('li');

    if (closestLi.hasClass('active'))
      return;

    var showTarget  = $jThis.data('target');
    var showRecord  = jQuery('.' + showTarget  + '-record');
    var hideRecords = allRecords.not(showRecord);

    if ($jThis.hasClass('ignore-click'))
      return;

    modeSelButtons.addClass('ignore-click');

    recordsNav.find('li.active').removeClass('active');

    jQuery.when(hideRecords.fadeOut()).then(function() {
      closestLi.addClass('active');

      showRecord.fadeIn();

      modeSelButtons.removeClass('ignore-click');
    });
  });
});

// vim: set ts=2 sw=2 et syn=javascript:
