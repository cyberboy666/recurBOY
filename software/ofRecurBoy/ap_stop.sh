#!/bin/bash

set -e

echo "[RecurBOY] Stopping Access Point..."

# stop AP services
systemctl stop hostapd || true
systemctl stop dnsmasq || true

# bring interface down
ip link set wlan0 down || true

# restore managed mode
iw dev wlan0 set type managed || true

# clear AP static IPs
ip addr flush dev wlan0 || true

# bring interface up
ip link set wlan0 up

# restart Wi-Fi stack
systemctl restart wpa_supplicant || true

# restart DHCP client
systemctl restart dhcpcd || true

echo "[RecurBOY] Access Point stopped"
