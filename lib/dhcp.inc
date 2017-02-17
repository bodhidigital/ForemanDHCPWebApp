<?php // lib/dhcp.inc

require_once 'curl_errno.inc';

abstract class aRecord {
  private $detail;

  public function __construct($detail, $validate) {
    assert(is_array($detail));
    assert(is_array($validate));

    foreach ($validate as $k => $s)
      assert(is_string($k));
      assert(is_string($s));

      if (!isset($detail[$k]))
        throw new Exception(
          "Invalid record recieved from DHCP Foreman Proxy: missing $s.");

    $this->detail = $detail;
  }

  public function getKeys() {
    return array_keys($this->detail);
  }

  public function get($key) {
    return $this->detail[$key];
  }
}

class ReserveRecord extends aRecord {
  public function __construct($detail) {
    parent::__construct(
      $detail,
      [ 'ip'  => 'IP address',
        'mac' => 'MAC address', ]
    );
  }
}

class LeaseRecord extends aRecord {
  public function __construct($detail) {
    parent::__construct(
      $detail,
      [ 'ip'     => 'IP address',
        'mac'    => 'MAC address',
        'starts' => 'Start time',
        'ends'   => 'End time', ]
    );
  }
}

class RecordCollector {
  private $base_url;

  private $reserve_records;
  private $lease_records;

  public function __construct($server, $port, $subnet) {
    $protocol = 'http';  // TODO: support HTTPS

    assert(is_string($protocol));
    assert(is_string($server));
    assert(is_int($port));
    assert(is_string($subnet));

    $this->base_url = "$protocol://$server:$port/dhcp/$subnet";
  }

  public function get_reserve_records() {
    return $this->reserve_records;
  }

  public function get_lease_records() {
    return $this->lease_records;
  }

  public function fetch() {
    $general_data = $this->fetch_general();

    $this->reserve_records = Array();
    $this->lease_records   = Array();

    foreach ($general_data['reservations'] as $reserve_detail)
      $this->reserve_records[] = new ReserveRecord($reserve_detail);

    foreach ($general_data['leases'] as $lease_detail)
      $this->lease_records[]   = new LeaseRecord($lease_detail);
  }

  private function fetch_general() {
    global $curl_error_codes;

    $ch = curl_init();

    curl_setopt($ch, CURLOPT_URL, $this->base_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    $result = curl_exec($ch);

    $ch_errno = curl_errno($ch);

    if ($ch_errno) {
      curl_close ($ch);

      throw new Exception(
        'Failed to retrive general records from server: ' .
        $curl_error_codes[$ch_errno] .
        curl_error($ch));
    }

    curl_close ($ch);

    try {
      $general_data = json_decode($result, true);
    } catch (Exception $e) {
      throw new Exception(
        'Server responded with invalid JSON: ' .
        (string)$e);
    }

    return $general_data;
  }

  private function fetch_detail($ip) {
    global $curl_error_codes;

    $ch = curl_init();

    curl_setopt($ch, CURLOPT_URL, $this->base_url . '/' . $ip);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    $result = curl_exec($ch);

    $ch_errno = curl_errno($ch);

    if ($ch_errno) {
      curl_close($ch);

      throw new Exception(
        'Failed to retrive specific record from server, ' .
        'perhaps the records were just modified: ' .
        curl_error($ch));
    }

    curl_close($ch);

    try {
      $detail_data = json_decode($result, true);
    } catch (Exception $e) {
      throw new Exception(
        'Server responded with invalid JSON: ' .
        (string)$e);
    }

    return $detail_data;
  }
}

// vim: set ts=2 sw=2 et syn=php:
