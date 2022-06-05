#include "chromosome.h"

using namespace std;

// initialisation des param�tres d'un chromosome
chromosome::chromosome(int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distances)
{
	this->nb_missions = nb_missions;
	this->nb_intervenants = nb_intervenants;
	this->missions = missions;
	this->intervenants = intervenants;
	this->distances = distances;
	int a;
	bool recommence = true;
	taille          = nb_missions + nb_intervenants - 1;//pas de séparateur pour le dernier
	// un chromosome est compos� de 'taille' g�nes,
	// les g�nes sont carat�ris� par un entier compris entre 0 et 'taille-1'
	// il ne peux avoir 2 fois le m�me g�ne dans un chromosome
	genes = new int[taille];
	int nb_separateurs = nb_intervenants - 1;
	for(int i=0; i<taille; i++)
	{
		float proba = (float)nb_separateurs/(taille-i)*100;
		if(rand()%100 < proba && nb_separateurs > 0){
			genes[i] = -1;
			nb_separateurs--;
		}
		else{
			recommence = true;
			while(recommence)
			{
				recommence = false;
				// on tire al�atoirement le g�ne suivante
				a = Random::aleatoire(nb_missions)+1; //missions numérotées de 1 à nb_missions (+1 car pas de mission avec l'id 0)
				// le g�ne ne doit pas �tre d�j� dans le chromosome
				// si tel est le cas on re-tire al�atoirement le g�ne
				for (int j=0; j<i; j++)
					if (a==genes[j])
						recommence = true;
			}
			genes[i]=a;
		}
		
	}
}

// destruction de l'objet 'chromosome'
chromosome::~chromosome()
{
	delete genes;
}

bool chromosome::hasAllMissionsAffected(){
	int *tab = new int[nb_missions];
	int count_sep = 0;
	for(int i=0; i<nb_missions; i++)
		tab[i] = i+1;
	for(int i=0; i<taille; i++){
		if(genes[i] == -1)
			count_sep++;
		else{
			for(int j=0; j<nb_missions; j++){
				if(genes[i] == tab[j]){
					tab[j] = -1;
					break;
				}
			}
		}
	}
	for(int i=0; i<nb_missions; i++)
		if(tab[i] != -1){
			delete []tab;
			return false;
		}	
	delete []tab;
	if(count_sep != nb_intervenants - 1)
		return false;
	return true;
}


bool chromosome::hasIntervenantCompetence(){
	int id_intervenant = 0;
	for(int i=0; i<taille; i++){
		if(genes[i] == -1){
			id_intervenant++;
			continue;
		}
		else{
			if(intervenants[id_intervenant].GetCompetence() != missions[genes[i]-1].GetCompetence()){
				//cout << "Intervenant " << intervenants[id_intervenant].GetId() << " n'a pas la competence " << missions[genes[i]-1].GetCompetence() << endl;
				return false;
			}
		}
	}
	return true;
}

int chromosome::hasOnlyOneMission(){
	int id_intervenant = 0;
	int **affected_mission = new int*[nb_intervenants];	//tableau contenant les missions affect�es � l'intervant de l'itération
	int *size = new int[nb_intervenants];							//tableau contenant le nombre de missions affect�es � l'intervenant de l'itération
	int count = 0;
	//initialisation du tableau
	for(int i=0; i<taille; i++){
		if(genes[i] == -1){
			affected_mission[id_intervenant] = new int[count];
			size[id_intervenant] = count;
			count = 0;
			id_intervenant++;
			continue;
		}
		count++;
	}
	affected_mission[id_intervenant] = new int[count];
	size[id_intervenant] = count;
	//remplissage du tableau
	count = 0;
	id_intervenant = 0;
	for(int i=0; i<taille; ++i){
		if(genes[i] == -1){
			id_intervenant++;
			count = 0;
			continue;
		}
		else{
			affected_mission[id_intervenant][count] = genes[i];
			count++;
		}
	}
	//v�rification des missions affect�es
	int chevauchement = 0;
	for(int i=0; i<nb_intervenants; i++){
		for(int j=1; j<size[i]; j++){
			for(int k = 0; k<j; k++){
				if(j != k && missions[affected_mission[i][j]].GetJour() == missions[affected_mission[i][k]].GetJour()){
					int deb_1 = missions[affected_mission[i][j]].GetDebutMissionMinute();
					int deb_2 = missions[affected_mission[i][k]].GetDebutMissionMinute();
					int fin_1 = missions[affected_mission[i][j]].GetFinMissionMinute();
					int fin_2 = missions[affected_mission[i][k]].GetFinMissionMinute();
					//chevauchement
					if(deb_1 < deb_2){ //debut de la mission 1 avant debut de la mission 2
						if(fin_1 > deb_2){ //fin de la mission 1 après debut de la mission 2
							//cout << "   1    "<< "Intervenant " << intervenants[i].GetId() << " chevauche les missions " << missions[affected_mission[i][j]].GetId() << " et " << missions[affected_mission[i][k]].GetId() << endl;
							chevauchement++;
						}
						else{ //sinon il faut avoir le temps d'aller de la mission 1 à la mission 2
							float temps_trajet = (float)distances->getDistanceMtoM(missions[affected_mission[i][j]].GetId(), missions[affected_mission[i][k]].GetId())/833.333;
							if(temps_trajet > (float)fin_2 - deb_1){
								//cout << "   2    "<< "Intervenant " << intervenants[i].GetId() << " n'a pas le temps d'aller de " << missions[affected_mission[i][j]].GetId() << " a " << missions[affected_mission[i][k]].GetId() << endl;
								chevauchement++;
							}
						}
					}
					else if(deb_1 > deb_2){ //debut par la mission 2 puis va vers la mission 1
						if(fin_2 > deb_1){ //fin de la mission 2 après debut de la mission 1
							//cout << "   3   "<<"Intervenant " << intervenants[i].GetId() << " chevauche les missions " << missions[affected_mission[i][j]].GetId() << " et " << missions[affected_mission[i][k]].GetId() << endl;
							chevauchement++;
						}
						else{	//sinon il faut avoir le temps d'aller de la mission 2 à la mission 1
							float temps_trajet = (float)distances->getDistanceMtoM(missions[affected_mission[i][k]].GetId(), missions[affected_mission[i][j]].GetId())/833.333;
							if(temps_trajet > (float)fin_1 - deb_2){
								//cout << "   4    "<< "Intervenant " << intervenants[i].GetId() << " n'a pas le temps d'aller de " << missions[affected_mission[i][k]].GetId() << " a " << missions[affected_mission[i][j]].GetId() << endl;
								chevauchement++;
							}
						}
					}
					else{ //deb_1 == deb_2
						//cout << "   5    "<<"Mission " << missions[affected_mission[i][j]].GetId() << " et " << missions[affected_mission[i][k]].GetId() << " sont en chevauchement" << endl;
						chevauchement++;
					}
				}
			}
		}
	}
	//libération de la mémoire
	for(int i=0; i<nb_intervenants; i++){
		delete[] affected_mission[i];
	}
	delete []affected_mission;
	delete []size;
	return chevauchement;
}
// �valuation d'une solution : fonction qui calcule la fitness d'une solution
void chromosome::evaluer()
{
	//fitness des employées
	if(!hasAllMissionsAffected()){
		cout << "Solution non valide" << endl;
	}
	else{
		cout << "Solution valide" << endl;
	}
	fitness = 0;
	
}

// copie les genes d'un chromosome. la fitness n'est reprise
void chromosome::copier(chromosome* source)
{
	for(int i=0; i<taille; i++)
		genes[i] = source->genes[i];
}

// on �change les 2 g�nes
void chromosome::echange_2_genes(int gene1, int gene2)
{
	int inter    = genes[gene1];
	genes[gene1] = genes[gene2];
	genes[gene2] = inter;
}

void chromosome::echange_2_genes_consecutifs()
{
	// on s�l�ctionne un g�ne al�atoirement entre premier et l'avant dernier.
	// Rappel : Random::aleatoire(taille-1) retourne un entier al�atoire compris entre 0 et taille-2
	int i = Random::aleatoire(taille-1);

	// on �change le g�ne s�l�ctionn� al�atoirement avec le g�ne le succ�dant
	echange_2_genes(i, i+1);
}

void chromosome::echange_2_genes_quelconques()
{
}

void chromosome::deplacement_1_gene()
{
}

void chromosome::inversion_sequence_genes()
{
}

// affichage des param�tre d'un chromosome
void chromosome::afficher()
{
	cout << genes[0];
	for(int i=1;i<taille;i++)
		cout << "|" << genes[i];
	//cout << " => fitness = " << fitness << endl;
	cout << endl;
}

bool chromosome::identique(chromosome* chro)
{
	for(int i=1; i<taille; i++)
		if (chro->genes [i] != this->genes[i])
			return false;
	return true;
}
