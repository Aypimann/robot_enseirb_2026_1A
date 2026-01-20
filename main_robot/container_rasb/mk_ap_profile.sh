nmcli con add type wifi ifname wlan0 con-name config-hotspot autoconnect yes ssid "robot des petits"
nmcli con modify config-hotspot 802-11-wireless.mode ap
nmcli con modify config-hotspot wifi-sec.key-mgmt wpa-psk
nmcli con modify config-hotspot wifi-sec.psk "LolaPrez2026"
nmcli con modify config-hotspot ipv4.method shared
nmcli con up config-hotspot
