<?php // lib/post.inc

function valid_ip($ip) {
  $ip_arr = explode('.', $ip);

  if (4 != count($ip_arr))
    return FALSE;

  for ($i = 0; count($ip_arr) > $i; ++$i) {
    $ip_seg = $ip_arr[$i];

    if (0 == count($ip_seg) || !is_numeric($ip_seg))
      return FALSE;

    $ip_seg_int = (int)ip_seg;

    if (0 > $ip_seg_int || 255 < $ip_seg_int)
      return FALSE;
    else if (3 == i && (0 == $ip_seg_int || 255 == $ip_seg_int))
      return FALSE;
  }

  return TRUE;
}

function valid_hostname($hostname) {
  $hostname_arr = explode('.', $hostname);

  if (0 == count($hostname_arr))
    return FALSE;

  $total_length = count($hostname_arr) - 1;

  for ($i = 0; count($hostname_arr) > $i; ++$i) {
    $hostname_seg = $hostname_arr[$i];

    if (1 > count($hostname_seg) || 63 < count($hostname_seg))
      return FALSE;
    else if ('-' == $hostname_seg[0])
      return FALSE;
    else if ('-' == $hostname_seg[count($hostname_seg) - 1])
      return FALSE;
    else if (0 != count(preg_replace('/[a-zA-Z0-9\-]/g', '', $hostname_seg)))
      return FALSE;

    $total_length += count($hostname_seg);
  }

  if (253 < $total_length)
    return FALSE;

  return TRUE;
}

function valid_mac($mac) {
  $mac_arr = explode(':', $mac);

  if (6 != count($mac_arr))
    return FALSE;

  foreach ($mac_arr as $mac_seg) {
    if (2 != count($mac_seg))
      return FALSE;
    else if (0 != count(preg_replace('/[a-f0-9]/g', '', $mac_seg)))
      return FALSE;
  }

  return TRUE;
}

function valid_filename($filename) {
  if ('/' == $filename[0])
    return FALSE;

  return TRUE;
}

function valid_next_server($next_server) {
  if (!valid_ip($next_server) &&
      (!valid_hostname($next_sever) || $next_server == gethostbyname($next_server)))
    return FALSE;
  
  return TRUE;
}

function handle_delete() {
  global $notify_error;
  global $record_manager;

  assert(isset($_POST['del_ip']));

  $del_target = $_POST['del_ip'];

  if (!valid_ip($del_target)) {
    $notify_error = 'Invalid IP address: `' . $del_target . '\'.';
    return;
  }

  try {
    $record_manager->deleteReq($del_target);
  } catch (Exception $e) {
    $notify_error = (string)$e;
  }
}

function handle_add() {
  global $notify_error;
  global $record_manager;

  assert(isset($_POST['hostname']));
  assert(isset($_POST['ip']));
  assert(isset($_POST['mac']));

  $hostname     = $_POST['hostname'];
  $target       = $_POST['ip'];
  $mac          = $_POST['mac'];

  $filename     = !empty($_POST['filename'])   ? $_POST['filename']   : NULL;
  $next_server  = !empty($_POST['nextServer']) ? $_POST['nextServer'] : NULL;;

  $general_records = $record_manager->getReq();

  $all_records = $general_records['reservations'] + $general_records['leases'];

  foreach ($all_records as $record) {
    if ($mac      == $record['mac'] ||
        $target   == $record['ip']  ||
        $hostname == $record['hostname']) {
      $notify_error = "IP, MAC, or hostname already in use.";
      return;
    }
  }

  if (!valid_hostname($hostname)) {
    $notify_error = 'Invalid hostname: `' . $hostname . '\'.';
    return;
  }

  if (!valid_ip($ip)) {
    $notify_error = 'Invalid IP: `' . $ip . '\'.';
    return;
  }

  if (!valid_mac($mac)) {
    $notify_error = 'Invalid MAC: `' . $mac . '\'.';
    return;
  }

  if (isset($filename) && !valid_filename($filename)) {
    $notify_error = 'Invalid filename: `' . $filename . '\'.';
    return;
  }

  if (isset($next_server) && !valid_next_server($next_server)) {
    $notify_error = 'Invalid next server: `' . $next_server . '\'.';
    return;
  }

  $add_data = Array();

  $add_data['hostname']     = $hostname;
  $add_data['ip']           = $target;
  $add_data['mac']          = $mac;

  if (!is_null($filename))
    $add_data['filename']   = $filename;

  if (!is_null($next_server))
    $add_data['nextServer'] = gethostbyname($next_server);

  try {
    $record_manager->addReq($add_data);
  } catch (Exception $e) {
    $notify_error = (string)$e;
  }
}

function handle_update() {
  global $notify_error;
  global $record_manager;

  assert(isset($_POST['ip']));
  assert(isset($_POST['del_ip']));
  assert(isset($_POST['mac']));
  assert(isset($_POST['hostname']));

  $hostname     = $_POST['hostname'];
  $target       = $_POST['ip'];
  $del_target   = $_POST['del_ip'];
  $mac          = $_POST['mac'];

  $general_records = $record_manager->getReq();

  $all_records = $general_records['reservations'] + $general_records['leases'];

  foreach ($all_records as $record) {
    if ($mac      == $record['mac'] ||
        $target   == $record['ip']  ||
        $hostname == $record['hostname']) {
      if ($mac      == $record['mac'] &&
          $target   == $record['ip']  &&
          $hostname == $record['hostname']) {
        break;
      } else if ($del_target != $record['ip']) {
        $notify_error = "IP, MAC, or hostname already in use by another host.";
        return;
      }
    }
  }

  handle_delete();

  if (is_null($notify_error))
    handle_add();
}

function do_post() {
  global $notify_error;

  switch($_POST['form']) {
  case "delete":
    handle_delete();
    break;
  case "update":
    handle_update();
    break;
  case "add":
    handle_add();
    break;
  default:
    $nofity_error = 'Unknown POST form: `' . $_POST['form'] . '\'.';
  }
}

// vim: set ts=2 sw=2 et syn=php:
