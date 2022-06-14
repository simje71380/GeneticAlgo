#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>
#include <string.h>

#include "chromosome.h"
#include "population.h"
#include "ae.h"	
#include "Liste.h"

using namespace std;


Intervenant *intervenants;
Mission *missions;
Distance *distances;
int nb_intervenants;
int nb_missions;
int taille_pop = 25;


int nbg = 50000;
double tcroisement = 0.5;
double tmutation = 0.75;



void getData(char *filename, char *filename2, char *filename3){
	string line, word;
    int count = 0;
    int i = 0;
    //fichier intervenant
	fstream file (filename, ios::in);
	if(file.is_open())
	{
        while(file.peek()!=EOF)
        {
            getline(file, line);
            count++;
        }
        file.close();
    }
    file.open(filename, ios::in);
    if(file.is_open()){
        nb_intervenants = count;
        intervenants = new Intervenant[nb_intervenants];
		while(getline(file, line))
		{
			stringstream str(line);
            getline(str, word, ',');
            int id = atoi(word.c_str());
            getline(str, word, ',');
            string competence = word;
            getline(str, word, ',');
            string specialite = word;
            getline(str, word, ',');
            int tps_hebdo = atoi(word.c_str());
            intervenants[i].SetId(id);
            intervenants[i].SetCompetence(competence);
            intervenants[i].SetSpecialite(specialite);
            intervenants[i].SetTpsHebdo(tps_hebdo);
            i++;
		}
        file.close();
	}
	else
		cout<<"Could not open the file: " << filename << endl;

    //fichier missions
    fstream file2 (filename2, ios::in);
	if(file2.is_open())
	{
        count = 0;
        while(file2.peek()!=EOF)
        {
            getline(file2, line);
            count++;
        }
        nb_missions = count;
        file2.close();
    }
    missions = new Mission[nb_missions];
    file2.open(filename2, ios::in);
    if(file2.is_open()){
        nb_missions = count;
        i = 0;
		while(getline(file2, line))
		{
			stringstream str(line);
            getline(str, word, ',');
            int id = atoi(word.c_str());
            getline(str, word, ',');
            int jour = atoi(word.c_str());
            getline(str, word, ',');
            int debut_min = atoi(word.c_str());
            getline(str, word, ',');
            int fin_min = atoi(word.c_str());
            getline(str, word, ',');
            string competence = word;
            getline(str, word, ',');
            string specialite = word;
            specialite = word.substr(0,word.size()-1);;
            missions[i].SetId(id);
            missions[i].SetJour(jour);
            missions[i].SetDebutMissionMinute(debut_min);
            missions[i].SetFinMissionMinute(fin_min);
            missions[i].SetCompetence(competence);
            missions[i].SetSpecialite(specialite);
            i++;
		}
        file2.close();
	}
	else
		cout<<"Could not open the file: " << filename2 << endl;

    //distances
    distances = new Distance(filename3);

}

int main(int argc, char **argv){
    if(argc-1 < 4){
        std::cout << "Intervenant <fichier.csv>\nMission <fichier.csv>\nDistances <fichier.csv>\nTemps de recherche <temps en seconds>\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(argc-1 != 8 && argc-1 != 4){
        std::cout << "Intervenant <fichier.csv>\nMission <fichier.csv>\nDistances <fichier.csv>\nTemps de recherche <temps en seconds>\nNombre de générations <nbg>\nTaux de croisement <[0.0;1.0]>\nTaux de mutation <[0.0;1.0]>\nTaille de la population <taille>\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    getData(argv[1], argv[2], argv[3]);
    int temps_exec_sec = atof(argv[4]);
    if(argc-1 == 8){
        nbg = atoi(argv[4]);
        tcroisement = atof(argv[5]);
        tmutation = atof(argv[6]);
        taille_pop = atoi(argv[7]);
    }
    cout << "Nombre d'intervenants: " << nb_intervenants << endl;
    cout << "Nombre de missions: " << nb_missions << endl;
    Random::randomize();
    //chromosome *Chromosome = new chromosome(nb_missions, nb_intervenants, missions, intervenants, distances);
    //Chromosome->afficher();
    //Chromosome->evaluer();
    /*Chromosome->hasIntervenantCompetence();
    cout << "nombre de chevauchement : " << Chromosome->hasOnlyOneMissionOrTime() << endl;
    Chromosome->afficher();
    cout << "nb pénalitées : " << Chromosome->countPenalties() << endl;*/
    //population *pop = new population(taille_pop, nb_missions, nb_intervenants, missions, intervenants, distances);
    //pop->statiatiques();
    //int nbg, double tcroisement, double tmutation, int tp, int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distances
   
    clock_t t1, t2, t_max;
    t1 = clock();
    t_max = t1 + CLOCKS_PER_SEC * temps_exec_sec;
    //liste de chromosomes contenant les meilleurs individus générés (on prends les 10 meilleurs de chaque ae)
    Liste<chromosome*> meilleurs;
    //tant que le temps n'est pas écoulé on créer un ae et on l'optimise.
    
    //Ae *ae = new Ae(nbg, tcroisement, tmutation, taille_pop, nb_missions, nb_intervenants, missions, intervenants, distances);
    //ae->optimiser(t_max, &meilleurs);
    
    while(clock() < t_max){
        Ae *ae = new Ae(nbg, tcroisement, tmutation, taille_pop, nb_missions, nb_intervenants, missions, intervenants, distances);
        ae->optimiser(t_max, &meilleurs);
    }

    if(meilleurs.longueur() == 0){
        t2 = clock();
        float time = (float)(t2-t1)/CLOCKS_PER_SEC;
        printf("\n\nExecution time : %fs\n", time);
        cout << "temps de recherche trop court ! " << endl;
        return 0;
    }


    cout << endl << endl;
    //garder les 10% meilleurs sur la fitness 2
    for(int i=0; i<meilleurs.longueur();i++){
        meilleurs[i]->evaluer2();
    }
    int number = meilleurs.longueur()/10;
    if(number == 0)
        number = 1;
    chromosome **meilleurs2 = new chromosome*[number]; //contient les 10% meilleurs sur la fitness 2 et fitness 1
    int iter = 0;
    while(iter < number){
        chromosome *best = meilleurs[0];
        int pos = 0;
        for(int i=0;i<meilleurs.longueur(); i++){
            if(meilleurs[i]->fitness2 < best->fitness2){
                best = meilleurs[i];
                pos = i;
            }
        }
        meilleurs2[iter] = best;
        meilleurs.supprimer(pos);
        iter++;
    }

    double best_fit = INFINITY;
    int pos = 0; //position dans le tableau de l'individu ayant la meilleure fitness 3
    for(int i = 0; i < number; i++){
        meilleurs2[i]->evaluer3();
        if(meilleurs2[i]->fitness3 < best_fit){
            best_fit = meilleurs2[i]->fitness3;
            pos = i;
        }
    }

    cout << "Meilleur chromosome : " << endl;
    meilleurs2[pos]->afficher();

    t2 = clock();

    float time = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("\n\nExecution time : %fs\n", time);

    return 0;
}



