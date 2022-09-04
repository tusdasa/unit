#!/bin/bash
sudo curl -X PUT --data-binary @unit_app_conf.conf --unix-socket /var/run/control.unit.sock http://localhost/config
