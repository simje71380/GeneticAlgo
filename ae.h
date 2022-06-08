#ifndef _AE_H
#define _AE_H

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "population.h"
#include "chromosome.h"

using namespace std;
// La classe Ae d�finie les param�tres d'une ex�cution d'un algorithme
//    �volutionniste ainsi que la proc�dure principale de recherche
class Ae{
public:
    // ATTRIBUTS
	int nbgenerations;       // nombre de g�n�rations apr�s quoi la recherche est arr�t�e
	int taille_pop;          // nombre d'individus dans la population
	double taux_croisement;  // taux de croisement : valeur entre 0 et 1
	double taux_mutation;    // taux de mutation : valeur entre 0 et 1
	population *pop;         // liste des individus de la population
    int **les_distances;     // matrice des distances entre les villes
	int nb_missions;         // nombre de missions
	int nb_intervenants;     // nombre d'intervenants
	Mission *missions;      // liste des missions
	Intervenant *intervenants; // liste des intervenants
	Distance *distances;    // liste des distances
	int nb_competences;     // nombre de comp�tence différents
	string* competences;	// liste des comp�tence diff�rentes

	// CONSTRUCTEURS
	Ae(int nbg, double tcroisement, double tmutation, int tp, int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distances);
                             // constructeur de l'objet Algaorithme evolutioniste
	~Ae();                   // destructeur de l'objet Ae

	// METHODES

	chromosome* optimiser(); // fonction principale qui d�cit le d�roulement de l'algorithme �volusionniste

	// OPERATEURS DE CROISEMENT
	void croisement_competence(chromosome *pere1, chromosome *pere2, chromosome *fils1, chromosome *fils2);


	// op�rateur de croisement 1X de deux chromosomes
    void croisement1X(chromosome* parent1, chromosome* parent2,
                      chromosome* enfant1, chromosome* enfant2);
	// op�rateur de croisement 1X de deux chromosomes
    void croisement2X(chromosome* parent1, chromosome* parent2,
                      chromosome* enfant1, chromosome* enfant2);
	// op�rateur de croisement 2LOX de deux chromosomes
    void croisement2LOX(chromosome* parent1, chromosome* parent2,
                        chromosome* enfant_s1, chromosome* enfant_s2);
};

# endif
