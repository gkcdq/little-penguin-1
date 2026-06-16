# Assignment 07 - Debugfs Virtual Filesystem Driver

## Description
L'objectif de cet exercice est d'explorer l'interface de communication noyau/utilisateur via le système de fichiers virtuel **`debugfs`**. Contrairement à `sysfs` ou `procfs`, `debugfs` n'impose aucune contrainte de format sur les données exportées, ce qui en fait l'outil privilégié des développeurs de modules pour le débogage.

Le module crée un sous-répertoire `/sys/kernel/debug/fortytwo/` contenant trois fichiers virtuels aux comportements distincts :

1. **`id`** : Accessible en lecture/écriture par tous les utilisateurs (`0666`). Il renvoie le login étudiant (`milin`) en lecture et n'accepte que ce même login en écriture (identique à l'Assignment 05).
2. **`jiffies`** : Accessible en lecture seule par tous (`0444`). Il expose dynamiquement la valeur actuelle du compteur de temps global du noyau (`jiffies`).
3. **`foo`** : Accessible en écriture uniquement par l'administrateur (`root`) et lisible par tous (`0644`). Les données y sont stockées (limite d'une page mémoire, soit 4096 octets). Une gestion de la concurrence via un verrou **Mutex** est implémentée pour prévenir les conditions de course (*race conditions*).

---

## Contenu du dépôt
* **`main.c`** : Code source du pilote exploitant les APIs `<linux/debugfs.h>`, `<linux/mutex.h>` et `<linux/jiffies.h>`.
* **`Makefile`** : Script de compilation classique du module.
* **`proof.txt`** : Journal d'exécution validant les permissions de fichiers, la mise à jour des jiffies et le cloisonnement des droits de `foo`.

---

## Guide d'Utilisation et Tests

### 1. Pré-requis et Chargement
Assurez-vous que le système de fichiers debugfs est bien monté à l'emplacement standard, puis insérez le module :
```bash
# Montage si nécessaire
sudo mount -t debugfs none /sys/kernel/debug/ 2>/dev/null

# Compilation et chargement
make
sudo insmod main.ko
```
### 2. Validation des Fichiers
Fichier id (Permissions globales)
```Bash

# Lecture (renvoie le login)
cat /sys/kernel/debug/fortytwo/id

# Écriture valide / invalide
echo "milin" > /sys/kernel/debug/fortytwo/id       # Succès
echo "incorrect" > /sys/kernel/debug/fortytwo/id   # Erreur : Invalid argument
```
Fichier jiffies (Compteur système temps réel)
```Bash

# Les lectures successives doivent montrer une valeur incrémentée
cat /sys/kernel/debug/fortytwo/jiffies
sleep 1
cat /sys/kernel/debug/fortytwo/jiffies
```
Fichier foo (Persistance et restriction Root)
```Bash

# Écriture de données par Root
sudo sh -c 'echo "Donnees_Securisees" > /sys/kernel/debug/fortytwo/foo'

# Lecture autorisée pour tout le monde
cat /sys/kernel/debug/fortytwo/foo

# Tentative d'écriture par un utilisateur classique (doit échouer)
echo "Hack" > /sys/kernel/debug/fortytwo/foo       # Erreur : Permission denied
```
### 3. Nettoyage du système
```Bash

# Déchargement du module (les fichiers debugfs sont automatiquement détruits)
sudo rmmod main
make clean
```
