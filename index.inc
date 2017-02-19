<?php // index.inc

if (is_file('config.php'))
  include_once 'config.php';

require_once 'default-config.php';

require_once 'lib/dhcp.inc';

date_default_timezone_set($config['tz']);

$notify_error = NULL;

$record_manager = new RecordManager(
  $config['dhcp_server'],
  $config['dhcp_server_port'],
  $config['dhcp_subnet']
);

require_once 'lib/format.inc';

$record_manager->chOpen();

if (is_null($notify_error)) {
  if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    require_once 'lib/post.inc';

    do_post();
  }
}

if (is_null($notify_error)) {
  $record_collector = new RecordCollector($record_manager);

  try {
    $record_collector->fetch();
  } catch (Exception $e) {
    $notify_error = (string)$e;
  }

  $reserve_records = $record_collector->get_reserve_records();
  $lease_records   = $record_collector->get_lease_records();
}

$record_manager->chClose();

// vim: set ts=2 sw=2 et syn=php:
