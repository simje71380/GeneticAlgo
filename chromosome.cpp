#include "chromosome.h"
#include <math.h>

//opérateurs de mutation et de croisement
//opérateur de réparation à aussi utiliser sur la génération du chromosome


using namespace std;

// initialisation des param�tres d'un chromosome
chromosome::chromosome(int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distances)
{
	this->nb_missions = nb_missions;
	this->nb_intervenants = nb_intervenants;
	this->missions = missions;
	this->intervenants = intervenants;
	this->distances = distances;
	this->heure_supp = new float[nb_intervenants];
	this->distance_par_intervenant = new double[nb_intervenants];
	this->working_hours_weekly = new double[nb_intervenants];
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
	
	//réparation (sur les compétences)
	repair_comp();

}

// destruction de l'objet 'chromosome'
chromosome::~chromosome()
{
	delete []heure_supp;
	delete []distance_par_intervenant;
	delete []genes;
	delete []working_hours_weekly;
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

void chromosome::repair_comp(){
	struct Incorrect_genes{
		int id_mission;
		int count_available_intervenants; //nombre d'intervenants disponibles pour la mission
		int *available_intervenant_idx; //contient les id des intervenants disponibles pour la mission
	}incorrect_gene;

	int taille = nb_missions + nb_intervenants - 1;
	int id_intervenant = 0;
	int **output = new int*[nb_intervenants];
	int *idx_out = new int[nb_intervenants];

	for(int i=0; i<nb_intervenants; i++){
		output[i] = new int[taille];
		idx_out[i] = 0;
	}
	
	for(int i=0; i<taille; i++){
		if(genes[i] == -1){
			id_intervenant++;
			continue;
		}
		else{
			if(intervenants[id_intervenant].GetCompetence() != missions[genes[i]-1].GetCompetence()){ //pas la bonne compétence
				//cout << "Intervenant " << intervenants[id_intervenant].GetId() << " n'a pas la competence " << missions[genes[i]-1].GetCompetence() << " Pour la mission" << genes[i] <<endl;
				Incorrect_genes incorrect_gene;
				incorrect_gene.id_mission = genes[i];
				incorrect_gene.count_available_intervenants = 0;
				for(int j=0; j<nb_intervenants; j++){
					if(intervenants[j].GetCompetence() == missions[genes[i]-1].GetCompetence()){
						incorrect_gene.count_available_intervenants++;
					}
				}
				if(incorrect_gene.count_available_intervenants == 0){ //normalement impossible
					cout << "Erreur repair_comp : aucun intervenant disponible pour la mission " << genes[i] << endl;
				}
				incorrect_gene.available_intervenant_idx = new int[incorrect_gene.count_available_intervenants];
				int idx = 0;
				for(int j=0; j<nb_intervenants; j++){
					if(intervenants[j].GetCompetence() == missions[genes[i]-1].GetCompetence()){
						incorrect_gene.available_intervenant_idx[idx] = j;
						//cout << "Intervenant " << intervenants[j].GetId() << " a la bonne competence " << intervenants[j].GetCompetence() << endl;
						idx++;
					}
				}
				//affection aléatoire à un intervenant ayant la bonne compétence
				int intervenant_real_idx = incorrect_gene.available_intervenant_idx[rand()%incorrect_gene.count_available_intervenants];
				output[intervenant_real_idx][idx_out[intervenant_real_idx]] = genes[i];
				//cout << "Intervenant " << intervenants[intervenant_real_idx].GetId() << " a été affecté à la mission " << genes[i] << endl;
				//affichage de l'affectation
				idx_out[intervenant_real_idx]++;
				//libération mémoire
				delete []incorrect_gene.available_intervenant_idx;
			}
			else{//bonne compétence
				output[id_intervenant][idx_out[id_intervenant]] = genes[i];
				idx_out[id_intervenant]++;
			}
		}
	}

	//copie des résultats dans les genes
	int idx = 0;
	for(int i=0; i<nb_intervenants; i++){
		//cout << "Intervenant : " << intervenants[i].GetId() << endl;
		for(int j=0; j<idx_out[i];j++){
			//cout << output[i][j] << " ";
			genes[idx] = output[i][j];
			idx++;
		}
		//séparateur sauf pour le dernier intervenant
		if(i != nb_intervenants-1){
			//cout << endl << "separateur" << endl;
			genes[idx] = -1;
			idx++;
		}	
	}
	//libération mémoire
	for(int i=0; i<nb_intervenants; i++){
		delete []output[i];
	}
	delete []output;
	delete []idx_out;

	//test de la correction
	if(!hasAllMissionsAffected()){
		cout << "Erreur : les missions n'ont pas été affectées" << endl;
	}
	if(!hasIntervenantCompetence()){
		cout << "Erreur repair_comp : les intervenants n'ont pas la bonne compétence" << endl;
	}
}


int chromosome::hasOnlyOneMissionOrTime(){
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
			affected_mission[id_intervenant][count] = genes[i]-1;//0 à 44 ald de 1 à 45
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
							cout << "   1    "<< "Intervenant " << intervenants[i].GetId() << " chevauche les missions " << missions[affected_mission[i][j]].GetId() << " et " << missions[affected_mission[i][k]].GetId() << endl;
							chevauchement++;
						}
						else{ //sinon il faut avoir le temps d'aller de la mission 1 à la mission 2
							float temps_trajet = (float)distances->getDistanceMtoM(missions[affected_mission[i][j]].GetId(), missions[affected_mission[i][k]].GetId())/833.333;
							if(temps_trajet > (float)fin_2 - deb_1){
								cout << "   2    "<< "Intervenant " << intervenants[i].GetId() << " n'a pas le temps d'aller de " << missions[affected_mission[i][j]].GetId() << " a " << missions[affected_mission[i][k]].GetId() << endl;
								chevauchement++;
							}
						}
					}
					else if(deb_1 > deb_2){ //debut par la mission 2 puis va vers la mission 1
						if(fin_2 > deb_1){ //fin de la mission 2 après debut de la mission 1
							cout << "   3   "<<"Intervenant " << intervenants[i].GetId() << " chevauche les missions " << missions[affected_mission[i][j]].GetId() << " et " << missions[affected_mission[i][k]].GetId() << endl;
							chevauchement++;
						}
						else{	//sinon il faut avoir le temps d'aller de la mission 2 à la mission 1
							float temps_trajet = (float)distances->getDistanceMtoM(missions[affected_mission[i][k]].GetId(), missions[affected_mission[i][j]].GetId())/833.333;
							if(temps_trajet > (float)fin_1 - deb_2){
								cout << "   4    "<< "Intervenant " << intervenants[i].GetId() << " n'a pas le temps d'aller de " << missions[affected_mission[i][k]].GetId() << " a " << missions[affected_mission[i][j]].GetId() << endl;
								chevauchement++;
							}
						}
					}
					else{ //deb_1 == deb_2
						cout << "   5    "<<"Mission " << missions[affected_mission[i][j]].GetId() << " et " << missions[affected_mission[i][k]].GetId() << " sont en chevauchement" << endl;
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
	/*//fitness des employées
	if(!hasAllMissionsAffected()){
		cout << "Solution non valide" << endl;
	}
	else{
		cout << "Solution valide" << endl;
	}*/

/*
	double moyenne    = 0;
	double ecart_type = 0;

	for (int i=0; i<taille_pop; i++)
	{
		moyenne += individus[i]->fitness;
		ecart_type += individus[i]->fitness*individus[i]->fitness;
	}
	moyenne = moyenne / taille_pop;
	ecart_type = sqrt(ecart_type/taille_pop - moyenne*moyenne);*/



	fitness = 0;
	distance_per_intervenant();
	double penalties = countPenalties();
	double moyenne = 0.0;
	double moyenne2 = 0.0;
	double ecart_type = 0.0;
	double ecart_type2 = 0.0;
	double ecart_type3 = 0.0;
	double moyenne_quota = 0.0;
	double moyenne3 = 0.0;
	for(int i=0; i<nb_intervenants; i++){
		//cout << "Intervenant " << intervenants[i].GetId() << " : distance parcourue = " << distance_par_intervenant[i]/1000 << " km" << endl;
		moyenne += distance_par_intervenant[i];
		moyenne2 += heure_supp[i];
		moyenne_quota += intervenants[i].GetTpsHebdo();
		ecart_type += distance_par_intervenant[i]*distance_par_intervenant[i];
		ecart_type2 += heure_supp[i]*heure_supp[i];
		ecart_type3 += (working_hours_weekly[i]-intervenants[i].GetTpsHebdo())*(working_hours_weekly[i]-intervenants[i].GetTpsHebdo());
		moyenne3 += working_hours_weekly[i]-intervenants[i].GetTpsHebdo();
	}
	
	moyenne /= nb_intervenants;
	moyenne2 /= nb_intervenants;
	moyenne_quota /= nb_intervenants;
	moyenne3 /= nb_intervenants;

	ecart_type = sqrt(ecart_type/nb_intervenants - moyenne*moyenne);
	ecart_type2 = sqrt(ecart_type2/nb_intervenants - moyenne2*moyenne2);
	ecart_type3 = sqrt(ecart_type3/nb_intervenants - moyenne3*moyenne3);
	/*
	cout << "Moyenne distances = " << moyenne/1000 << " km" << endl;
	cout << "Moyenne heures supplémentaires = " << moyenne2 << " heures" << endl;
	cout << "Moyenne du quota des heures hebdomadaire = " << moyenne3 << " heures" << endl;
*/
	double ecart_type_D = ecart_type;
	double ecart_type_OH = ecart_type2;
	double ecart_type_WH = ecart_type3;
	/*cout << "Ecart type distances = " << ecart_type_D/1000 << " km" << endl;
	cout << "Ecart type heures supplémentaires = " << ecart_type_OH << " heures" << endl;
	cout << "Ecart type wasting hours = " << ecart_type_WH << " heures" << endl;
*/
	double gamma, zeta, kappa = 0.0;
	gamma = 100/10.0; //100 / nb total des heures supp toléré (10)
	zeta = 100/moyenne_quota;


    moyenne = 0;
    // somme toutes missions de la distance centre à m + m à centre / nombre intervenants
    for(int i = 1; i < nb_missions+1; i++){
        moyenne += distances->getDistanceSESSADtoM(i);
		moyenne += distances->getDistanceMtoSESSAD(i);
    }
    moyenne /= nb_intervenants;
	kappa = 100 / moyenne;//moyenne de toutes les distance
	fitness = (zeta*ecart_type_WH + gamma * ecart_type_OH + kappa * ecart_type_D)/3;
	fitness += penalties;
	cout << endl << "Fitness : " << fitness << endl;
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
	cout << endl << endl;

	int id_intervenant = 0;
	int **affected_mission = new int*[nb_intervenants];	//tableau contenant les missions_id affect�es � l'intervant
	int *size = new int[nb_intervenants];				//tableau contenant le nombre de missions affect�es par intervenant
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

	//affichage des missions affect�es
	/*for(int i=0; i<nb_intervenants; i++){
		cout << "Intervenant " << i+1 << " : " << endl;
		if(size[i] == 0){
			cout << "Aucune mission affectées" << endl;
			continue;
		}
		int *out_id = new int[size[i]];
		OrdonnerMissions(affected_mission[i], out_id, size[i]);
		cout << out_id[0];
		for(int j=1; j<size[i]; j++){
			cout << " " << out_id[j];
		}
		cout << endl;
	}*/
}

bool chromosome::identique(chromosome* chro)
{
	for(int i=1; i<taille; i++)
		if (chro->genes [i] != this->genes[i])
			return false;
	return true;
}

void chromosome::distance_per_intervenant(){
	int id_intervenant = 0;
	int **affected_mission = new int*[nb_intervenants];	//tableau contenant les missions_id affect�es � l'intervant
	int *size = new int[nb_intervenants];				//tableau contenant le nombre de missions affect�es par intervenant
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


	//calcul de la distance
	for(int i=0; i<nb_intervenants; i++){
		if(size[i] == 0){
			distance_par_intervenant[i] = 0.0;
			continue;
		}
		double distance = 0.0;
		int *out_id = new int[size[i]];
		OrdonnerMissions(affected_mission[i], out_id, size[i]);
		int day = missions[out_id[0]-1].GetJour();
		//cout << "Intervenant " << i+1 << " : " << endl;
		distance += distances->getDistanceSESSADtoM(out_id[0]); //sessad à mission 1
		//cout << "Déplacement du SESSAD à la mission " << out_id[0] << " : " << distances->getDistanceSESSADtoM(out_id[0]) << endl;
		for(int j=1; j<size[i]; j++){
			if(missions[out_id[j]-1].GetJour() == day){ // meme jour donc mission - 1 à celle la
				distance += distances->getDistanceMtoM(out_id[j-1], out_id[j]);
				//cout << "Déplacement de la mission " << out_id[j-1] << " à la mission " << out_id[j] << " : " << distances->getDistanceMtoM(out_id[j-1], out_id[j]) << endl;
			}
			else{//autre jour donc mission - 1 à sessad et sessad à mission courante
				//cout << "Déplacement de la mission " << out_id[j-1] << " à la SESSAD : " << distances->getDistanceMtoSESSAD(out_id[j-1]) << endl;
				distance += distances->getDistanceMtoSESSAD(out_id[j-1]);
				//cout << "Déplacement de la SESSAD à la mission " << out_id[j] << " : " << distances->getDistanceSESSADtoM(out_id[j]) << endl;
				distance += distances->getDistanceSESSADtoM(out_id[j]);
				day = missions[out_id[j]-1].GetJour();
			}
		}
		//fermez la boucle
		//cout << "Déplacement de la mission " << out_id[size[i]-1] << " à la SESSAD : " << distances->getDistanceMtoSESSAD(out_id[size[i]-1]) << endl;
		distance += distances->getDistanceMtoSESSAD(out_id[size[i]-1]);

		//MAJ de la distance
		distance_par_intervenant[i] = distance;
	}
}


double chromosome::countPenalties(){
	int id_intervenant = 0;
	int **affected_mission = new int*[nb_intervenants];	//tableau contenant les missions_id affect�es � l'intervant
	int *size = new int[nb_intervenants];				//tableau contenant le nombre de missions affect�es par intervenant
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
	int counter_penalties = 0;
	for(int i = 0; i < nb_intervenants; i++){
		int* out = new int[size[i]]; //contient l'ID de la mission et pas sa position dans le tableau missions[]
		OrdonnerMissions(affected_mission[i], out, size[i]);
		
		//temps de travail de la 1ère mission		
		if(size[i] == 0){
			heure_supp[i] = 0;
			working_hours_weekly[i] = 0;
			//cout << endl << endl << "Intervenant " << i+1 << " nb de missions affectées : " << size[i] << endl << endl;
			continue;
		}
		int working_minutes_daily = 0;
		working_minutes_daily += (int)missions[out[0]-1].GetFinMissionMinute() - (int)missions[out[0]-1].GetDebutMissionMinute();
		working_minutes_daily += distances->getDistanceSESSADtoM(out[0])/833.333;//on compte le temps de trajet vers la mission
		int working_minutes_weekly = working_minutes_daily;
		int day = missions[out[0]-1].GetJour();
		int start_time = missions[out[0]-1].GetDebutMissionMinute();
		int heure_sup_weekly = 0;
		//cout << "Intervenant " << i+1 << " : " << " jour : " << day << " heure de début : " << start_time << " Mission : " << out[0] << endl;
		for(int j = 1; j < size[i]; j++){
			bool deja_penalise = false;
			if(missions[out[j-1]-1].GetJour() == missions[out[j]-1].GetJour()){ //missions le meme jour
				if(missions[out[j-1]-1].GetFinMissionMinute() > missions[out[j]-1].GetDebutMissionMinute()){ //fin de la premi�re mission > debut de la seconde
					counter_penalties += coef_pena_chevauchement_missions;
					deja_penalise = true;//on ne repénalise pas pour le temps de déplacement (devenu impossible)
					//cout << "Intervenant : " << i+1 <<  " Mission " << out[j-1] << " et " << out[j] << " sont en chevauchement" << endl;
				}
				else{
					//temps de trajet
					float temps_trajet = (float)distances->getDistanceMtoM(out[j-1], out[j])/833.333;
					if(temps_trajet > (float)missions[out[j]-1].GetDebutMissionMinute() - (float)missions[out[j-1]-1].GetFinMissionMinute()){
						//cout << "Intervenant : " << i+1 <<  " Temps de trajet entre " << out[j-1] << " et " << out[j] << " : " << temps_trajet << endl;
						//cout << "Intervenant : " << i+1 <<  " temps entre missions" << (float)missions[out[j]-1].GetDebutMissionMinute() - (float)missions[out[j-1]-1].GetFinMissionMinute() << endl;
						counter_penalties += coef_pena_temps_deplacement;
						deja_penalise = true;
						//cout << "Intervenant : " << i+1 <<" Mission " << out[j-1] << " et " << out[j] << " pas le temps trajet" << endl;
					}
				}
				if(missions[out[j-1]-1].GetFinMissionMinute() >= 720 && !deja_penalise){ //mission précédente fini à/apres midi -> il faut un delta d'au moins 1h avant le debut de la mission courrante
					int delta = (int)missions[out[j]-1].GetDebutMissionMinute() - (int)missions[out[j-1]-1].GetFinMissionMinute(); //temps de trajet non compté
					if(delta < 60){
						counter_penalties += coef_pena_pause_midi;
						deja_penalise = true;
						//cout << "Intervenant : " << i+1 <<  " Mission " << out[j-1] << " et " << out[j] << " pas de pause le midi assez longue ("<< delta << ")" << endl;
					}
				}
			}
			//calcul des heures de travail sur la journée
			if(missions[out[j]-1].GetJour() == day){
				working_minutes_daily += (int)missions[out[j]-1].GetFinMissionMinute() - (int)missions[out[j]-1].GetDebutMissionMinute();
				working_minutes_weekly += (int)missions[out[j]-1].GetFinMissionMinute() - (int)missions[out[j]-1].GetDebutMissionMinute();
				//ajout du temps de trajet
				working_minutes_daily += (int)distances->getDistanceMtoM(out[j-1], out[j])/833.333;
				working_minutes_weekly += (int)distances->getDistanceMtoM(out[j-1], out[j])/833.333;
			}
			else{ //changement de journée
				//compute
				//ajout temps retour vers sessad :
				working_minutes_daily += distances->getDistanceMtoSESSAD(out[j-1])/833.333;
				working_minutes_weekly += distances->getDistanceMtoSESSAD(out[j-1])/833.333;
				//contient le temps maximum journalier selon s'il est a temps partiel ou non
				int h_max_daily = 0;
				if(intervenants[i].GetTpsHebdo() == 35){//temps plein
					h_max_daily = 480;
				}
				else if(intervenants[i].GetTpsHebdo() == 24){//temps partiel
					h_max_daily = 360;
				}
				else{
					cout << "Erreur : tps hebdo non défini" << endl;
					exit(1);
				}
				if(working_minutes_daily > h_max_daily){ //ne doit pas dépasser 8h
					counter_penalties += coef_pena_temps_travail_journalier;
					//cout << "Intervenant " << i+1 <<  " jour " << day << " trop de temps de travail ("<< working_minutes_daily << ")" << endl;
					if(working_minutes_daily > h_max_daily+120){ // + de 2h supp sur la journée
						counter_penalties += coef_pena_heure_supp;
						//cout << "Intervenant " << i+1 <<  " jour " << day << " trop de temps de travail avec heure supp ("<< working_minutes_daily << ")" << endl;
					}
					heure_sup_weekly += working_minutes_daily - h_max_daily;
				}
				int end_time = missions[out[j-1]-1].GetFinMissionMinute();
				//cout << "Intervenant " << i+1 << " : " << " jour : " << day << " heure de fin : " << end_time << " Mission : " << out[j-1] << endl;
				if(end_time - start_time > 720){ // amplitude de 12h max
					counter_penalties += coef_pena_amplitude;
					//cout << "Intervenant " << i+1 <<  " jour " << day << " amplitude trop grande ("<< end_time - start_time << ")" << endl;
				}
				day = missions[out[j]-1].GetJour();
				working_minutes_daily = (double)missions[out[j]-1].GetFinMissionMinute() - (double)missions[out[j]-1].GetDebutMissionMinute();//on compte cette mission
				working_minutes_daily += distances->getDistanceSESSADtoM(out[j])/833.333;//on compte le temps de trajet vers la mission
				working_minutes_weekly += (double)missions[out[j]-1].GetFinMissionMinute() - (double)missions[out[j]-1].GetDebutMissionMinute();
				working_minutes_weekly += distances->getDistanceSESSADtoM(out[j])/833.333;
				start_time = missions[out[j-1]-1].GetDebutMissionMinute();
				//cout << "Intervenant " << i+1 << " : " << " jour : " << day << " heure de début : " << start_time << " Mission : " << out[j] << endl;
			}
		}
		//calcul des pénalitées sur le dernier jour
		int h_max_daily = 0;
		if(intervenants[i].GetTpsHebdo() == 35){//temps plein
			h_max_daily = 480;
		}
		else if(intervenants[i].GetTpsHebdo() == 24){//temps partiel
			h_max_daily = 360;
		}
		else{
			cout << "Erreur : tps hebdo non défini" << endl;
			exit(1);
		}
		if(working_minutes_daily > h_max_daily){ //ne doit pas dépasser 8h
			counter_penalties += coef_pena_temps_travail_journalier;
			//cout << "Intervenant " << i+1 <<  " jour " << day << " trop de temps de travail ("<< working_minutes_daily << ")" << endl;
			if(working_minutes_daily > h_max_daily+120){ // + de 2h supp sur la journée
				counter_penalties += coef_pena_heure_supp;
				//cout << "Intervenant " << i+1 <<  " jour " << day << " trop de temps de travail avec heure supp ("<< working_minutes_daily << ")" << endl;
			}
			heure_sup_weekly += working_minutes_daily - h_max_daily;
		}
		//pénalitée des heures supp weekly
		if(heure_sup_weekly > 600){ //10h supp / semaine max
			counter_penalties += coef_pena_heure_supp;
			//cout << "Intervenant " << i+1 <<  "trop d'heures supplémentaires ("<< heure_sup_weekly << ")" << endl;
		}

		//ajout des heures supp par intervenant dans le tableau pour calcul de la fitness
		heure_supp[i] = heure_sup_weekly/60.0;
		working_hours_weekly[i] = working_minutes_weekly/60.0;
		int end_time = missions[out[size[i]-1]-1].GetFinMissionMinute();
		//cout << "Intervenant " << i+1 << " : " << " jour : " << day << " heure de fin : " << end_time << endl;
		if(end_time - start_time > 720){ // amplitude de 12h max
			counter_penalties += coef_pena_amplitude;
			//cout << "Intervenant " << i+1 <<  " jour " << day << " amplitude trop grande ("<< end_time - start_time << ")" << endl;
		}
	}

	//verif mission affectée qu'un fois normalement impossible
	int *memo = new int[taille];
	int idx = 0;
	for(int i=0; i<taille; i++){
		if(genes[i] != -1){
			for(int j=0; j<idx; j++){
				if(genes[i] == memo[j]){
					counter_penalties += coef_pena_mission_affecter_plusieurs_fois;
					cout <<  "Mission " << genes[i] << " est affectée 2 fois !" << endl;
					continue;
				}
			}
		}
		memo[idx] = genes[i];
		idx++;
	}
	delete [] memo;
	return counter_penalties;
}


int* chromosome::OrdonnerMissions(int *missionsid, int *out, int size){
	struct Id_Time{
		int Mid;
		int date;
	}id_time;
	struct Id_Time *id_time_tab = new struct Id_Time[size];
	for(int i=0; i<size; i++){
		id_time.date = missions[missionsid[i]-1].GetJour()*24*60 + missions[missionsid[i]-1].GetDebutMissionMinute(); //-1 car missions[0] = mission 1
		id_time.Mid = missionsid[i];
		id_time_tab[i] = id_time;
	}
	for(int i=0; i<size; i++){
		//trouver le minimum date et mettre à -1
		int min = 100000;
		int min_id = -1;
		for(int j=0; j<size; j++){
			if(id_time_tab[j].date < min){
				min = id_time_tab[j].date;
				min_id = j;
			}
		}
		out[i] = id_time_tab[min_id].Mid;
		id_time_tab[min_id].date = 100000;
	}
	delete []id_time_tab;
	return out;
}