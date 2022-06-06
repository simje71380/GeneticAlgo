#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "random.h"
#include "mission.h"
#include "intervenant.h"
#include "distance.h"



// La classe chromosome represente la structure d'une solution du probl�me
class chromosome{
public:
    // ATTRIBUTS
	int *genes;         // les g�nes du chromosome/solution
	int taille;         // la taille du chromosome = nombre de g�nes
	int fitness;        // la valeur de la fonction objectif (fitness) de la solution
    int nb_intervenants;// le nombre d'intervenants dans la solution
    int nb_missions;    // le nombre de missions dans la solution
    Mission *missions;  // pointeurs sur les missions
    Intervenant *intervenants; // pointeurs sur les intervenants
    Distance *distances; // pointeur sur la distance

	// CONSTRUCTEURS
	chromosome(int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distance); // constructeur de l'objet al�atoirement
	~chromosome();      // destructeur de l'objet

	// METHODES
	void afficher();    // fonction d'affichage du chromosome (i.e. de la solution)
	void evaluer(); // fonction d'evaluation de la solution
	                    // fonction d'�valuation du chromosome (c-�-d calcul la fitness)
                        //   Elle doit etre lanc�e � la creation des solution et apres
					    //   l'ex�cution des operateurs de mutation et de croisement
    void copier(chromosome* source);  // copie le chromosome 'source'
    bool hasAllMissionsAffected();
    bool hasIntervenantCompetence();
    int hasOnlyOneMissionOrTime(); //retourne le nombre de missions qui sont superposées ou les missions ou l'intervenant n'a pas le temps d'aller d'une mission vers l'autre
    int countPenalties(); //retourne le nombre de fois ou les contraintes souples sont non respectées
    int* OrdonnerMissions(int *missions, int *out, int size); //retourne un tableau ordonné des missions
    bool identique(chromosome* chro); // test si 2 chromosome sont identique

    // OPERATEURS DE MUTATION
    void echange_2_genes(int gene1,     // interchange 2 g�nes du chromosome
                         int gene2);
    void echange_2_genes_consecutifs(); // interchange 2 g�nes cons�cutifs du chromosome
    void echange_2_genes_quelconques(); // interchange al�atoirement 2 g�nes du chromosome
    void deplacement_1_gene();          // d�place un g�ne dans le chromosome
    void inversion_sequence_genes();    // inverse une s�quence de g�nes du chromosome
};

# endif
