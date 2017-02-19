// js/validate.js

function valid_ip(ip_arr) {
  var err_str = 'Invalid IP address `' + ip_arr.join('.') + '\'.';

  if (4 != ip_arr.length)
    throw err_str;

  ip_arr.forEach(function(it, i) {
    if (0 == it.length || isNaN(it))
      throw err_str;

    var it_int = parseInt(it);

    if (0 > it_int || 255 < it_int)
      throw err_str;
    else if (3 == i && (0 == it_int || 255 == it_int))
      throw err_str;
  });
}

function valid_hostname(hostname_arr) {
  var err_str = 'Invalid hostname`' + hostname_arr.join('.') + '\'.';

  if (0 == hostname_arr.length)
    throw err_str;

  var total_length = hostname_arr.length - 1;

  hostname_arr.forEach(function(it, i) {
    if (1 > it.length || 63 < it.length)
      throw err_str;
    else if ('-' == it[0])
      throw err_str;
    else if ('-' == it[it.length - 1])
      throw err_str;
    else if (0 != it.replace(/[a-zA-Z0-9\-]/g, '').length)
      throw err_str;

    total_length += it.length;
  });

  if (253 < total_length)
    throw err_str;
}

// vim: set ts=2 sw=2 et syn=javascript:
