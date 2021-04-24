% IN405 TD8 -- Systèmes d'exploitation
% Dictionnaire Pascal

# Modalités de soumission
Ce TD est à faire en monôme, et à rendre au plus tard le 25 avril 2021,
23:59 (France métropolitaine) sur moodle, dans l'espace dédié. Il est à
rendre dans une archive .tar, contenant les fichiers source à rendre, le
matériel de compilation et un fichier 'README.md' dans lequel vous mettrez
votre nom, prénom et numéro d'étudiant. Plus d'informations dans la section
"Rendu et notation du TD noté".

# Contenu de l'archive
Cette archive est constituée des fichiers suivants :

- le fichier 'README.md', que vous êtes en train de lire ;
- le fichier 'client_template.c', fichier source pouvant servir de base pour
  l'implémentation du côté client : seules les fonctions de communication
  manquent ;
- le fichier 'server_template.c', fichier source pouvant servir de base pour
  l'implémentation du côté serveur : seules les structures de données, la
  fonction main() et les prototypes de fonctions sont implémentés.

# Utilisation
La compilation des fichiers doit se faire simplement à l'aide des commandes
ci-dessous :

```bash
$ gcc -o server server.c
$ gcc -o client client.c
```

L'utilisation, comme décrite dans l'énoncé du TD ainsi que dans les 'usage()'
des fichiers source 'client_template.c' et 'server_template.c', se fait comme
ci-dessous :

```bash
$ ./server math 4 &
$ ./client math + pi 3.14
$ ./client math ? pi
$ ./client math !
```

# Quelques informations
- les fichiers sources 'xyz_template.c' vous sont proposés, mais libre à vous
  de les utiliser ou non : aucun test unitaire ne sera fait sur les fonctions ;
- les noms des tubes créés par le serveur doivent avoir comme chemin
  `/tmp/dict_NAME_{in/out}`, avec `NAME` le nom du dictionnaire que vous donnez
  en argument des programmes et `in` ou `out` en fonction du sens de lecteur,
  ils sont correctement utilisés côté client dans le fichier 'client.c'.

# Rendu et notation du TD noté
Le rendu consistera en une simple archive ne contenant qu'un fichier
'README.md' ainsi que les fichiers source 'server.c' et 'client.c' que vous
aurez implémenté. Théoriquement, le code devrait être assez court (moins de 400
lignes par fichier), et ne nécessite donc pas de plus le découper que comme
proposé. Si jamais vous souhaitez tout de même le faire, veuillez ajouter un
Makefile permettant de compiler le client et le serveur avec les commandes
respectives `make client` et `make server`.

La notation va se faire par le biais de divers scripts d'intégration, qui vont
donc tester le comportement global du serveur en utilisant diverses commandes
du client.

Une passe d'analyse statique du code, et optionnellement dynamique, viendront
s'ajouter à la note.

Tout comme pour le premier TD noté, une passe de similitude sera effectuée
entre chaque rendu, et la note sera impactée si le coefficient de similitude
est trop important.

# Remontée de bugs
N'hésitez pas à remonter tout problème d'implémentation majeur (soupçon
d'erreur dans un prototype, erreur dans la documentation, matériel de
compilation bugué, etc.) à l'adresse pro.seb.gougeaud@gmail.com