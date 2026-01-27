# 1. Créer un point de sauvegarde de l'état actuel
sudo nmcli general reload

# Si on ne touche à rien, il va y avoir un redémarrage 
# qui roll back là la sauvegarde juste au dessus
sudo shutdown -r +5 "Redémarrage de sécurité réseau"

# Ici je peux faire des trucs dangeureux.


# Si ça marche je suis toujours connecté donc je peux annuler le redémarrage
sudo shutdown -c
