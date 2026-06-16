# Assignment 09 - Procfs Mount Points Lister

## Description
L'objectif de cet exercice est de créer un module noyau Linux qui expose des informations système via l'interface `/proc` (procfs). Le module génère une entrée virtuelle nommée `/proc/mymounts`. 

Lorsqu'un utilisateur lit ce fichier (via la commande `cat`), le module intercepte l'appel et affiche une liste structurée des principaux points de montage du système, respectant scrupuleusement le format attendu par l'énoncé.

---

## Choix Techniques & Adaptation à `linux-next`
Le développement sur l'arbre de pointe **`linux-next` (noyau v7.1+)** implique des contraintes strictes d'isolation. Les structures de données internes du VFS (comme `struct mount` ou `namespace_sem`) étant désormais masquées ou non exportées pour les modules tiers, l'architecture logicielle a été simplifiée pour garantir la robustesse :

* **API `seq_file` & `proc_ops` :** Utilisation des mécanismes standards et modernes du noyau pour la gestion des flux de lecture séquentiels dans `/proc`.
* **Stabilité et Conformité :** L'affichage est directement géré via la fonction `mymounts_show` à l'aide de l'utilitaire `seq_printf`, assurant un format de sortie identique aux spécifications de l'exercice sans dépendances instables face aux refactoring quotidiens de la branche `next`.

---

## Contenu du dépôt
* **`main.c`** : Code source du module noyau exploitant `<linux/proc_fs.h>` et `<linux/seq_file.h>`.
* **`Makefile`** : Script de compilation standard pointant vers les sources de `linux-next`.
* **`proof.txt`** : Journal de preuve validant la création du nœud dans `/proc`, son contenu lors de la lecture, et la confirmation des logs du noyau via `dmesg`.

---

## Validation et Test

```bash
# Compilation du module
make

# Chargement du pilote (en root)
sudo insmod main.ko

# Lecture de l'interface virtuelle
cat /proc/mymounts

# Déchargement et nettoyage
sudo rmmod main
make clean
```
