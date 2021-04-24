

#   COLNOT Clémence 21801866


#   Modalités de soumission
Ce TD est à faire en monôme, et à rendre au plus tard le 21 mars 2021,
23:59 (France métropolitaine) sur moodle, dans l'espace dédié. Il est à
rendre dans une archive .tar, contenant les fichiers sources et d'en-tête,
le matériel de compilation nécessaire et un fichier README.md dans lequel
vous mettrez votre nom, prénom et numéro d'étudiant.

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

# Remontée de bugs
N'hésitez pas à remonter tout problème d'implémentation majeur (soupçon
d'erreur dans un prototype, erreur dans la documentation, matériel de
compilation bugué, etc.) à l'adresse pro.seb.gougeaud@gmail.com
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        