#!/bin/bash

set -e

echo "[RecurBOY] Starting Access Point..."

# safety: ensure wlan0 is up
ip link set wlan0 up

# give AP interface its static IP
ip addr add 192.168.4.1/24 dev wlan0 2>/dev/null || true

# start services (order matters)
systemctl start dnsmasq
systemctl start hostapd

echo "[RecurBOY] Access Point started."
