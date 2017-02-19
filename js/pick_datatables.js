// js/pick_datatables.js

jQuery(document).ready(function() {
  var pickDataTables = jQuery('.records.records-tables .record table');

  pickDataTables.DataTable({
    responsive: true,
    aoColumnDefs: [ { 
        bSortable: false,
        aTargets: [ 0, -1 ]
    } ]
  });

  pickDataTables.find('thead tr th:contains(\'IP \')').click();
});

// vim: set ts=2 sw=2 et syn=javascript:
