Projet de Vision
================

Auteurs :
---------
Guénon Marie & Favreau Jean-Dominique

Date :
------
18/12/2013

Librairies :
------------
Les librairies nécessaires à ce projet sont :<br>

* openCV (opencv_core)
* openGL 3
* SFML 2.0

<br>Merci de vérifier que ces librairies sont bien installées sur votre ordinateur avant d'essayer de compiler notre projet.

Compilation :
-------------
Pour compiler notre projet, il suffit de faire tourner la commande `make` ou `cmake` dans le fichier `./Data/Code`
Execution :
-----------
Se mettre dans le répertoire `./Data/code_back` et éxecuter le fichier `vision` créé lors de la compilation en lançant la ligne de commande `.vision`

commandes :
-----------
Dans l'interface :

* flèches clavier gauche/droite pour déplacer la liste d'images
* flèches clavier haut/bas pour faire défiler la liste des appariements
* clique sur une image de la liste pour la sélectionner
* clique sur les images actives pour ajouter des appariements
* `echap` pour quitter
* `w` pour sauvegarder les appariements dans un fichier `maching_points` et lance l'affichage du recollement

Dans l'affichage des recollements:

* `F1` prend une impression d'écran sauvegardée dans un fichier `screenshot.jpg`
* `echap` pour quitter
* `F2` calcul le recollement suivant

