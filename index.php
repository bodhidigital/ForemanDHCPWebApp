<?php // index.php

if (is_file('config.php'))
  include_once 'config.php';

require_once 'default-config.php';

require_once 'lib/dhcp.inc';

$rc = new RecordCollector(
  $config['dhcp_server'],
  $config['dhcp_server_port'],
  $config['dhcp_subnet']
);

$notify_error = NULL;

try {
  $rc->fetch();
} catch (Exception $e) {
  $notify_error = (string)$e;
}

$reserve_records = $rc->get_reserve_records();
$lease_records   = $rc->get_lease_records();

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
        <div class="col-xs-12 col-md-4 mode-sel-bar">
          <ul>
            <li class="first"><button data-toggle="#reserve-table">Reserved</button></li>
            <li class="last"><button data-toggle="#lease-table">Leased</button></li>
          </ul>
        </div>
        <div class="col-xs-12 col-md-8 management-tables">
<?php if (isset($notify_error)): ?>
          <pre style="border-color:rgb(192,32,32)"><?php echo htmlspecialchars($notify_error); ?></pre>
<?php else: ?>
          <div id="reserve-table">
            <h3 class="text-center">Reserved IPs<span class="hidden-xs"> (Static Leases)</span></h3>
            <table>
              <thead>
                <tr>
                  <th></th>
                  <th>Hostname</th>
                  <th>IP<span class="hidden-xs"> Address</span></th>
                  <th>MAC<span class="hidden-xs"> Address</span></th>
                  <th></th>
                </tr>
              </thead>
              <tbody>
<?php for ($i = 0; count($reserve_records) > $i; ++$i): ?>
<?php   $reserve = $reserve_records[$i]; ?>
<?php   $first = 0 == $i; ?>
<?php   $last  = count($reserve_records) - 1 == $i; ?>
<?php   $row_classes = ($first ? 'first' : '') . ($last ? ' last' : ''); ?>
                <tr <?php if ($row_classes) echo "class=\"$row_classes\""; ?>>
                  <td class="first"><span class="glyphicon glyphicon-remove-sign"></span><span class="glyphicon glyphicon-edit"></span></td>
                  <td><span class="hostname"><?php echo $reserve->get('hostname'); ?></span></td>
                  <td><span class="ip"><?php echo $reserve->get('ip'); ?></span></td>
                  <td><span class="mac"><?php echo $reserve->get('mac'); ?></span></td>
                  <td class="last"><span class="glyphicon glyphicon-info-sign"></span></td>
                </tr>
<?php endfor; ?>
              </tbody>
            </table>
          </div>
          <div hidden id="lease-table">
            <h3 class="text-center">Dynamic Leases</h3>
            <table>
              <thead>
                <tr>
                  <th></th>
                  <th>IP<span class="hidden-xs"> Address</span></th>
                  <th>MAC<span class="hidden-xs"> Address</span></th>
                  <th>Starts</th>
                  <th>Ends</th>
                  <th></th>
                </tr>
              </thead>
              <tbody>
<?php for ($i = 0; count($lease_records) > $i; ++$i): ?>
<?php   $lease = $lease_records[$i]; ?>
<?php   $first = 0 == $i; ?>
<?php   $last  = count($lease_records) - 1 == $i; ?>
<?php   $row_classes = ($first ? 'first' : '') . ($last ? ' last' : ''); ?>
                <tr <?php if ($row_classes) echo "class=\"$row_classes\""; ?>>
                  <td class="first"><span class="glyphicon glyphicon-remove-sign"></span><span class="glyphicon glyphicon-edit"></span></td>
                  <td><span class="ip"><?php echo $lease->get('ip'); ?></span></td>
                  <td><span class="mac"><?php echo $lease->get('mac'); ?></span></td>
                  <td><span class="time"><?php echo $lease->get('starts'); ?></span></td>
                  <td><span class="time"><?php echo $lease->get('ends'); ?></span></td>
                  <td class="last"><span class="glyphicon glyphicon-info-sign"></span></td>
                </tr>
<?php endfor; ?>
              </tbody>
            </table>
          </div>
<?php endif; ?>
        </div>
      </div>
    </div>
    <script
      src="<?php echo $config['jquery_src']; ?>"
<?php if (isset($config['jquery_integ'])): ?>
      integrity="<?php echo $config['jquery_integ']; ?>"
<?php endif; ?>
<?php if (isset($config['jquery_crossorigin'])): ?>
      crossorigin="<?php echo $config['jquery_crossorigin']; ?>">
<?php endif; ?>
    </script>
    <script src="<?php echo $config['bootstrap_base']; ?>/js/bootstrap.min.js">
    </script>
    <script>
<?php echo file_get_contents('js/dhcp.js'); ?>
    </script>
  </body>
</html>
<?php // vim: set ts=2 sw=2 et syn=php: ?>
