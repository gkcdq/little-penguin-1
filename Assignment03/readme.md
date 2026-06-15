# Little Penguin 1 - Assignment 03: Linux Kernel Coding Style

Ce projet consiste à restructurer et corriger un fichier source en langage C afin de le rendre 100% conforme aux normes de codage officielles du noyau Linux (décrites dans `Documentation/process/coding-style.rst`).

## 📂 Fichier inclus dans ce rendu

* **`CodingStyle.c`** : Le fichier C corrigé, nettoyé de tout code mort, correctement typé pour les fonctions d'initialisation du noyau, et aligné sur les règles d'indendation strictes de Linus Torvalds.

---

## 🛠️ Processus de correction et de validation

Le code a été audité et corrigé en utilisant l'outil officiel de vérification du Git de Linux : **`checkpatch.pl`**.

### 1. Critères de style appliqués
* **Indentation** : Remplacement de tous les espaces par des tabulations réelles de 8 caractères.
* **Gestion des accolades `{}`** : Ajustement des blocs conditionnels `if` et des boucles `for` selon la norme du noyau (pas d'accolades superflues pour les blocs d'une seule ligne).
* **Identifiants de Licence** : Ajout de la balise obligatoire `// SPDX-License-Identifier: GPL-2.0` en première ligne.
* **Nettoyage du code** : Suppression du code mort (instruction `return 1;` inaccessible) et ajout des macros de section de mémoire `__init` et `__exit`.

### 2. Commande de validation
La conformité finale a été validée via la commande suivante :
```bash
~/Downloads/linux/scripts/checkpatch.pl --file --no-tree CodingStyle.c
```
