// js/pick_datatables.js

jQuery(document).ready(function() {
  var dataTablesWrap = jQuery('.records.records-tables');

  dataTablesWrap.find('.record.reserve-record table').dataTable({
    responsive: true,
    columnDefs: [ {
      type: 'ip-address',
      targets: 2
    } ],
    aoColumnDefs: [ { 
      bSortable: false,
      aTargets: [ 0, -1 ]
    } ]
  });

  dataTablesWrap.find('.record.lease-record table').dataTable({
    responsive: true,
    columnDefs: [ {
      type: 'ip-address',
      targets: 1
    } ],
    aoColumnDefs: [ { 
      bSortable: false,
      aTargets: [ 0, -1 ]
    } ]
  });

  dataTablesWrap.find('.record table thead tr th:contains(\'IP \')').click();
});

// vim: set ts=2 sw=2 et syn=javascript:
