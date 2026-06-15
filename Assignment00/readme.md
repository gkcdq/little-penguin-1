# Little Penguin 1 - Assignment 00: Custom Kernel Compilation

Ce projet consiste à compiler manuellement le noyau Linux à partir des sources officielles (arbre de Linus Torvalds) en y intégrant des configurations spécifiques.

## 📂 Contenu du rendu

Le dossier  contient les fichiers requis pour la validation du projet :

1. **.config**
   * **Description** : Le fichier de configuration généré et utilisé pour la compilation du noyau .
   * **Validation** : Ce fichier intègre l'option obligatoire . Cette option force le système de build à lier la version du noyau au commit Git actif, assurant la traçabilité des modifications.

2. **boot.log**
   * **Description** : Le journal de démarrage complet du système (extrait via la commande ).
   * **Validation** : Il sert de preuve d'exécution. Les premières lignes du journal démontrent que la machine virtuelle a démarré avec succès sur le noyau personnalisé, affichant la signature unique de l'exercice : .

## Resume

```bash
su -
apt-get clean && apt-get autoclean
apt-get autoremove -y
journalctl --vacuum-size=100M
exit

cd ~/Downloads
git clone --depth=1 https://github.com/torvalds/linux.git
cd linux
cp /boot/config-$(uname -r) .config
make localmodconfig

make menuconfig # Aller dans General setup ➔ Cocher Automatically append version information to the version string ([*]) ➔ Sauvegarder et quitter.

make -j$(nproc) # Compilation avec tous les cœurs CPU disponibles

su - # Installation (en root)
cd /Downloads/linux
make modules_install && make install
reboot

cd ~/Documents/little-penguin-1
mkdir -p Assignment00

dmesg > Assignment00/boot.log # Générer le log de boot réel (dmesg)

```

---
