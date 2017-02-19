<?php // lib/format.inc

const RESERVE = 'reserve';
const LEASE   = 'lease';

function get_info_modal_id($type, $number) {
  return $type . '-' . $number . '-info';
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

  echo '<td class="first">' .
         '<span class="glyphicon glyphicon-remove-sign"></span>' .
         '<span class="glyphicon glyphicon-edit"></span>' .
       '</td>';

  if (RESERVE == $type)
    echo '<td class="hostname">' . $record->get('hostname') . '</td>';

  echo '<td class="ip">' . $record->get('ip') . '</td>' .
       '<td ' . $xs_hidden_attr . ' class="mac">' . $record->get('mac') . '</td>';

  if (LEASE == $type) {
    $short_stime = date('H:i:s n/j', strtotime($record->get('starts')));
    $short_etime = date('H:i:s n/j', strtotime($record->get('ends')));

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

// vim: set ts=2 sw=2 et syn=php:
