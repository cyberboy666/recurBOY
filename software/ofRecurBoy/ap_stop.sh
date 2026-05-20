#!/bin/bash

set -e

echo "[RecurBOY] Stopping Access Point..."

# stop services first
systemctl stop hostapd
systemctl stop dnsmasq

# clean up interface state
ip addr flush dev wlan0

echo "[RecurBOY] Access Point stopped."
