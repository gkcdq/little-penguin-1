# Little Penguin 1 - Assignment 01: Hello World Kernel Module

Ce projet consiste à créer un module de noyau exécutable (LKM - Loadable Kernel Module) basique qui écrit des messages spécifiques dans les logs du noyau (`dmesg`) lors de son chargement et de son déchargement.

## 📂 Fichiers inclus
* `main.c` : Le code source en C du module utilisant les macros du noyau.
* `Makefile` : Le script de compilation utilisant le système `kbuild` du kernel.

---

## 🛠️ Guide d'exécution

Toutes les commandes de manipulation du module (chargement/déchargement) nécessitent les privilèges `root`. Passez en root (`su -` ou `sudo -i`) ou utilisez `sudo` devant chaque commande.

### 1. Compilation du module
Pour compiler le code source et générer le fichier binaire du module (`main.ko`), exécutez la commande suivante dans le dossier du projet :
```bash
make
su -
insmod main.ko # pour injecter le module compile dans le noyaux linux
dmesg | tail -1 # pour verifier le chargement du module
```
### 2. Resultat
```bash
[XXXXXX.XXXXXX] Hello world!
```
### 3. Dechargement du module
```bash
su -
rmmod main
# verif du msg
dmesg | tail -1
# resultat attendu
[XXXXXX.XXXXXX] Cleaning hello world! module.
# nettoyage des fichier tmp
make clean
```
