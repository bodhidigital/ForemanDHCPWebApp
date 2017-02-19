<?php // lib/post.inc

function handle_delete() {
  global $notify_error;
  global $record_manager;

  assert(isset($_POST['ip']));

  $target = $_POST['ip'];

  try {
    $record_manager->deleteReq($target);
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

  $add_data = Array();

  $add_data['hostname']     = $hostname;
  $add_data['ip']           = $target;
  $add_data['mac']          = $mac;

  if (!is_null($filename))
    $add_data['filename']   = $filename;

  if (!is_null($next_server))
    $add_data['nextServer'] = $next_server;

  try {
    $record_manager->addReq($add_data);
  } catch (Exception $e) {
    $notify_error = (string)$e;
  }
}

function handle_update() {
  global $notify_error;

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
    $nofity_error = 'Unknown POST form: ' . $_POST['form'];
  }
}

// vim: set ts=2 sw=2 et syn=php:
