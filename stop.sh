#!/bin/bash
sudo curl -X DELETE --data-binary @unit_app_conf.conf --unix-socket /var/run/control.unit.sock http://localhost/config
