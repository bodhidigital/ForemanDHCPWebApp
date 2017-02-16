<?php // index.php

if (is_file('config.php'))
  include_once 'config.php';

require_once 'default-config.php';

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
      html, body {
        height: 100%;
      }

      .management-form table {
        background-color: #DDD;
        border: 1px solid grey;
        width: 100%;
      }
      .management-form tbody {
        height: 100%;
        overflow-y: scroll;
        display: block;
      }
      .management-form thead {
        display: block;
      }
      .management-form thead tr {
        display: table;
        width: 100%;
      }
      .management-form thead tr th {
        display: table-cell;
        text-align: center;
        vertical-align: middle;
      }

      .mode-sel-bar ul {
        padding: 0;
      }
      .mode-sel-bar li {
        width: 100%;
        list-style-type: none;
      }
      .mode-sel-bar button {
        width: 100%;
        padding: 8px;
        border: 1px solid grey;
        border-top: none;
        font-size: 20px;
        font-weight: bold;
      }
      .mode-sel-bar .first button {
        border-top: 1px solid grey;
      }
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
        <div class="col-xs-12 col-md-8 management-form">
          <form id="main-form">
            <div id="reserve-table">
              <h3 class="text-center">Reserved IPs<span class="hidden-xs"> (Static Leases)</span></h3>
              <table>
                <thead>
                  <tr>
                    <th><label><input id="select-all-reserve" type="checkbox"><span class="hidden-xs"> Select all</span></label></th>
                    <th>Hostname</th>
                    <th>IP<span class="hidden-xs"> Address</span></th>
                    <th>MAC<span class="hidden-xs"> Address</span></th>
                    <th class="hidden-xs">Additional Information</th>
                  </tr>
                </thead>
                <tbody>
                </tbody>
              </table>
            </div>
            <div hidden id="lease-table">
              <h3 class="text-center">Dynamic Leases</h3>
              <table>
                <thead>
                  <tr>
                    <th><label><input id="select-all-lease" type="checkbox"><span class="hidden-xs"> Select all</span></label></th>
                    <th>Hostname</th>
                    <th>IP<span class="hidden-xs"> Address</span></th>
                    <th>MAC<span class="hidden-xs"> Address</span></th>
                    <th>Starts</th>
                    <th>Ends</th>
                    <th class="hidden-xs">Additional Information</th>
                  </tr>
                </thead>
                <tbody>
                </tbody>
              </table>
            </div>
          </form>
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
      var modeSelButtons = jQuery('.mode-sel-bar button');

      modeSelButtons.click(function() {
        var showTable = jQuery('#main-form > ' + jQuery(this).data('toggle'));
        var allTables = jQuery('#main-form > *').not(showTable);

        allTables.fadeOut(function() {
          showTable.fadeIn();
        });
      });
    </script>
  </body>
</html>
<?php // vim: set ts=2 sw=2 et syn=php: ?>
