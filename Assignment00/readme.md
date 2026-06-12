# Little Penguin 1 - Assignment 00: Custom Kernel Compilation

Ce projet consiste à compiler manuellement le noyau Linux à partir des sources officielles (arbre de Linus Torvalds) en y intégrant des configurations spécifiques.

## 📂 Contenu du rendu

Le dossier  contient les fichiers requis pour la validation du projet :

1. ****
   * **Description** : Le fichier de configuration généré et utilisé pour la compilation du noyau .
   * **Validation** : Ce fichier intègre l'option obligatoire . Cette option force le système de build à lier la version du noyau au commit Git actif, assurant la traçabilité des modifications.

2. ****
   * **Description** : Le journal de démarrage complet du système (extrait via la commande ).
   * **Validation** : Il sert de preuve d'exécution. Les premières lignes du journal démontrent que la machine virtuelle a démarré avec succès sur le noyau personnalisé, affichant la signature unique de l'exercice : .

---
