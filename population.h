 #ifndef POPULATION_H
#define POPULATION_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "chromosome.h"

// La classe population englobe plusieurs solution potentielle du probleme
class population{
public:
    // ATTRIBUTS
	chromosome **individus;  // liste des individus de la population
	int taille_pop;          // nombre d'individus de la population
	int *ordre;              // tableau donnat l'ordre des individus dans la population
                             //   du meilleur au plus mauvais en fonction de la fitness	population(int tp, int tc);    // constructeur de l'objet

	// CONSTRUCTEURS
	population(int tp, int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distance); // constructeur de l'objet
	~population();              // destructeur de l'objet

	// METHODES
	void statiatiques(); // affiche quelques statistiques sur la population
	int nb_chromosomes_similaires(chromosome* chro);
	                     // compte le nombre de chromosomes similaires � 'chro'
	void similitude();   // affiche les r�sultats du comptage de chromosomes similaires
	                     // aux meilleurs individus de la population.

    // OPERATEURS DE SELECTION ET DE REMPLACEMENT
    chromosome *selection_roulette();                 // sel�ction par roulette biais�e d'un individu de la population
    void remplacement_roulette(chromosome *individu); // rempacement par roulette biais�e d'un individu de la population par un chromosome donn�

    void ordonner();
    void reordonner();
    void afficher();
};

# endif
