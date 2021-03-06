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
	double fitness;        // la valeur de la fonction objectif (fitness) de la solution
    double fitness2;        // la valeur de la fonction objectif (fitness) 2
    double fitness3;        // la valeur de la fonction objectif (fitness) 3
    int nb_intervenants;// le nombre d'intervenants dans la solution
    int nb_missions;    // le nombre de missions dans la solutio
    float *heure_supp;    // le nombre d'heures suppl�mentaires pour chaque intervenant
    double *working_hours_weekly; // le nombre d'heures de travail pour chaque intervenant
    double *distance_par_intervenant; // la distance parcourue par chaque intervenant
    Mission *missions;  // pointeurs sur les missions
    Intervenant *intervenants; // pointeurs sur les intervenants
    Distance *distances; // pointeur sur la distance
    int nb_competences;     // nombre de comp�tence différents
	string* competences;	// liste des comp�tence diff�rentes

    //coef des pénalitées pour non respect d'une contrainte souple
    int coef_pena_pause_midi = 100; // coefficient de penalisation pour les pauses midi
    int coef_pena_heure_supp = 100; // coefficient de penalisation pour les heures suppl�mentaires
    int coef_pena_amplitude = 100; // coefficient de penalisation pour les amplitudes des journées
    int coef_pena_chevauchement_missions = 200; // coefficient de penalisation pour les chevauchements de missions
    int coef_pena_temps_deplacement = 200; // coefficient de penalisation pour les temps de déplacement entre mission
    int coef_pena_mission_affecter_plusieurs_fois = 1000; // coefficient de penalisation pour les missions affect�es plusieurs fois
    int coef_pena_temps_travail_journalier = 50; // coefficient de penalisation pour les temps de travail journalier
    int type_pena[7] ={0,0,0,0,0,0,0}; //compte le nombre de pénalités de chaque type
    

	// CONSTRUCTEURS
	chromosome(int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distance); // constructeur de l'objet al�atoirement
	~chromosome();      // destructeur de l'objet

	// METHODES
	void afficher();    // fonction d'affichage du chromosome (i.e. de la solution)
	void evaluer(); // fonction d'evaluation de la solution
	                    // fonction d'�valuation du chromosome (c-�-d calcul la fitness)
                        //   Elle doit etre lanc�e � la creation des solution et apres
					    //   l'ex�cution des operateurs de mutation et de croisement
    void evaluer2(); //fitness 2
    void evaluer3(); //fitness 3
    
    bool hasAllMissionsAffected();
    bool hasIntervenantCompetence();
    int hasOnlyOneMissionOrTime(); //retourne le nombre de missions qui sont superposées ou les missions ou l'intervenant n'a pas le temps d'aller d'une mission vers l'autre
    double countPenalties(); //retourne le nombre de fois ou les contraintes souples sont non respectées
    int* OrdonnerMissions(int *missions, int *out, int size); //retourne un tableau ordonné des missions
    bool identique(chromosome* chro); // test si 2 chromosome sont identique
    void copier(chromosome* source);  // copie le chromosome 'source'
    void repair_comp(); // corrige les problèmes de la solution
    void show_type_pena(); // affiche le nombre de pénalités de chaque type
    void countPenalties_show_infos(); // affiche les infos sur les contraintes souples non respectées de la solution

    // OPERATEURS DE MUTATION
    void echange_2_genes(int gene1,     // interchange 2 g�nes du chromosome
                         int gene2);
    void echange_2_genes_quelconques(); // interchange al�atoirement 2 g�nes du chromosome
    void echange_2_genes_entre_intervenant_meme_competences(); // interchange al�atoirement 2 g�nes du chromosome entre les intervenants ayant les m�mes comp�tences
    void distance_per_intervenant(); //retourne la distance entre les missions affect�es par l'intervenant
};

# endif
