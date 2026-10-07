#!/bin/bash

set -e

echo "[RecurBOY] Starting Access Point..."

# stop client mode
systemctl stop dnsmasq || true
systemctl stop hostapd || true

systemctl stop dhcpcd || true
systemctl stop wpa_supplicant || true
systemctl stop wpa_supplicant@wlan0 2>/dev/null || true

killall wpa_supplicant 2>/dev/null || true

# reset interface state
ip link set wlan0 down || true
ip addr flush dev wlan0 || true

# switch to AP mode
iw dev wlan0 set type __ap || true

# bring interface up
ip link set wlan0 up
sleep 1

# assign static IP
ip addr add 192.168.4.1/24 dev wlan0

# start services
systemctl start hostapd
sleep 1
systemctl start dnsmasq

echo "[RecurBOY] Access Point started."
