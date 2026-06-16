# Assignment 04 - USB Keyboard Hotplug Module

## Description
Ce projet complète le module noyau écrit lors de l'Assignment 01. L'objectif est de configurer le système pour qu'il charge automatiquement ce module lorsqu'un périphérique de type clavier USB est détecté (mécanisme de *hotplug*), sans aucune intervention manuelle (`insmod`).

Le déclenchement repose sur deux piliers :
1. **La table d'identifiants (`MODULE_DEVICE_TABLE`)** exportée par le module (`main.c`), permettant à `depmod` d'associer le module aux signatures matérielles des claviers USB.
2. **Une règle `udev` (`99-usb-kbd.rules`)** en espace utilisateur (*userspace*) qui intercepte l'événement d'ajout de périphérique pour appeler `modprobe main`.

---

## Contenu du dépôt
* **`main.c`** : Code source du module de l'Assignment 01, mis à jour avec la structure `struct usb_device_id` ciblant la classe HID/Keyboard et la macro `MODULE_DEVICE_TABLE`.
* **`Makefile`** : Script de compilation du module pour le noyau actuel.
* **`99-usb-kbd.rules`** : Fichier de configuration pour le gestionnaire de périphériques `udev`.
* **`proof.txt`** : Fichier de preuve attestant du chargement automatique réussi du module (`lsmod` & `dmesg`).

---

## Installation et Test

### 1. Compilation et déploiement du module
```bash
# Compilation du module
make

# Déploiement dans l'arborescence des modules du noyau actuel
sudo cp main.ko /lib/modules/$(uname -r)/kernel/drivers/usb/

# Mise à jour des dépendances et des alias de modules
sudo depmod -a
```

### 2. Configuration de la règle Udev
```Bash

# Copie de la règle dans le répertoire système d'udev
sudo cp 99-usb-kbd.rules /etc/udev/rules.d/

# Rechargement de la configuration udev
sudo udevadm control --reload-rules
```
### 3. Simulation du Hotplug (Déclenchement)

Pour tester le comportement sans manipulation physique de matériel (notamment au sein d'une machine virtuelle), le signal de détection d'un périphérique d'entrée USB est simulé logiciellement :
```Bash

sudo udevadm trigger --action=add --subsystem-match=input
```
### 4. Vérification

Le succès de l'opération se valide en vérifiant que le module apparaît de lui-même dans la liste des modules actifs :
```Bash

lsmod | grep main
```
