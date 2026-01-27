## Étape A : installer de quoi lancer un container
sudo pacman -S debootstrap
sudo debootstrap stable /var/lib/machines/debian-target http://deb.debian.org/debian/

## Étape B : faire une copie pour éviter les betises
sudo cp -r /var/lib/machines/debian-target /var/lib/machines/machine-a

## Étape C : préparer la machine
### i) Copier les les fichier dans le container :
sudo cp ./config.sh ./mk_ap_profile.sh /var/lib/machines/machine-a/root
### ii) Rentrer dans le container
sudo systemd-nspawn -D /var/lib/machines/machine-a
## ii) Dans le container
cd
chmod +x *
./config.sh

## Étape D : lancer le container
sudo systemd-nspawn -bD /var/lib/machines/machine-a --network-interface=wlan0
-> Pour forcer l'arret : sudo machinectl terminate machine-a

## Étape E : se connecter au point d'accès et se connecter à la machine en ssh
ssh aycont@machine-a.local
