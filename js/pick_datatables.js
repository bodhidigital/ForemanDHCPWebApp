// js/pick_datatables.js

jQuery(document).ready(function() {
  var pickDataTables = jQuery('.records.records-tables .record table');

  pickDataTables.DataTable({
    responsive: true,
    aoColumnDefs: [ {
      bSortable: false,
      aTargets: [ 0, -1 ]
    }, {
      sType: "ip-addr",
      aTargets: [ 2 ]
    } ]
  });

  function cmp_ips(x, y) {
    function valid_ip(ip_arr) {
      if (4 != ip_arr.length)
        throw 'Invalid IP address `' + ip_arr.join('.') + '\'.';

      ip_arr.forEach(function(it, i) {
        if (0 == it.length || isNaN(it))
          throw 'Invalid IP address `' + ip_arr.join('.') + '\'.';
      });
    }

    var x_arr = x.split('.'),
        y_arr = y.split('.');

    valid_ip(x_arr);
    valid_ip(y_arr);

    for (i = 0; 4 > i; ++i) {
      x_seg = parseInt(x_arr[i], 10);
      y_seg = parseInt(y_arr[i], 10);

      if (x_seg != y_seg) {
        if (x_seg > y_seg)
          return 1;
        else
          return -1;
      }
    }

    return 0;
  }

  jQuery.fn.dataTableExt.oSort['ip-addr-asc']  = function(x, y) {
    var cmp_ips_result = cmp_ips(x, y);

    if (0 < cmp_ips_result)
      return 1;
    else if (0 > cmp_ips_result)
      return -1;
    else
      return 0;
  };

  jQuery.fn.dataTableExt.oSort['ip-addr-desc']  = function(x, y) {
    var cmp_ips_result = cmp_ips(x, y);

    if (0 < cmp_ips_result)
      return -1;
    else if (0 > cmp_ips_result)
      return 1;
    else
      return 0;
  };


  pickDataTables.find('thead tr th:contains(\'MAC \')').click();
});

// vim: set ts=2 sw=2 et syn=javascript:
