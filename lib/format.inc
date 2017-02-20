<?php // lib/format.inc

const RESERVE = 'reserve';
const LEASE   = 'lease';

function get_info_modal_id($type, $number) {
  return $type . '-' . $number . '-info';
}

function get_edit_modal_id($type, $number) {
  return $type . '-' . $number . '-edit';
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

  $info_modal_id = get_info_modal_id($type, $number);
  $edit_modal_id = get_edit_modal_id($type, $number);

  echo '<td class="first">' .
         '<span class="glyphicon glyphicon-remove-sign"></span>' .
         '<a data-toggle="modal" data-target="#' . $edit_modal_id . '">' .
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

  echo '<div class="modal fade" id="' . $edit_modal_id . '" tabindex="-1" role="dialog">' .
         '<div class="modal-dialog modal-content modal-lg">' .
           '<div class="modal-header">' .
             '<button type="button" class="close" data-dismiss="modal">×</button>' .
             '<h3 class="text-center">Edit Host</h3>' .
           '</div>' .
           '<div class="modal-body record-add" role="document">';

  echo       '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">' .
               '<input type="hidden" name="form" value="update">' .
               '<input type="hidden" name="del_ip" value="' . $ip . '">' .
               '<input type="hidden" name="del_type" value="' . $type . '">' .
               '<div class="form-group">' .
                 '<label for="' . $edit_modal_id . '-hostname">' .
                   'Hostname' .
                 '</label>' .
                 '<input type="text" ' .
                        'class="form-control" ' .
                        'id="' . $edit_modal_id . '-hostname" ' .
                        'name="hostname" ' .
                        'value="' . $hostname . '">' .
                 '<small class="form-text text-muted">' .
                   'Must be valid as per RFC 1123.  Omit trailing dot.' .
                 '</small>' .
               '</div>' .
               '<div class="form-group">' .
                 '<label for="' . $edit_modal_id . '-ip">' .
                   'IP Address' .
                 '</label>' .
                 '<input type="text" ' .
                        'class="form-control" ' .
                        'id="' . $edit_modal_id . '-ip" ' .
                        'name="ip" ' .
                        'value="' . $ip . '">' .
                 '<small class="form-text text-muted">' .
                   'In decimal-dot notation.' .
                 '</small>' .
               '</div>' .
               '<div class="form-group">' .
                 '<label for="' . $edit_modal_id . '-mac">' .
                   'MAC address' .
                 '</label>' .
                 '<input type="text" ' .
                        'class="form-control" ' .
                        'id="' . $edit_modal_id . '-mac" ' .
                        'name="mac" ' .
                        'value="' . $mac . '">' .
                 '<small class="form-text text-muted">' .
                   'Six groups of colon seperated octets in hexadecimal format.' .
                 '</small>' .
               '</div>' .
               '<div class="form-group">' .
                 '<label for="' . $edit_modal_id . '-filename">' .
                   'PXE Filename' .
                 '</label>' .
                 '<input type="text" ' .
                        'class="form-control" ' .
                        'id="' . $edit_modal_id . '-filename" ' .
                        'name="filename" ' .
                        'value="' . $filename . '">' .
                 '<small class="form-text text-muted">' .
                   'Must be a relavive path.' .
                 '</small>' .
               '</div>' .
               '<div class="form-group">' .
                 '<label for="' . $edit_modal_id . '-nextServer">' .
                   'PXE TFTP Server' .
                 '</label>' .
                 '<input type="text" ' .
                        'class="form-control" ' .
                        'id="' . $edit_modal_id . '-nextServer" ' .
                        'name="nextServer" ' .
                        'value="' . $next_server . '">' .
                 '<small class="form-text text-muted">' .
                   'IP address or hostname.' .
                 '</small>' .
               '</div>' .
               '<button type="submit" class="btn btn-primary">Submit</button><br>' .
             '</form>';

  echo     '</div>' .
         '</div>' .
       '</div>';
}

// vim: set ts=2 sw=2 et syn=php:
