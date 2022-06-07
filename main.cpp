#include <math.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "chromosome.h"
#include "population.h"


using namespace std;


Intervenant *intervenants;
Mission *missions;
Distance *distances;
int nb_intervenants;
int nb_missions;
int taille_pop = 10;

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
    if(argc-1 != 3){
        std::cout << "Intervenant <fichier.csv>\nMission <fichier.csv>\nDistances <fichier.csv>\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    getData(argv[1], argv[2], argv[3]);
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
    population *pop = new population(taille_pop, nb_missions, nb_intervenants, missions, intervenants, distances);
    pop->statiatiques();
    return 0;
}



