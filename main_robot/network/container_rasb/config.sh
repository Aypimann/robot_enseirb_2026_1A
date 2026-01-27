# i) Installer les paquets rudimentaires
apt update
apt install sudo neovim git network-manager ssh avahi-daemon zsh curl -y
# ii) Définir le nom de la machine
echo "machine-a" > /etc/hostname
# ii) Ajouter un utilisateur
useradd -m aycont
# iii) Ajouter le groupe des sudoer
groupadd wheel
# iv) Rendre l'utilisateur sudoer
usermod -G sudo aycont
# v) Définir le mot de passe de l'utilisateur
passwd aycont
