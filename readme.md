Compilation :

La commande "make" produira l'éxécutable "algo_evo" grâce au makefile

Il est également possible de compiler le programme à la main :
g++ main.cpp chromosome.cpp ae.cpp population.cpp -o algo_evo


Lancement du programme :

./algo_evo <arguments>

Le programme prend au minimum 4 arguments :
-le fichier contenant les caractéristiques des intervants au format csv
-le fichier contenant les caractéristiques des missions au format csv
-le fichier contenant les distances au format csv
-le temps de recherche en secondes

exemple :
./algo_evo Intervenants.csv Missions.csv Distance.csv 60

Il est également possible de modifier les paramètres de l'algorithme au lancement en ajoutant 4 arguments supplémentaire :
-le nombre de génération
-la probabilité de croisement
-la probalilité de mutation
-la taille de la population

exemple :
./algo_evo Intervenants.csv Missions.csv Distance.csv 60 50000 0.8 0.5 50