<?php // default-config.php

if (!isset($config))
  $config = Array();

if (!isset($config['bootstrap_base']))
  $config['bootstrap_base']   = '//maxcdn.bootstrapcdn.com/bootstrap/3.3.7';

if (!isset($config['jquery_src']))
  $config['jquery_src']       = '//code.jquery.com/jquery-3.1.1.slim.min.js';

if (!isset($config['dhcp_server']))
  $config['dhcp_server']      = 'localhost';

if (!isset($config['dhcp_server_port']))
  $config['dhcp_server_port'] = 8000;

if (!isset($config['dhcp_subnet']))
  $config['dhcp_subnet']      = '192.168.1.0';

// vim: set ts=2 sw=2 et syn=php:
