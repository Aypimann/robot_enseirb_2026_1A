nmcli con add type wifi ifname wlan0 con-name rasp_net autoconnect yes ssid "robot des petits"
nmcli con modify rasp_net 802-11-wireless.mode ap
nmcli con modify rasp_net wifi-sec.key-mgmt wpa-psk
nmcli con modify rasp_net wifi-sec.psk "LolaPrez2026"
nmcli con modify rasp_net ipv4.method manual ipv4.address 192.168.10.1/
nmcli con modify rasp_net ipv4.gateway 192.168.10.2 ipv4.dns 8.8.8.8
nmcli con up rasp_net
