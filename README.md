# PROJET Analyse syntaxique
(Licence 3 - Informatique)

Réalisé par:
- Jimmy Gouraud
- Yordan Kirov
- Nicolas Palard
- Sébastien Pouteau
- Étienne Robert

# Objectif de ce projet

Ce projet permet de générer des pages html à partir de fichier écrit dans une syntaxe définie.

Utilisation de flex et bison.

Plus de détail dans le rapport.


# Compilation
    - make
         compile le projet, l'exécutable se trouve dans bin/
    - make check
         lance une batterie de test, et créer les images des arbres de nos fonctions tests, ainsi que leur page HTML, ils sont dans bin/check/
    - make page
         créser les images des arbres de nos fichiers pageHTML, ainsi que leur page HTML, ils sont dans bin/page/ (vous y trouverez introduction.html)
    - make clean
         nettoye les dossiers
    - make doc
         génère la documentation de nos fonctions, elle sera mise dans le dossier doc/
