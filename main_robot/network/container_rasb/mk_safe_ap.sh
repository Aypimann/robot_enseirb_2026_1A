nmcli con add type wifi ifname wlan0 con-name safe-config autoconnect yes ssid "sauvetage des petits"
nmcli con modify safe-config 802-11-wireless.mode ap
nmcli con modify safe-config wifi-sec.key-mgmt wpa-psk
nmcli con modify safe-config wifi-sec.psk "LolaPrez2026"
nmcli con modify safe-config ipv4.method link-local
nmcli con up safe-config
