

# Contenu de l'archive
Cette archive est constituée des fichiers suivants :

- README.md : fichier décrivant le contenu de l'archive ;
- main.c : fichier source principale du projet, contenant la fonction main()
  du programme ;
- api.h : fichier d'en-tête décrivant les deux appels principaux de l'API, un
  pour récupérer le contenu de la hiérarchie à "copier", et un second pour
  créer un répertoire "phantom", copiant les méta-données de la hiérarchie
  "copiée" ;
- internals.h : fichier d'en-tête décrivant les appels de base utilisés par
  l'API qui seront testés unitairement.
- Makefile : matériel de compilation basique

# Quelques informations
Le code fourni est censé être immuable :

- aucune structure de donnée ne peut être modifiée ;
- aucun prototype de fonction donné ne peut être modifié ;
- le code de la fonction main() dans main.c peut évidemment être modifié,
  mais l'usage du programme (à savoir qu'il prend deux chemins en argument)
  ne doit lui pas être modifié ;
- vous pouvez rajouter n'importe quel nombre de fonctions dans les fichiers
  main.c, api.c et internals.c, et même d'autres fichiers -- vous n'êtes pas
  bloqués aux seuls prototypes fournis dans cette archive ;
- vous pouvez modifier le Makefile fourni.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
