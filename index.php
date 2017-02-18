<?php // index.php

if (is_file('config.php'))
  include_once 'config.php';

require_once 'default-config.php';

require_once 'lib/dhcp.inc';

const RESERVE = 'reserve';
const LEASE   = 'lease';

function handle_delete() {
  assert(isset($_POST['ip']));

  $target = $_POST['ip'];
}

function handle_update() {
  assert(isset($_POST['ip']));
  assert(isset($_POST['hostname']));
  assert(isset($_POST['name']));
  assert(isset($_POST['filename']));
  assert(isset($_POST['nextServer']));
  assert(isset($_POST['mac']));

  $target       = $_POST['ip'];
  $name         = $_POST['name'];
  $hostname     = $_POST['hostname'];
  $mac          = $_POST['mac'];
  $filename     = $_POST['filename'];
  $next_server  = $_POST['nextServer'];
}

function handle_add() {
  assert(isset($_POST['ip']));
  assert(isset($_POST['hostname']));
  assert(isset($_POST['name']));
  assert(isset($_POST['filename']));
  assert(isset($_POST['nextServer']));
  assert(isset($_POST['mac']));

  $target       = $_POST['ip'];
  $name         = $_POST['name'];
  $hostname     = $_POST['hostname'];
  $mac          = $_POST['mac'];
  $filename     = $_POST['filename'];
  $next_server  = $_POST['nextServer'];
}

function get_info_modal_id($type, $number) {
  return $type . '-' . $number . '-info';
}

function format_header($type) {
  assert(RESERVE == $type || LEASE == $type);

  echo '<th class="first"></th>';

  if (RESERVE == $type)
    echo '<th>Hostname</th>';

  echo '<th>IP<span class="hidden-xs"> Address</span></th>' .
       '<th class="hidden-xs">MAC Address</th>';

  if (LEASE == $type)
    echo '<th class="hidden-xs">Starts</th>' .
         '<th>Ends</th>';

  echo '<th class="last"></th>';
}

function format_row($type, $number, $record) {
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
       '<td class="hidden-xs"><span class="mac">' . $record->get('mac') . '</span></td>';

  if (LEASE == $type) {
    $short_stime = date('H:i:s n/j', strtotime($record->get('starts')));
    $short_etime = date('H:i:s n/j', strtotime($record->get('ends')));

    echo '<td class="hidden-xs"><span class="time">' . $short_stime . '</span></td>' .
         '<td class="hidden-xs"><span class="time">' . $short_etime . '</span></td>';
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

$notify_error = NULL;

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
  $rc = new RecordCollector(
    $config['dhcp_server'],
    $config['dhcp_server_port'],
    $config['dhcp_subnet']
  );

  try {
    $rc->fetch();
  } catch (Exception $e) {
    $notify_error = (string)$e;
  }

  $reserve_records = $rc->get_reserve_records();
  $lease_records   = $rc->get_lease_records();
}

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
        <div class="col-xs-12 col-md-2 mode-sel-bar">
          <ul>
            <li class="first"><button data-target="#reserve-table">Reserved</button></li>
            <li class="last"><button data-target="#lease-table">Leased</button></li>
          </ul>
        </div>
<?php if (isset($notify_error)): ?>
        <div class="col-xs-12">
          <h2>Error</h2>
          <pre style="border-color:rgb(192,32,32)"><?php echo htmlspecialchars($notify_error); ?></pre>
        </div>
<?php endif; ?>
<?php if (!isset($notify_error)): ?>
        <div class="col-xs-12 col-md-10 management-tables">
<?php   $first_table = true; ?>
<?php   foreach ([
            RESERVE => [
              'name'    => "Reserved IPs",
              'records' => $reserve_records,
            ],
            LEASE => [
              'name'    => "Dynamic Leases",
              'records' => $lease_records,
          ],] as $record_type => $record_data): ?>
          <div <?php if ($first_table) $first_table = false; else echo 'hidden'; ?>
               id="<?php echo $record_type; ?>-table">
            <h3 class="text-center"><?php echo $record_data['name']; ?></h3>
            <table>
              <thead>
                <tr>
<?php format_header($record_type); ?>
                </tr>
              </thead>
              <tbody>
<?php     for ($i = 0; count($record_data['records']) > $i; ++$i): ?>
<?php       $records = $record_data['records'][$i]; ?>
<?php       $first_record = 0 == $i; ?>
<?php       $last_record  = count($record_data['records']) - 1 == $i; ?>
<?php       $row_classes  = $first_record                 ? 'first' : ''; ?>
<?php       $row_classes .= $first_record && $last_record ? ' '     : ''; ?>
<?php       $row_classes .= $last_record                  ? 'last'  : ''; ?>
                <tr <?php if (!empty($row_classes)) echo "class=\"$row_classes\""; ?>>
<?php       format_row($record_type, $i, $records); ?>
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
    <script
      src="<?php echo $config['jquery_src']; ?>"
<?php if (isset($config['jquery_integ'])): ?>
      integrity="<?php echo $config['jquery_integ']; ?>"
<?php endif; ?>
<?php if (isset($config['jquery_crossorigin'])): ?>
      crossorigin="<?php echo $config['jquery_crossorigin']; ?>"
<?php endif; ?>
    ></script>
    <script src="<?php echo $config['bootstrap_base']; ?>/js/bootstrap.min.js">
    </script>
    <script>
<?php echo file_get_contents('js/dhcp.js'); ?>
    </script>
  </body>
</html>
<?php // vim: set ts=2 sw=2 et syn=php: ?>
