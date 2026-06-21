# Little Penguin 1 - Linux Kernel Development 🐧

Ce projet est une introduction intensive au développement de modules et à la manipulation du noyau Linux (Kernel), calquée sur le modèle d'une "Piscine" de l'école 42. L'objectif est d'apprendre à compiler, configurer, patcher le noyau, et interagir avec l'espace utilisateur via différents sous-systèmes (procfs, debugfs, misc devices, udev).

---

## 🛠️ Environnement Requis
* Une distribution Linux personnalisée (Custom Linux Distribution).
* L'outil `git` installé et configuré.
* Les outils essentiels de compilation (`build-essential`, `gcc`, `make`, `bc`, `libelf-dev`, `libssl-dev`).

---

## 📋 Résumé des Assignments

### 🛠️ Chapitre V : Assignment 00 - Custom Kernel Build
* **Objectif :** Télécharger l'arbre Git officiel du noyau Linux de Linus Torvalds, le configurer, le compiler et démarrer dessus.
* **Contrainte technique :** Activer impérativement l'option `CONFIG_LOCALVERSION_AUTO=y` dans le fichier de configuration.
* **Livrables :** Le fichier de configuration `.config` et les logs de démarrage (`dmesg`).

### 💬 Chapitre VI : Assignment 01 - Hello World Module
* **Objectif :** Écrire un module noyau basique chargeant et déchargeant un message dans les logs systèmes.
* **Comportement attendu :** * `insmod` -> Affiche `Hello world!` dans `dmesg`.
  * `rmmod` -> Affiche `Cleaning up module.` dans `dmesg`.
* **Contrainte :** Assurer la compatibilité multi-version du code.
* **Livrables :** Code source (`main.c`) et `Makefile`.

### 🔀 Chapitre VII : Assignment 02 - Kernel Patching
* **Objectif :** Modifier le `Makefile` principal du noyau Linux de l'Assignment 00 pour altérer la variable `EXTRAVERSION`.
* **Comportement attendu :** La version du noyau actif (`uname -r`) doit inclure le suffixe `-thor_kernel`.
* **Livrables :** Log de boot et un fichier `.patch` conforme aux normes de soumission de Linux (`Documentation/SubmittingPatches`).

### 🪥 Chapitre VIII : Assignment 03 - Code Style Fix (Linux Way)
* **Objectif :** Nettoyer et reformater un fichier C fourni, truffé d'erreurs de syntaxe et de style, pour le rendre conforme à la charte officielle de développement Linux.
* **Contrainte :** Respect strict des règles édictées dans `Documentation/CodingStyle`.
* **Livrables :** Le fichier C nettoyé et fonctionnel.

### 🔌 Chapitre IX : Assignment 04 - USB Autoloading (Udev)
* **Objectif :** Modifier le module de l'Assignment 01 pour qu'il se charge automatiquement dès qu'un clavier USB est branché sur la machine.
* **Ressource utile :** Chapitre 14 du livre *Linux Device Drivers, 3rd Edition*.
* **Livrables :** Fichier de règles (ex: `udev`), code source mis à jour et preuves de fonctionnement.

### 💾 Chapitre X : Assignment 05 - Misc Character Device
* **Objectif :** Transformer le premier module en un pilote de périphérique de type "misc character device" accessible via le nœud `/dev/fortytwo`.
* **Comportement attendu :** * Attribution d'un numéro mineur dynamique.
  * Une lecture (`read`) renvoie votre login 42.
  * Une écriture (`write`) compare la chaîne reçue avec votre login (renvoie une erreur si non identique).
* **Livrables :** Code source et preuves de bon fonctionnement.

### 🧬 Chapitre XI : Assignment 06 - Linux-Next Build
* **Objectif :** Comprendre le cycle de développement de Linux en téléchargeant, configurant et démarrant sur la branche de développement la plus récente : `linux-next`.
* **Ressource utile :** `Documentation/development-process/`.
* **Livrables :** Log de boot du noyau `linux-next`.

### 📂 Chapitre XII : Assignment 07 - Debugfs Deep Dive
* **Objectif :** Créer une arborescence dans le système de fichier virtuel `debugfs` sous le dossier `/sys/kernel/debug/fortytwo/` contenant 3 fichiers virtuels :
  * `id` : Même comportement (lecture/écriture du login) que l'Assignment 05.
  * `jiffies` : En lecture seule, renvoie la valeur actuelle du compteur *jiffies* du kernel.
  * `foo` : Lecture publique, écriture restreinte à `root`. Stocke jusqu'à une page de données. Implémentation obligatoire de verrous (locks) pour la concurrence.
* **Livrables :** Code source et preuves.

### 🕵️‍♂️ Chapitre XIII : Assignment 08 - Reverse Driver Obscurity
* **Objectif :** Analyser, corriger le style et réparer le comportement d'un code source de module "mystère" fourni (qui s'avère être un périphérique inversant les chaînes de caractères écrites dedans).
* **Livrables :** Fichier C propre, sécurisé au niveau de l'allocation mémoire et fonctionnel.

### 🗺️ Chapitre XIV : Assignment 09 - Procfs Mounts Lister
* **Objectif :** Écrire un module noyau qui liste l'ensemble des points de montage actuels du système d'exploitation à travers un fichier virtuel dédié `/proc/mymounts`.
* **Comportement attendu :** Le format de sortie lors d'un `cat /proc/mymounts` doit être `<nom> <point_de_montage>`.
* **Livrables :** Code source et `Makefile`.

---

## 🚀 Commandes Utiles (Mémo)

### Compilation d'un module
```bash
make -C /lib/modules/$(uname -r)/build M=$(pwd) modules
