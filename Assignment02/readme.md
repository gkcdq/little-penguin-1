# Little Penguin 1 - Assignment 02: Kernel Customization & Patching

Ce projet consiste à modifier la variable `EXTRAVERSION` directement dans le Makefile principal du noyau Linux afin de personnaliser la chaîne de version du système, puis à générer un patch conforme aux standards de soumission du noyau (`Documentation/SubmittingPatches`).

## 📂 Fichiers inclus dans ce rendu

* **`makefile.patch`** : Le patch Git officiel contenant la modification du `Makefile`. Il inclut une description claire et la signature obligatoire `Signed-off-by` (Developer's Certificate of Origin).
* **`boot.log`** : Le journal de démarrage de la machine (généré via `dmesg`), prouvant que le système a démarré avec succès sur le noyau personnalisé.

---

## 🛠️ Processus de réalisation

### 1. Modification du Makefile
La variable `EXTRAVERSION` présente en haut du `Makefile` à la racine des sources du noyau Linux a été éditée pour inclure le suffixe demandé :
```makefile
EXTRAVERSION = -rc7-thor_kernel
```
### 2. Compilation et installation

Le noyau a ensuite été recompilé et installé avec les commandes standard :
Bash
```
make -j$(nproc)
sudo make modules_install && sudo make install
sudo reboot
```
### 3. Vérification de la version

Au redémarrage, la commande uname -r confirme que le noyau actif intègre bien la modification :
Plaintext
```
7.1.0-rc7-thor_kernel-milin-gXXXXXX
```
### 4. Génération du patch universel

Le patch a été créé à l'aide de Git depuis l'arbre des sources du noyau en suivant scrupuleusement les consignes de Linus Torvalds (git commit -s pour la signature, puis git format-patch) :
Bash
```
git format-patch -1 HEAD --stdout > makefile.patch
```
