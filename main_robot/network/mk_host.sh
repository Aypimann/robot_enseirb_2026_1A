#!/bin/bash

# 1. Nettoyage radical
nmcli con delete host_robot 2> /dev/null

# 2. Création avec liaison
nmcli con add type wifi ifname wlan0 con-name host_robot ssid "robot des petits"

# 3. Sécurité
nmcli con modify host_robot wifi-sec.key-mgmt wpa-psk
nmcli con modify host_robot wifi-sec.psk "LolaPrez2026"
nmcli con modify host_robot wifi-sec.psk-flags 0

# 4. Configuration IP + DHCP (Mode Serveur)
nmcli con modify host_robot ipv4.method shared ipv4.address 192.168.10.2/24

# 5. Tentative de connexion
nmcli con up host_robot
