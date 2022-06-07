#include "ae.h"

//opérateurs de croisements a faire


using namespace std;

// initialisation des param�tres de l'AG et g�n�ration de la population initiale
Ae::Ae(int nbg, double tcroisement, double tmutation, int tp, int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distances)
{
	nbgenerations     = nbg;
	taille_pop        = tp;
	taux_croisement   = tcroisement;
	taux_mutation     = tmutation;
	this->nb_missions = nb_missions;
	this->nb_intervenants = nb_intervenants;
	this->missions    = missions;
	this->intervenants= intervenants;
	this->distances   = distances;
	pop   = new population(taille_pop, nb_missions, nb_intervenants, missions, intervenants, distances);
}

// destructeur de l'objet Ae
Ae::~Ae()
{
	delete pop;
}

// proc�dure principale de la recherche
chromosome* Ae::optimiser()
{
	int amelioration = 0;
	chromosome *fils1 = new chromosome(nb_missions, nb_intervenants, missions, intervenants, distances);//la
	chromosome *fils2 = new chromosome(nb_missions, nb_intervenants, missions, intervenants, distances);//la
	chromosome *pere1;
	chromosome *pere2;
	int best_fitness;

	// �valuation des individus de la population initiale
	for(int ind=0; ind<taille_pop; ind++)
		pop->individus[ind]->evaluer();

	// on ordonne les indivudus selon leur fitness
	pop->ordonner();

	best_fitness = pop->individus[pop->ordre[0]]->fitness;
	//  on affiche les statistiques de la population initiale
	cout << "Quelques statistiques sur la population initiale" << endl;
	pop->statiatiques();

	return pop->individus[pop->ordre[0]];
	//here

	//tant que le nombre de g�n�rations limite n'est pas atteint
	for(int g=0; g<nbgenerations; g++)
	{
		//s�lection de deux individus de la population courante
		pere1 = pop->selection_roulette();
		pere2 = pop->selection_roulette();

		// On effectue un croisementavec une probabilit� "taux_croisement"
		if(Random::aleatoire(1000)/1000.0 < taux_croisement)
		{
			croisement1X(pere1, pere2, fils1, fils2);
		}
		else
		{
			fils1->copier(pere1);
			fils2->copier(pere2);
		}

		// On effectue la mutation d'un enfant avec une probabilit� "taux_mutation"
		if(Random::aleatoire(1000)/1000.0 < taux_mutation)
			fils1->echange_2_genes_consecutifs();

		// On effectue la mutation de l'autre enfant avec une probabilit� "taux_mutation"
		if(Random::aleatoire(1000)/1000.0 < taux_mutation)
			fils2->echange_2_genes_consecutifs();

		// �valuation des deux nouveaux individus g�n�r�s
		fils1->evaluer();
		fils2->evaluer();

		// Insertion des nouveaux individus dans la population
		pop->remplacement_roulette(fils1);
		pop->remplacement_roulette(fils2);

		// On r�ordonne la population selon la fitness
		pop->reordonner();

		// Si l'un des nouveaux indivudus-solutions est le meilleur jamais rencont�
		if (pop->individus[pop->ordre[0]]->fitness < best_fitness)
		{
			best_fitness = pop->individus[pop->ordre[0]]->fitness;
			cout << "Amelioration de la meilleure solution a la generation " << g << " : " << best_fitness << endl;
			amelioration = g;
		}
	}
	//  on affiche les statistiques de la population finale
	cout << "Quelques statistiques sur la population finale" << endl;
	pop->statiatiques();
	//  on affiche la consanginit� de la population finale
	pop->similitude();

	//retourner le meilleur individu rencontr� pendant la recherche
	return pop->individus[pop->ordre[0]];
}

// op�rateur de croisement � un point : croisement 1X
// 1) l'op�rateur 1X choisit de mani�re al�atoire un point de croisement
// 2) l'op�rateur 1X recopie le d�but du parent 1 au d�but de l'enfant 1
//                     et le d�but du parent 2 au d�but de l'enfant 2.
// 3) l'op�rateur 1X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
//    Le 1ier fils est le produit de la partie haute du premier parent et
//    de la partie basse du deuxi�me parent et inversement pour le 2�me fils
void Ae::croisement1X(chromosome* parent1, chromosome* parent2,
                      chromosome* enfant1, chromosome* enfant2)
{
	int nb_genes = parent1->taille;

	int* odre_parent1 = new int[nb_genes];
	int* odre_parent2 = new int[nb_genes];

	for (int i=0; i<nb_genes; i++)
	{
		odre_parent1[parent1->genes[i]] = i;
		odre_parent2[parent2->genes[i]] = i;
	}

	// 1) l'op�rateur 1X choisit de mani�re al�atoire le point de croisement
	int point = Random::aleatoire(nb_genes);

	// 2) l'op�rateur 1X recopie le d�but du parent 1 au d�but de l'enfant 1
	//                     et le d�but du parent 2 au d�but de l'enfant 2.
	enfant1->copier(parent1);
	enfant2->copier(parent2);

	// 3) l'op�rateur 1X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
	//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
	for (int k=point+1; k<nb_genes; k++)
	{
		for (int l=k+1; l<nb_genes; l++)
		{
			if(odre_parent2[enfant1->genes[k]]>odre_parent2[enfant1->genes[l]])
				enfant1->echange_2_genes(k,l);
			if(odre_parent1[enfant2->genes[k]]>odre_parent1[enfant2->genes[l]])
				enfant2->echange_2_genes(k,l);
		}
	}
	delete[] odre_parent1;
	delete[] odre_parent2;
}

// op�rateur de croisement � deux points : croisement 2X
// 1) l'op�rateur 2X choisit de mani�re al�atoire 2 points de croisement
// 2) l'op�rateur 2X recopie le d�but du parent 1 au d�but de l'enfant 1
//                        et le d�but du parent 2 au d�but de l'enfant 2.
// 3) l'op�rateur 2X compl�te l'enfant 1 avec les g�nes manquant en les pla�ant dans l'ordre du parent 2
//                         et l'enfant 2 avec les g�nes manquant en les pla�ant dans l'ordre du parent 1.
void Ae::croisement2X(chromosome* parent1, chromosome* parent2,
                      chromosome* enfant_s1, chromosome* enfant_s2)
{
}

void Ae::croisement2LOX(chromosome* parent1, chromosome* parent2,
                        chromosome* enfant_s1, chromosome* enfant_s2)
{
}


