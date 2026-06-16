# Assignment 06 - linux-next Kernel Compilation

## Description
Cet exercice marque une transition temporaire du développement pur de modules vers la gestion de l'infrastructure du noyau. L'objectif est de se familiariser avec l'environnement de développement "bleeding-edge" de la communauté Linux en téléchargeant, configurant, compilant et bootant le noyau **`linux-next`**.

### Qu'est-ce que le noyau `linux-next` ?
Dans le cycle de développement du noyau Linux, `linux-next` sert d'arbre d'intégration quotidien pour le cycle de développement de la prochaine version majeure. 
* Les mainteneurs de sous-systèmes (USB, réseau, drivers, etc.) y envoient leurs patchs et nouveautés une fois validés dans leurs arbres respectifs.
* Cet arbre permet de tester l'interopérabilité des modifications de chacun, de résoudre les conflits de fusion (*merge conflicts*) et de corriger les régressions de build/boot **avant** que Linus Torvalds n'ouvre la *merge window* officielle de la version suivante.

---

## Contenu du dépôt
* **`proof.txt`** : Le journal d'initialisation complet du noyau (*Kernel boot log*) extrait via la commande `dmesg`, prouvant le démarrage réussi sur la version `linux-next`.

---

## Processus de Réalisation

### 1. Clonage superficiel (Shallow Clone)
L'historique complet de `linux-next` étant massif, seul le dernier snapshot de la branche active a été récupéré pour optimiser le temps de téléchargement :
```bash
git clone --depth 1 [https://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git](https://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git)
cd linux-next
```
### 2. Configuration et Compilation

La configuration a été calquée sur les options matérielles validées lors des exercices précédents, puis mise à jour pour inclure les nouveautés de l'arbre next :
```Bash

# Import de la configuration locale actuelle
cp /boot/config-$(uname -r) .config

# Alignement de la configuration avec les nouvelles options (valeurs par défaut acceptées)
make olddefconfig

# Compilation optimisée sur l'ensemble des cœurs de la machine virtuelle
make -j$(nproc)
```
### 3. Installation et Redémarrage
```Bash

# Installation des modules et du binaire vmlinuz au sein de /boot
sudo make modules_install
sudo make install

# Redémarrage de la VM en sélectionnant l'entrée adéquate dans le menu GRUB
sudo reboot
```
### 4. Extraction du Log

Après confirmation du chargement de la bonne version système via uname -r, la preuve a été générée :
```Bash
dmesg > proof.txt
```
