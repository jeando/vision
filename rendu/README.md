Projet de Vision
================

Auteurs :
---------
Gu�non Marie & Favreau Jean-Dominique

Date :
------
06/01/2014

Librairies :
------------
Les librairies n�cessaires � ce projet sont :<br>

* openCV (opencv_core)
* openGL 3
* SFML 2.0
* glew

<br>Merci de v�rifier que ces librairies sont bien install�es sur votre ordinateur avant d'essayer de compiler notre projet.

Compilation :
-------------
Pour compiler notre projet, il suffit de faire tourner la commande `cmake .;make` dans le dossier`./Data/Code`
<br>
<b>Attention</b>: il faut donc avoir les modules `cmake` correspondants aux librairies pr�cis�es ci-dessus!

Execution :
-----------

* Si vous voulez rentrer � la main (au clic de souris) la liste des appariements :<br>
Se mettre dans le r�pertoire `./Data/code` et �xecuter le fichier `vision` cr�� lors de la compilation en lan�ant la ligne de commande `./vision_gui`
<br>(le recollement ne se fait pas automatiquement, il faudra pour cela lanc� la commande suivante)
* Si vous voulez g�n�rer le recollement � partir d'un fichier o� les appariements sont list�s : (celui-ci devra se nomer `matching_points`)<br>
Se mettre dans le r�pertoire `./Data/code` et �xecuter le fichier `vision` cr�� lors de la compilation en lan�ant la ligne de commande `./recollement`


commandes :
-----------
Dans l'interface :

* fl�ches clavier gauche/droite pour d�placer la liste d'images
* fl�ches clavier haut/bas pour faire d�filer la liste des appariements
* clique sur une image de la liste pour la s�lectionner
* clique sur les images actives pour ajouter des appariements
* `echap` pour quitter
* `w` pour sauvegarder les appariements dans un fichier `maching_points`

Dans l'affichage des recollements:

* `F1` prend une impression d'�cran sauvegard�e dans un fichier `screenshot.jpg`
* `echap` pour quitter
* `F2` calcul le recollement suivant

