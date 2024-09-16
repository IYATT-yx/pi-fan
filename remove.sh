#!/usr/bin/env bash

sudo systemctl stop pi-fan.service
sudo systemctl disable pi-fan.service
rm -rf /usr/local/bin/pi-fan /etc/systemd/system/pi-fan.service
echo "移除完成！"