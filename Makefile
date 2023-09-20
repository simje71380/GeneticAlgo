# Dans les makefile, on commence usuellement par d�finir des macros. 
# Si vous �crivez dans ce fichier :
#     TOTO= titi tutu tata
# ou ce qui est �quivalent mais plus beau � voir :
#     TOTO= titi \
#     <tab> tutu \
#     <tab> tata 
# make remplacera tous les $(TOTO) qu'il rencontrera dans la suite du
# fichier par :	
#     titi tutu tata

# Donc on y va pour la d�finition pr�alable de quelques macros

# Les librairies que l'on va utiliser
LIBS= -lm

# Les endroits o� on peut trouver des librairies
LIBS_PATH= \
	-L/lib \
	-L/usr/lib \
	-L/usr/local/lib

# Le compilateur que nous utiliserons
COMPILO=g++

# Les options de compilation des sources
#    -c    : Ne fait pas un ex�cutable, mais seulement un .o
#    -g    : Fait un objet d�buggable
#    -Wall : Affiche un maximum de warnings si le code C++ est douteux
#    -ansi : Assure que le code C++ suit la norme ANSI, et est donc portable.
FLAGS= -c -g -Wall -ansi

# Liste des objets que l'on va obtenir, et qu'il faudra linker.
OBJ= \
	main.o \
	ae.o \
	chromosome.o \
	population.o





# Apr�s avoir d�fini les macro, on d�finit des cibles. Une cible est une expression
# du style :
#   <nom> : <dep1> <dep2> ... <depn>
#   <tab> : <commande>
#
# Pour que make ex�cute une cible <nom>, il faut taper � l'endroit o� se trouve le makefile
#   make <nom>
# Si on ne donne pas de <nom>, la premi�re cible est ex�cut�e.
# L'ex�cution de la cible <nom> se d�roule comme suit :
#   Pour chacun des <depi>, regarder si <depi> n'est pas un nom de fichier.
#     Si <depi> n'est pas un nom de fichier
#        Alors ex�cuter la cible <depi> (qui doit �tre d�finie dans ce fichier makefile)
#              puis ex�cuter <commande>
#     Si <depi> est un fichier 
#        Si il est a jour (sa date est ant�rieur � la date de la derni�re commande make)
#           Ne rien faire
#        Si il n'est pas a jour et qu'il existe une cible <depi>, l'ex�cuter
#              puis ex�cuter <commande>
#        Si il n'est pas a jour et qu'il n'existe pas de cible <depi>
#               ex�cuter <commande>.
#
# Ce m�canisme permet, entre autre chose, de ne recompiler que les objets dont le source
# C++ correspondant � �t� modifi�.

######################
#                    #
# Cibles Principales #
#                    #
######################


# Pour compiler coup_de_vent, il faut v�rifier que tous les .o
# sont � jour (les recompiler dans le cas contraire) puis lancer
# la commande de linkage.
algo_evo : $(OBJ) 
	$(COMPILO) -o algo_evo $(OBJ) $(LIBS_PATH) $(LIBS)

# Taper 'make clean' provoque l'effacement de tous les .o, et des *~ laiss�s 
# par emacs. Il n'y a pas de d�pendence pour cette cible.
clean : 
	rm -f *.o *~

# Taper 'make clear' fait un 'make clean' puis efface en plus l'ex�cutable.
clear : 
	make clean
	rm -f algo_evo


###########################
#                         #
# Compilation des sources #
#                         #
###########################

# Source bidon, bien utile pour faire des copier-coller et obtenir les suivantes
# par en remplacement de xx.
# Le sens est que xx.o s'obtent � partir de xx.cc (la seule d�pendance ici) quand
# le fichier xx.cc vient d'�tre modifi�
%.o : %.cc
	$(COMPILO)$(FLAGS) -o $< $@ 

#xx.o : xx.cc
#	$(COMPILO) $(FLAGS) $(INCLUDE_PATH) -o xx.o xx.cc

