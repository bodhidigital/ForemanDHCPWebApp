<?php // index.php

if (is_file('config.php'))
  include_once 'config.php';

require_once 'default-config.php';

require_once 'lib/dhcp.inc';

const RESERVE = 'reserve';
const LEASE   = 'lease';

$notify_error = NULL;

$record_manager = new RecordManager(
  $config['dhcp_server'],
  $config['dhcp_server_port'],
  $config['dhcp_subnet']
);

function handle_delete() {
  assert(isset($_POST['ip']));

  $target = $_POST['ip'];

  try {
    $record_manager->deleteReq($target);
  } catch (Exception $e) {
    $notify_error = (string)$e;
  }
}

function handle_add() {
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
  handle_delete();

  if (is_null($notify_error))
    handle_add();
}

function get_info_modal_id($type, $number) {
  return $type . '-' . $number . '-info';
}

function format_header($type) {
  assert(RESERVE == $type || LEASE == $type);

  global $config;

  if ($config['datatables_enable'])
    $xs_hidden_attr = '';
  else
    $xs_hidden_attr = 'class="hidden-xs"';

  echo '<th class="first"></th>';

  if (RESERVE == $type)
    echo '<th>Hostname</th>';

  echo '<th>IP<span class="hidden-xs"> Address</span></th>' .
       '<th ' . $xs_hidden_attr . '>MAC Address</th>';

  if (LEASE == $type)
    echo '<th class="hidden-xs">Starts</th>' .
         '<th>Ends</th>';

  echo '<th class="last"></th>';
}

function format_row($type, $number, $record) {
  assert(RESERVE == $type || LEASE == $type);
  assert(is_int($number));
  assert(is_a($record, 'aRecord'));

  global $config;

  if ($config['datatables_enable'])
    $xs_hidden_attr = '';
  else
    $xs_hidden_attr = 'class="hidden-xs"';

  assert(RESERVE == $type || LEASE == $type);
  assert(is_int($number));
  assert(is_a($record, 'aRecord'));

  $info_modal_id = get_info_modal_id($type, $number);

  echo '<td class="first">' .
         '<span class="glyphicon glyphicon-remove-sign"></span>' .
         '<span class="glyphicon glyphicon-edit"></span>' .
       '</td>';

  if (RESERVE == $type)
    echo '<td><span class="hostname">' . $record->get('hostname') . '</span></td>';

  echo '<td><span class="ip">' . $record->get('ip') . ' </span></td>' .
       '<td ' . $xs_hidden_attr . '><span class="mac">' . $record->get('mac') . '</span></td>';

  if (LEASE == $type) {
    $short_stime = date('H:i:s n/j', strtotime($record->get('starts')));
    $short_etime = date('H:i:s n/j', strtotime($record->get('ends')));

    echo '<td ' . $xs_hidden_attr . '><span class="time">' . $short_stime . '</span></td>' .
         '<td><span class="time">' . $short_etime . '</span></td>';
  }

  echo '<td class="last">' .
         '<a data-toggle="modal" data-target="#' . $info_modal_id . '">' .
           '<span class="glyphicon glyphicon-info-sign"></span>' .
         '</a>'.
       '</td>';
}

function format_info_modal($type, $number, $record) {
  assert(RESERVE == $type || LEASE == $type);
  assert(is_int($number));
  assert(is_a($record, 'aRecord'));

  $info_modal_id = get_info_modal_id($type, $number);

  echo '<div class="modal fade" id="' . $info_modal_id . '" tabindex="-1" role="dialog">' .
         '<div class="modal-dialog modal-content modal-lg">' .
           '<div class="modal-header">' .
             '<button type="button" class="close" data-dismiss="modal">×</button>' .
             '<h3 class="text-center">Additional Info</h3>' .
           '</div>' .
           '<div class="modal-body record-info" role="document">' .
             '<ul>';

  foreach ($record->getKeys() as $k) {
    echo       '<li>' .
                 '<h4>' .
                   htmlspecialchars($k) . ' =&gt; ' . htmlspecialchars($record->get($k)) .
                 '</h4>' .
               '</li>';
  }

  echo       '</ul>' .
           '</div>' .
         '</div>' .
       '</div>';
}

$record_manager->chOpen();

if (is_null($notify_error)) {
  if ($_SERVER['REQUEST_METHOD'] == 'POST') {
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

?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <title>DHCP Leases</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link
      rel="stylesheet"
      type="text/css"
      href="<?php echo $config['bootstrap_base']; ?>/css/bootstrap.min.css">
<?php if ($config['datatables_enable']): ?>
    <link
      rel="stylesheet"
      type="text/css"
      href="<?php echo $config['datatables_base']; ?>/css/dataTables.bootstrap4.min.css">
    <link
      rel="stylesheet"
      type="text/css"
      href="<?php echo $config['datatables_responsive_base']; ?>/css/responsive.dataTables.min.css">
<?php endif; ?>
    <style>
<?php echo file_get_contents('css/style.css'); ?>
    </style>
  </head>
  <body>
    <div class="container">
      <div class="row">
        <div class="col-xs-12">
          <h1 class="text-center">DHCP Management for Foreman Proxy</h1>
          <hr>
        </div>
<?php if (!isset($notify_error)): ?>
<?php   $first_title= true; ?>
        <div class="col-xs-12 records records-titles">
<?php   foreach ([RESERVE => "Reserved IPs", LEASE => "Dynamic Leases"] as $record_type => $title): ?>
          <div <?php if ($first_title) $first_title = false; else echo 'hidden'; ?>
               class="record <?php echo $record_type; ?>-record">
            <h3 class="text-center"><?php echo $title; ?></h3>
          </div>
<?php   endforeach; ?>
        </div>
        <div class="col-xs-12" id="records-nav">
          <ul class="nav nav-tabs">
            <li class="active first"><a class="show-records" href="javascript:void(0)" data-target="reserve">Reserved</a></li>
            <li><a class="show-records" href="javascript:void(0)" data-target="lease">Leased</a></li>
            <li class="pull-right last"><a href="javascript:void(0)">Add New</a></li>
          </ul>
        </div>
<?php endif; ?>
<?php if (isset($notify_error)): ?>
        <div class="col-xs-12">
          <h2>Error</h2>
          <pre style="border-color:rgb(192,32,32)"><?php echo htmlspecialchars($notify_error); ?></pre>
        </div>
<?php endif; ?>
<?php if (!isset($notify_error)): ?>
        <div class="col-xs-12 records records-tables">
<?php   $first_table = true; ?>
<?php   foreach ([RESERVE => $reserve_records, LEASE => $lease_records] as $record_type => $records): ?>
          <div <?php if ($first_table) $first_table = false; else echo 'hidden'; ?>
               class="record <?php echo $record_type; ?>-record">
            <table class="table table-striped table-bordered" cellspacing="0" width="100%">
              <thead>
                <tr>
<?php format_header($record_type); ?>
                </tr>
              </thead>
              <tbody>
<?php     for ($i = 0; count($records) > $i; ++$i): ?>
<?php       $record = $records[$i]; ?>
<?php       $first_record = 0 == $i; ?>
<?php       $last_record  = count($records) - 1 == $i; ?>
<?php       $row_classes  = $first_record                 ? 'first' : ''; ?>
<?php       $row_classes .= $first_record && $last_record ? ' '     : ''; ?>
<?php       $row_classes .= $last_record                  ? 'last'  : ''; ?>
                <tr <?php if (!empty($row_classes)) echo "class=\"$row_classes\""; ?>>
<?php       format_row($record_type, $i, $record); ?>
                </tr>
<?php     endfor; ?>
              </tbody>
            </table>
          </div>
<?php   endforeach; ?>
        </div>
<?php endif; ?>
      </div>
    </div>
<?php if (!isset($notify_error)): ?>
<?php   foreach ([
              RESERVE => $reserve_records,
              LEASE => $lease_records
            ] as $record_type => $records): ?>
<?php     for ($i = 0; count($records) > $i; ++$i): ?>
<?php       $record = $records[$i]; ?>
<?php       format_info_modal($record_type, $i, $record); ?>
<?php     endfor; ?>
<?php   endforeach; ?>
<?php endif; ?>
    <script src="<?php echo $config['jquery_src']; ?>"
<?php if (isset($config['jquery_integ'])): ?>
            integrity="<?php echo $config['jquery_integ']; ?>"
<?php endif; ?>
<?php if (isset($config['jquery_crossorigin'])): ?>
            crossorigin="<?php echo $config['jquery_crossorigin']; ?>"
<?php endif; ?>
    ><?php // > is not a typo ?>
    </script>
    <script src="<?php echo $config['bootstrap_base']; ?>/js/bootstrap.min.js">
    </script>
    <script>
<?php echo file_get_contents('js/dhcp.js'); ?>
    </script>
<?php if ($config['datatables_enable']): ?>
    <script src="<?php echo $config['datatables_base']; ?>/js/jquery.dataTables.min.js">
    </script>
    <script src="<?php echo $config['datatables_base']; ?>/js/dataTables.bootstrap4.min.js">
    </script>
    <script src="<?php echo $config['datatables_responsive_base']; ?>/js/dataTables.responsive.min.js">
    </script>
    <script>
<?php echo file_get_contents('js/pick_datatables.js'); ?>
    </script>
<?php endif; ?>
  </body>
</html>
<?php // vim: set ts=2 sw=2 et syn=php: ?>
