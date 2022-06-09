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

	int nb_competence_differentes = 1;
	string *comp = new string[nb_intervenants];
	comp[0] = intervenants[0].GetCompetence();
	int idx=1;
	for(int i=1; i < nb_intervenants; i++){
		bool deja_vu = false;
		for(int j=0; j<idx; j++){
			if(comp[j] == intervenants[i].GetCompetence()){
				deja_vu = true;
				break;
			}
		}
		if(!deja_vu){
			comp[idx] = intervenants[i].GetCompetence();
			nb_competence_differentes++;
			idx++;
		}
	}
	this->competences = new string[nb_competence_differentes];
	for(int i=0; i<nb_competence_differentes; i++){
		this->competences[i] = comp[i];
	}
	delete [] comp;
	this->nb_competences = nb_competence_differentes;
}

// destructeur de l'objet Ae
Ae::~Ae()
{
	delete pop;
}

// proc�dure principale de la recherche
chromosome* Ae::optimiser(clock_t t_max, Liste<chromosome*> *liste)
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
	bool broke = false;
	//tant que le nombre de g�n�rations limite n'est pas atteint
	for(int g=0; g<nbgenerations; g++)
	{
		//s�lection de deux individus de la population courante
		pere1 = pop->selection_roulette();
		pere2 = pop->selection_roulette();

		// On effectue un croisementavec une probabilit� "taux_croisement"
		if(Random::aleatoire(1000)/1000.0 < taux_croisement)
		{
			//croisement1X(pere1, pere2, fils1, fils2);
			croisement_competence(pere1, pere2, fils1, fils2);
		}
		else
		{
			fils1->copier(pere1);
			fils2->copier(pere2);
		}

		// On effectue la mutation d'un enfant avec une probabilit� "taux_mutation"
		if(Random::aleatoire(1000)/1000.0 < taux_mutation)
			if(Random::aleatoire(1000)/1000.0 < 0.5){
				fils1->echange_2_genes_quelconques();
			}
			else{
				fils1->echange_2_genes_entre_intervenant_meme_competences();
			}
		    
			//fils1->echange_2_genes_consecutifs();

		// On effectue la mutation de l'autre enfant avec une probabilit� "taux_mutation"
		if(Random::aleatoire(1000)/1000.0 < taux_mutation)
			if(Random::aleatoire(1000)/1000.0 < 0.5){
				fils2->echange_2_genes_quelconques();
			}
			else{
				fils2->echange_2_genes_entre_intervenant_meme_competences();
			}
			//fils2->echange_2_genes_consecutifs();

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
		clock_t t = clock();
		if(t > t_max){
			broke = true;
			break;
		}	
			
	}
	//  on affiche les statistiques de la population finale
	if(!broke){
		cout << "Quelques statistiques sur la population finale" << endl;
		pop->statiatiques();
		//  on affiche la consanginit� de la population finale
		pop->similitude();
		pop->individus[pop->ordre[0]]->countPenalties_show_infos();
		pop->individus[pop->ordre[0]]->afficher();
		//ajouter les 10% meilleures a la liste
		for(int i=0; i<taille_pop*0.1; i++){
			liste->ajouter(pop->individus[pop->ordre[i]],0);
		}
	}
	
	//retourner le meilleur individu rencontr� pendant la recherche


	
	
	return pop->individus[pop->ordre[0]];
}


//effectue un croisement par compétence:
//enfant 1 : on prend du parent 1 les missions des intervenants ayant la compétence LSF et du parent2 les missions des intervenants ayant la compétence LPC
//enfant 2 : on prend le reste du parent 1 et du parent 2
void Ae::croisement_competence(chromosome *pere1, chromosome *pere2, chromosome *fils1, chromosome *fils2){
	//tirage au sort de la compétence a swapper
	int comp = Random::aleatoire(nb_competences);
	//on swappe les compétences
	int taille = nb_missions + nb_intervenants - 1;
	//cout << "On swappe sur la compétence " << competences[comp] << endl;
	int idx_fils1 = 0;
	int idx_fils2 = 0;
	int idx_pere1 = 0;
	int idx_pere2 = 0;
	//construction des fils
	for(int i=0; i<nb_intervenants; i++){
		if(intervenants[i].GetCompetence() == competences[comp]){
			//fils 2 prend les missions du parent 1
			while(pere1->genes[idx_pere1] != -1 && idx_pere1 < taille){
				//cout << "fils2 prend la mission " << pere1->genes[idx_pere1] << " du parent 1" << endl;
				fils2->genes[idx_fils2] = pere1->genes[idx_pere1];
				idx_fils2++;
				idx_pere1++;
			}
			//fils 1 prend les missions du parent 2
			while(pere2->genes[idx_pere2] != -1 && idx_pere2 < taille){
				fils1->genes[idx_fils1] = pere2->genes[idx_pere2];
				idx_fils1++;
				idx_pere2++;
			}
			if(idx_fils2 < taille){
				/*cout << "fils2 prend la mission " << pere1->genes[idx_pere1] << " du parent 1" << endl;
				cout << "idx_pere1 = " << idx_pere1 << endl;
				cout << "idx_fil2 = " << idx_fils2 << endl;
				cout << "idx_pere2 = " << idx_pere2 << endl;*/
				fils2->genes[idx_fils2] = pere1->genes[idx_pere1];
				fils1->genes[idx_fils1] = pere2->genes[idx_pere2];
				idx_fils2++;
				idx_fils1++;
				idx_pere1++;
				idx_pere2++;
			}
		}
		else{//recopie
			while(pere2->genes[idx_pere2] != -1 && idx_pere2 < taille){
				//cout << "fils2 prend la mission " << pere2->genes[idx_pere2] << " du parent 2" << endl;
				fils2->genes[idx_fils2] = pere2->genes[idx_pere2];
				idx_fils2++;
				idx_pere2++;
			}
			while(pere1->genes[idx_pere1] != -1 && idx_pere1 < taille){
				fils1->genes[idx_fils1] = pere1->genes[idx_pere1];
				idx_fils1++;
				idx_pere1++;
			}
			if(idx_fils2 < taille){
				/*cout << "fils2 prend la mission " << pere2->genes[idx_pere2] << " du parent 2" << endl;
				cout << "idx_pere1 = " << idx_pere1 << endl;
				cout << "idx_fil2 = " << idx_fils2 << endl;
				cout << "idx_pere2 = " << idx_pere2 << endl;*/
				fils2->genes[idx_fils2] = pere2->genes[idx_pere2];
				fils1->genes[idx_fils1] = pere1->genes[idx_pere1];
				idx_fils1++;
				idx_fils2++;
				idx_pere1++;
				idx_pere2++;
			}
		}
	}
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


