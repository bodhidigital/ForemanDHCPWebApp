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
    }, {
      sType: "hostname",
      aTargets: [ 1 ]
    }, {
      sType: "ip-addr",
      aTargets: [ 2 ]
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
    }, {
      sType: "ip-addr",
      aTargets: [ 1 ]
    } ]
  });


  function cmp_ips(x, y) {
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

  function cmp_hostnames(x, y) {
    var x_arr = x.split('.'),
        y_arr = y.split('.');

    valid_hostname(x_arr);
    valid_hostname(y_arr);

    // Imply the .local domain on short hostnames
    if (1 == x_arr.length)
      x_arr.push('local');

    if (1 == y_arr.length)
      y_arr.push('local');

    var lt_len = x_arr.length <= y_arr.length ? x_arr.length : y_arr.length;

    for (i = 1; lt_len >= i; ++i) {
      x_seg = x_arr[x_arr.length - i];
      y_seg = y_arr[y_arr.length - i];

      if (x_seg != y_seg) {
        if (x_seg > y_seg)
          return 1;
        else
          return -1;
      }
    }

    if (x_arr.length > y_arr.length)
      return -1;
    else if (x_arr.length < y_arr.length)
      return 1;
    else
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

  jQuery.fn.dataTableExt.oSort['hostname-asc']  = function(x, y) {
    var cmp_hostnames_result = cmp_hostnames(x, y);

    if (0 < cmp_hostnames_result)
      return 1;
    else if (0 > cmp_hostnames_result)
      return -1;
    else
      return 0;
  };

  jQuery.fn.dataTableExt.oSort['hostname-desc']  = function(x, y) {
    var cmp_hostnames_result = cmp_hostnames(x, y);

    if (0 < cmp_hostnames_result)
      return -1;
    else if (0 > cmp_hostnames_result)
      return 1;
    else
      return 0;
  };

  dataTablesWrap.find('.record table thead tr th:contains(\'IP \')').click();
});

// vim: set ts=2 sw=2 et syn=javascript:
