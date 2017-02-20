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
        'starts' => 'Start time' ]
    );
  }
}

class RecordManager {
  const GET_REQ    = 'GET';
  const DELETE_REQ = 'DELETE';
  const POST_REQ   = 'POST';

  private $base_url;

  private $ch;

  public function __construct($server, $port, $subnet) {
    $protocol = 'http';  // TODO: support HTTPS

    assert(is_string($protocol));
    assert(is_string($server));
    assert(is_int($port));
    assert(is_string($subnet));

    $this->base_url = "$protocol://$server:$port/dhcp/$subnet";
  }

  public function chOpen() {
    $this->ch = curl_init();
  }

  public function chClose() {
    curl_close($this->ch);
  }

  public function getReq($ip = NULL) {
    assert(is_null($ip) || is_string($ip));

    $url_ext = !is_null($ip) ? "/$ip" : "";

    $this->startRequest($url_ext);

    curl_setopt($this->ch, CURLOPT_CUSTOMREQUEST, RecordManager::GET_REQ);

    $result_json = $this->endRequest(RecordManager::GET_REQ);

    try {
      $result = json_decode($result_json, true);
    } catch (Exception $e) {
      throw new Exception('Server responded with invalid JSON',
                          $e->getCode(), $e);
    }

    return $result;
  }

  public function deleteReq($ip) {
    assert(is_string($ip) && !empty($ip));

    $url_ext = "/$ip";

    $this->startRequest($url_ext);

    curl_setopt($this->ch, CURLOPT_CUSTOMREQUEST, RecordManager::DELETE_REQ);

    $this->endRequest(RecordManager::DELETE_REQ);
  }

  public function addReq($data) {
    assert(is_array($data));

    $this->startRequest('');

    $post_data = $this->postencode($data);

    curl_setopt($this->ch, CURLOPT_CUSTOMREQUEST, RecordManager::POST_REQ);
    curl_setopt($this->ch, CURLOPT_POSTFIELDS, $post_data);
    curl_setopt($this->ch, CURLOPT_POST, 1);

    curl_setopt($this->ch, CURLOPT_HTTPHEADER,
                ["Content-Type: application/x-www-form-urlencoded"]);

    $this->endRequest(RecordManager::POST_REQ);
  }

  protected function chReset() {
    curl_reset($this->ch);
  }

  protected function startRequest($url_ext) {
    $req_url = $this->base_url . $url_ext;

    curl_setopt($this->ch, CURLOPT_URL, $req_url);
    curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, 1);
  }

  protected function endRequest($req_type) {
    $req_url = curl_getinfo($this->ch, CURLINFO_EFFECTIVE_URL);

    try {
      $result = $this->makeRequest();
    } catch (Exception $e) {
      throw new Exception("Failed to make $req_type request to `$req_url'",
                          $e->getCode(), $e);
    } finally {
      $this->chReset();
    }

    return $result;
  }

  protected function postencode($data) {
    $post_data = "";

    foreach ($data as $k => $v) {
      if (!empty($post_data))
        $post_data .= '&';

      if (is_string($k))
        $post_data .= urlencode($k) . '=' . urlencode("$v");
      else
        $post_data .= urlencode("$v");
    }

    return $post_data;
  }

  private function makeRequest() {
    global $curl_error_codes;

    $result = curl_exec($this->ch);

    $ch_errno = curl_errno($this->ch);

    if ($ch_errno)
      throw new Exception(
        $curl_error_codes[$ch_errno] . "\n" .
        curl_error($this->ch));

    return $result;
  }
}

class RecordCollector {
  private $record_manager;

  private $reserve_records;
  private $lease_records;

  public function __construct($record_manager) {
    assert(is_a($record_manager, 'RecordManager'));

    $this->record_manager = $record_manager;
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

    foreach ($general_data['reservations'] as $reserve_general) {
      if (!isset($reserve_general['ip']))
        throw new Exception('Server returned reserve record without IP.');

      $reserve_detail          = $this->fetch_detail($reserve_general['ip']);
      $this->reserve_records[] = new ReserveRecord($reserve_detail);
    }

    foreach ($general_data['leases'] as $lease_general) {
      if (!isset($lease_general['ip']))
        throw new Exception('Server returned lease record without IP.');

      $lease_detail            = $this->fetch_detail($lease_general['ip']);
      $this->lease_records[]   = new LeaseRecord($lease_detail);
    }
  }

  private function fetch_general() {
    try {
      $general_data = $this->record_manager->getReq();
    } catch (Exception $e) {
      throw new Exception("Failed to fetch general data.",
                          $e->getCode(), $e);
    }

    return $general_data;
  }

  private function fetch_detail($ip) {
    try {
      $detail_data = $this->record_manager->getReq($ip);
    } catch (Exception $e) {
      throw new Exception('Failed to retrive specific record from server, ' .
                          'perhaps the records were just modified: ',
                          $e->getCode(), $e);
    }

    return $detail_data;
  }
}

// vim: set ts=2 sw=2 et syn=php:
