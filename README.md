Space Invaders - Projet C (Architecture MVC)

Un clone classique du jeu Space Invaders développé en langage C, utilisant les bibliothèques SDL3 pour les graphismes et Ncurses pour le mode terminal.


1. Architecture du Projet : Le Modèle MVC
Ce projet suit strictement le motif de conception MVC (Modèle-Vue-Contrôleur) afin de séparer la logique métier de l'interface utilisateur.
- Modèle (src/model/) : Gère l'état du jeu (positions des entités, scores, niveaux, détection des collisions). Il est indépendant de toute interface graphique.
- Vue (src/view/) : Responsable de l'affichage. Le projet dispose de deux implémentations :
    view_sdl.c : Affichage graphique haute performance.
    view_ncurses.c : Affichage rétro en mode texte dans le terminal.
- Contrôleur (src/controller/) : Intercepte les entrées clavier de l'utilisateur et envoie des commandes au Modèle ou à la Vue pour mettre à jour l'état du jeu.


2. Dépendances et Versions Recommandées
Pour compiler et exécuter ce projet, vous devez installer les bibliothèques suivantes :
Dépendance | Version Recommandée | Description 
GCC | 9.0 ou supérieur | Compilateur C (support standard C99). 
SDL3 | Dernière version stable | Gestion des graphismes et des fenêtres. 
Ncurses | 6.0 ou supérieur | Gestion de l'affichage en mode texte. 
Make | 4.0 ou supérieur | Automatisation de la compilation. 



3. Compilation et Exécution
Compilation: 
Ouvrez un terminal dans le dossier racine du projet et exécutez la commande suivante :
Dans terminal on tape:
make ( ou make clean && make)
Exécution: 
- Pour version SDL: 
    ./space_invaders_app -sdl 
    ou : make run-sdl
-Pour version text: 
    ./space_invaders_app -text 
    ou : make run-ncurses

4. Commandes Clavier (Commandes de Jeu)
Pour contrôler le jeu, utilisez les touches suivantes :
- Flèche Gauche : Déplacer le vaisseau vers la gauche.
- Flèche Droite : Déplacer le vaisseau vers la droite.
- Espace : Tirer un projectile pour détruire les ennemis.
- Touche R : Recommencer une nouvelle partie après un Game Over.
- Touche Q ou ESC : Quitter immédiatement l'application.


5. État de Fin de Jeu (Game Over)
Le système de fin de partie fonctionne de la manière suivante :
- Lorsque le joueur perd toutes ses vies, l'écran de jeu devient totalement noir pour marquer la fin.
- Le message "End!" s'affiche en haut dans l'écran dans version sdl et au centre dans version text
- Le Score Final du joueur est affiché juste à côté du message "End!".

- À ce stade, l'utilisateur a deux choix : appuyer sur 'R' pour relancer le jeu ou sur 'Q' pour fermer le programme.
 
  


https://github.com/user-attachments/assets/2f2c58f5-9de1-4232-94c9-15000195f87e







