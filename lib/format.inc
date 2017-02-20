<?php // lib/format.inc

const RESERVE     = 'reserve';
const LEASE       = 'lease';

const FORM_UPDATE = 'update';
const FORM_DELETE = 'delete';
const FORM_ADD    = 'add';

function get_info_modal_id($type, $number) {
  return $type . '-' . $number . '-info';
}

function get_edit_modal_id($type, $number) {
  return $type . '-' . $number . '-edit';
}

function get_delete_modal_id($type, $number) {
  return $type . '-' . $number . '-delete';
}

function get_add_modal_id() {
  return 'reserve-add';
}

function format_header($type) {
  assert(RESERVE == $type || LEASE == $type);

  global $config;

  if ($config['datatables_enable']) {
    $xs_hidden_attr   = '';
    $xs_visible_attr  = 'class="visible-xs"';
  } else {
    $xs_hidden_attr   = 'class="hidden-xs"';
    $xs_visible_attr  = 'hidden';
  }

  echo '<th class="first"></th>';

  if (RESERVE == $type)
    echo '<th>Hostname</th>';

  echo '<th>IP<span class="hidden-xs"> Address</span></th>' .
       '<th ' . $xs_hidden_attr . '>MAC Address</th>';

  if (LEASE == $type)
    echo '<th ' . $xs_hidden_attr . '>Starts</th>' .
         '<th>Ends</th>';

  echo '<th class="last"><span ' . $xs_visible_attr . '>Info</span></th>';
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

  $info_modal_id    = get_info_modal_id(  $type, $number);
  $edit_modal_id    = get_edit_modal_id(  $type, $number);
  $delete_modal_id  = get_delete_modal_id($type, $number);

  echo '<td class="first">' .
         '<a class="gly-remove" ' .
            'href="javascript:void(0)" ' .
            'data-toggle="modal" ' .
            'data-target="#' . $delete_modal_id . '">' .
           '<span class="glyphicon glyphicon-remove-sign"></span>' .
         '</a>'.
         '<a class="gly-edit" ' .
            'href="javascript:void(0)" ' .
            'data-toggle="modal" ' .
            'data-target="#' . $edit_modal_id . '">' .
           '<span class="glyphicon glyphicon-edit"></span>' .
         '</a>'.
       '</td>';

  if (RESERVE == $type)
    echo '<td class="hostname">' . $record->get('hostname') . '</td>';

  echo '<td class="ip">' . $record->get('ip') . '</td>' .
       '<td ' . $xs_hidden_attr . ' class="mac">' . $record->get('mac') . '</td>';

  if (LEASE == $type) {
    $short_stime = date('m/j H:i:s', strtotime($record->get('starts')));
    if (in_array('ends', $record->getKeys()))
      $short_etime = date('m/j H:i:s', strtotime($record->get('ends')));
    else
      $short_etime = "NULL";

    echo '<td ' . $xs_hidden_attr . ' class="time">' . $short_stime . '</td>' .
         '<td class="time">' . $short_etime . '</td>';
  }

  echo '<td class="last">' .
         '<a class="gly-info" ' .
            'href="javascript:void(0)" ' .
            'data-toggle="modal" ' .
            'data-target="#' . $info_modal_id . '">' .
           '<span class="glyphicon glyphicon-info-sign"></span>' .
         '</a>'.
       '</td>';
}

function show_info($record) {
  assert(is_a($record, 'aRecord'));

  echo <<<HTML
<div class="mono">
  <ul>
HTML;

  foreach ($record->getKeys() as $k) {
    $esc_k = htmlspecialchars($k);
    $esc_v = htmlspecialchars($record->get($k));

    echo <<<HTML
    <li>
      <h4>$esc_k =&gt; $esc_v</h4>
    </li>
HTML;
  }

  echo <<<HTML
  </ul>
</div>
HTML;
}

function format_info_modal($type, $number, $record) {
  assert(RESERVE == $type || LEASE == $type);
  assert(is_int($number));
  assert(is_a($record, 'aRecord'));

  $info_modal_id = get_info_modal_id($type, $number);

  echo <<<HTML
<div class="modal fade" id="$info_modal_id" tabindex="-1" role="dialog">
  <div class="modal-dialog modal-content modal-lg">
    <div class="modal-header">
      <button type="button" class="close" data-dismiss="modal">×</button>
      <h3 class="text-center">Additional Info</h3>
    </div>
    <div class="modal-body record-info" role="document">
HTML;

  show_info($record);

  echo <<<HTML
    </div>
  </div>
</div>
HTML;
}

function format_host_form_modal($modal_id, $form, $type, $ip = '', $mac = '',
                                $hostname = '', $filename = '', $next_server = '') {
  assert(FORM_UPDATE == $form || FORM_ADD == $form);
  assert(RESERVE     == $type || LEASE    == $type);

  $title = (FORM_UPDATE == $form) ? 'Edit Host' : 'New Host';

  $hidden_inputs = 
    "<input type=\"hidden\" name=\"form\" value=\"$form\">";
  
  if ("update" == $form) {
    $hidden_inputs .=
      "<input type=\"hidden\" name=\"del_ip\" value=\"$ip\">" .
      "<input type=\"hidden\" name=\"del_type\" value=\"$type\">";
  }

  echo <<<HTML
<div class="modal fade" id="$modal_id" tabindex="-1" role="dialog">
  <div class="modal-dialog modal-content modal-lg">
    <div class="modal-header">
      <button type="button" class="close" data-dismiss="modal">×</button>
      <h3 class="text-center">$title</h3>
    </div>
    <div class="modal-body record-$form" role="document">
      <form method="post" action="{$_SERVER['PHP_SELF']}">
        $hidden_inputs
        <div class="form-group">
          <label for="{$modal_id}-hostname">Hostname</label>
          <input type="text"
                 class="form-control"
                 id="{$modal_id}-hostname"
                 name="hostname"
                 value="$hostname">
          <small class="form-text text-muted">
            Must be valid as per RFC 1123.  Omit trailing dot.
          </small>
        </div>
        <div class="form-group">
          <label for="{$modal_id}-ip">IP Address</label>
          <input type="text"
                 class="form-control"
                 id="{$modal_id}-ip"
                 name="ip"
                 value="$ip">
          <small class="form-text text-muted">
            In decimal-dot notation.
          </small>
        </div>
        <div class="form-group">
          <label for="{$modal_id}-mac">MAC address</label>
          <input type="text"
                 class="form-control"
                 id="{$modal_id}-mac"
                 name="mac"
                 value="$mac">
          <small class="form-text text-muted">
            Six groups of colon seperated octets in hexadecimal format.
          </small>
        </div>
        <div class="form-group">
          <label for="{$modal_id}-filename">PXE Filename</label>
          <input type="text"
                 class="form-control"
                 id="{$modal_id}-filename"
                 name="filename"
                 value="$filename">
          <small class="form-text text-muted">
            Must be a relavive path.
          </small>
        </div>
        <div class="form-group">
          <label for="{$modal_id}-nextServer">
            PXE TFTP Server
          </label>
          <input type="text"
                 class="form-control"
                 id="{$modal_id}-nextServer"
                 name="nextServer"
                 value="$next_server">
          <small class="form-text text-muted">
            IP address or hostname.
          </small>
        </div>
        <button type="submit" class="btn btn-primary">Submit</button>
      </form>
    </div>
  </div>
</div>
HTML;
}

function format_add_modal() {
  $add_modal_id = get_add_modal_id();

  format_host_form_modal($add_modal_id, FORM_ADD, RESERVE);
}

function format_edit_modal($type, $number, $record) {
  assert(RESERVE == $type || LEASE == $type);
  assert(is_int($number));
  assert(is_a($record, 'aRecord'));

  $edit_modal_id = get_edit_modal_id($type, $number);

  $record_keys = $record->getKeys();

  if (RESERVE == $type)
    $hostname     = $record->get('hostname');
  else
    $hostname     = '';

  $ip             = htmlspecialchars($record->get('ip'));
  $mac            = htmlspecialchars($record->get('mac'));

  if (RESERVE == $type && in_array('filename', $record_keys))
    $filename     = htmlspecialchars($record->get('filename'));
  else
    $filename     = '';

  if (RESERVE == $type && in_array('nextServer', $record_keys))
    $next_server  = htmlspecialchars($record->get('nextServer'));
  else
    $next_server  = '';

  format_host_form_modal($edit_modal_id, FORM_UPDATE, $type, $ip, $mac,
                         $hostname, $filename, $next_server);
}

function format_delete_modal($type, $number, $record) {
  assert(RESERVE == $type || LEASE == $type);
  assert(is_int($number));
  assert(is_a($record, 'aRecord'));

  $modal_id = get_delete_modal_id($type, $number);

  $ip    = htmlspecialchars($record->get('ip'));
  $title = "Are you sure?";
  $form  = FORM_DELETE;

  echo <<<HTML
<div class="modal fade" id="$modal_id" tabindex="-1" role="dialog">
  <div class="modal-dialog modal-content modal-lg">
    <div class="modal-header">
      <button type="button" class="close" data-dismiss="modal">×</button>
      <h3 class="text-center">$title</h3>
    </div>
    <div class="modal-body record-$form" role="document">
      <h4>Are you sure you want to delete this host:</h4>
HTML;

  show_info($record);

  echo <<<HTML
      <br>
      <form method="post" action="{$_SERVER['PHP_SELF']}">
        <input type="hidden" name="form" value="$form">
        <input type="hidden" name="del_ip" value="$ip">
        <input type="hidden" name="del_type" value="$type">
        <button type="submit" class="btn btn-primary">Submit</button>
        <button type="button" class="btn btn-primary btn-cancel" data-dismiss="modal">Cancel</button>
      </form>
    </div>
  </div>
</div>
HTML;
}

// vim: set ts=2 sw=2 et syn=php:
