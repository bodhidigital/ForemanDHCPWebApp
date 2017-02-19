<?php // index.php

include 'index.inc';

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
<?php       format_edit_modal($record_type, $i, $record); ?>
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
<?php echo file_get_contents('js/validate.js'); ?>
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
