# Assignment 05 - Misc Character Device Driver

## Description
Ce projet transforme le module noyau initial en un véritable pilote de périphérique de type caractère (**Misc Character Device**). L'utilisation du sous-système `miscdevice` de Linux permet de s'affranchir de la gestion complexe des classes `sysfs` et de l'allocation manuelle des numéros majeurs (*Major numbers*), tout en créant automatiquement un nœud d'accès dans le système de fichiers.

Le driver implémente les fonctionnalités suivantes :
* Enregistrement dynamique d'un numéro mineur (*Minor number*).
* Création automatique du nœud de périphérique virtuel dans `/dev/fortytwo`.
* Implémentation sécurisée des opérations systèmes `read` et `write` via les fonctions d'échange de mémoire `copy_to_user()` et `copy_from_user()`.
* **Comportement en lecture :** Renvoie le login étudiant (`milin`) à l'utilisateur.
* **Comportement en écriture :** Compare la chaîne reçue avec le login attendu. Si la chaîne correspond, l'écriture réussit. Sinon, le pilote lève une erreur d'argument invalide (`-EINVAL`).

---

## Contenu du dépôt
* **`main.c`** : Code source du pilote de périphérique caractère contenant les structures `file_operations` et `miscdevice`.
* **`Makefile`** : Script permettant de compiler le module contre le code source du noyau actuel.
* **`proof.txt`** : Fichier de preuve validant l'existence du périphérique, la lecture du login et le rejet des entrées erronées.

---

## Utilisation et Tests

### 1. Compilation et Chargement du module
```bash
# Compilation du driver
make

# Insertion du module au sein du noyau (en spécifiant le chemin absolu d'insmod si nécessaire)
sudo insmod main.ko
```
### 2. Validation des opérations

Une fois chargé, le périphérique /dev/fortytwo doit répondre aux commandes standard de l'espace utilisateur :
```Bash
# 1. Entête et statut du périphérique (Heure mise à jour)
echo "=== 1. Vérification de l'existence du périphérique ===" > proof.txt
ls -l /dev/fortytwo >> proof.txt

# 2. Test de lecture
echo "" >> proof.txt
echo "=== 2. Test de LECTURE (Doit afficher mon login) ===" >> proof.txt
cat /dev/fortytwo >> proof.txt

# 3. Test du bon login (Le sujet veut voir un retour propre/vide sans erreur)
echo "" >> proof.txt
echo "=== 3. Test d'ÉCRITURE avec le BON login (Doit réussir) ===" >> proof.txt
echo "milin" > /dev/fortytwo 2>> proof.txt

# 4. Test du mauvais login (Doit enregistrer l'erreur d'argument)
echo "" >> proof.txt
echo "=== 4. Test d'ÉCRITURE avec un MAUVAIS login (Doit renvoyer une erreur) ===" >> proof.txt
echo "wrong_login" > /dev/fortytwo 2>> proof.txt
```
### 3. Nettoyage du système
```Bash

# Déchargement du module
sudo rmmod main

# Nettoyage des fichiers intermédiaires de compilation
make clean
```
