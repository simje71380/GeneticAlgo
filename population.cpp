#include "population.h"

using namespace std;

// initialisation d'une population de solutions
population::population(int tp, int nb_missions, int nb_intervenants, Mission *missions, Intervenant *intervenants, Distance *distance)
{
	taille_pop = tp;
	individus  = new chromosome*[taille_pop];
	for (int i=0; i<taille_pop; i++)
		individus[i] = new chromosome(nb_missions, nb_intervenants, missions, intervenants, distance);
	ordre = new int[taille_pop];
}

// destruction de l'objet "population"
population::~population()
{
	for (int i=0; i<taille_pop; i++)
		delete individus[i];
	delete individus;
	delete ordre;
}

// statistiques sur la population
void population::statiatiques()
{
	double moyenne    = 0;
	double ecart_type = 0;

	for (int i=0; i<taille_pop; i++)
	{
		moyenne += individus[i]->fitness;
		ecart_type += individus[i]->fitness*individus[i]->fitness;
	}
	moyenne = moyenne / taille_pop;
	ecart_type = sqrt(ecart_type/taille_pop - moyenne*moyenne);

	cout << "fitness : (moyenne, ecart_type) -> ("
	     <<  moyenne << " , "  << ecart_type << ")" << endl;
	cout << "fitness : [meilleure, mediane, pire] -> ["
	     << individus[ordre[0]]->fitness << " , "
	     << individus[ordre[(int)(taille_pop/2)]]->fitness << " , "
	     << individus[ordre[taille_pop-1]]->fitness << "]" << endl;
}

// Similitude de la population
void population::similitude()
{
	int nb_ind_id_1, nb_ind_id_2, nb_ind_id_3;
	nb_ind_id_1 = nb_chromosomes_similaires(individus[ordre[0]]);
	cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[0]]->fitness << " : " << nb_ind_id_1 << " / " << taille_pop << endl;
	if (nb_ind_id_1<taille_pop)
	{
		nb_ind_id_2 = nb_chromosomes_similaires(individus[ordre[nb_ind_id_1]]);
		cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[nb_ind_id_1]]->fitness << " : " << nb_ind_id_2 << " / " << taille_pop << endl;
		if (nb_ind_id_1+nb_ind_id_2<taille_pop)
		{
			nb_ind_id_3 = nb_chromosomes_similaires(individus[ordre[nb_ind_id_1+nb_ind_id_2]]);
			cout << "Nombre d'indivudus de la population identique ayant la fitness = " << individus[ordre[nb_ind_id_1+nb_ind_id_2]]->fitness << " : " << nb_ind_id_3 << " / " << taille_pop << endl;
		}
	}
}

// compte le nombre de chromosomes similaires
int population::nb_chromosomes_similaires(chromosome* chro)
{
	int nb = 0;
	for (int i=0; i<taille_pop; i++)
		if (chro->identique(individus[i]))
			nb++;
	return nb;
}

// Oronne les individus de la population par ordre croissant de fitness
void population::ordonner()
{
	int inter;
	for(int i=0; i<taille_pop; i++)
		ordre[i]=i;

	for(int i=0; i<taille_pop-1; i++)
		for(int j=i+1; j<taille_pop; j++)
			if(individus[ordre[i]]->fitness > individus[ordre[j]]->fitness)
			{
				inter    = ordre[i];
				ordre[i] = ordre[j];
				ordre[j] = inter;
			}
}

// R�-ordonne le classement des individus de la population par ordre croissant de fitness
//   apr�s un petit changement
void population::reordonner()
{
	int inter;
	for(int i=0; i<taille_pop-1; i++)
		for(int j=i+1; j<taille_pop; j++)
			if(individus[ordre[i]]->fitness > individus[ordre[j]]->fitness)
			{
				inter    = ordre[i];
				ordre[i] = ordre[j];
				ordre[j] = inter;
			}
}

// SELECTION PAR ROULETTE BIAISEE
//op�rateur de s�lection bas� sur la fonction fitness
chromosome* population::selection_roulette()
{
	int somme_fitness = individus[0]->fitness;
	int fitness_max   = individus[0]->fitness;
	int somme_portion;

	for(int i=1; i<taille_pop; i++)
	{
		somme_fitness += individus[i]->fitness;
		if (fitness_max < individus[i]->fitness)
			fitness_max = individus[i]->fitness;
	}
	somme_portion = fitness_max*taille_pop - somme_fitness;

	double variable_alea = Random::aleatoire(1000)/1000.0;

	int ind = 0;
	double portion = (fitness_max - individus[0]->fitness)*1./somme_portion;
	while ((ind<taille_pop-1) && (variable_alea>=portion))
	{
		ind++;
		portion += (fitness_max - individus[ind]->fitness)*1./somme_portion;
	}
	return individus[ind];
}

// op�rateur de remplacement bas� sur la roulette biais�e d'un individu de la population
//   par un nouveau individu donn� en argument
void population::remplacement_roulette(chromosome* individu)
{
	int somme_fitness = individus[0]->fitness;
	for(int i=1; i<taille_pop; i++)
		somme_fitness += individus[i]->fitness;

	double variable_alea;
	int ind = ordre[0];
	double portion;

	while (ordre[0]==ind)
	{
		variable_alea = Random::aleatoire(1000)/1000.0;
		ind = 0;
		portion = individus[0]->fitness*1./somme_fitness;
		while ((ind<taille_pop-1) && (variable_alea>portion))
		{
			ind++;
			portion += individus[ind]->fitness*1./somme_fitness;
		}
	}
	individus[ind]->copier(individu);
	individus[ind]->fitness = individu->fitness;
}

// SELECTION ALEATOIRE
//op�rateur de s�lection al�atoire
chromosome* population::selection_aleatoire()
{
	int ind_alea = Random::aleatoire(taille_pop);
	return individus[ind_alea];
}

// op�rateur de remplacement al�toire d'un individu de la population
//   par un nouveau individu donn� en argument
void population::remplacement_aleatoire(chromosome* individu)
{
	int ind_alea = Random::aleatoire(taille_pop);
	individus[ind_alea]->copier(individu);
	individus[ind_alea]->fitness = individu->fitness;
}

// SELECTION PAR RANKING
// op�rateur de s�lection bas� sur le ranking, les individus de la
//   population sont ordonn�s de fa�on � ce que le meilleur est le rang 0
//   le taux de ranking permet de r�gler la pr�ssion de s�lection :
//   0 forte pression et +INFINI faible pression (probabilite = 1/nb qlq soit l'individu)
chromosome* population::selection_ranking(float taux_ranking)
{
	reordonner();

	double variable_aleatoire = Random::aleatoire(10000)/10000.0;
	int nb = taille_pop;
	int i = 0;
	taux_ranking = taux_ranking/100;

	double portion = ((double(nb-i)*2.0/double(nb*(nb+1)))+taux_ranking)/(1.0+nb*taux_ranking);

	while(variable_aleatoire>portion)
	{
		i++;
		portion += ((double(nb-i)*2.0/double(nb*(nb+1)))+taux_ranking)/(1.0+nb*taux_ranking);
	}

	if(i>=nb)
		i = Random::aleatoire(nb);
	return individus[ordre[i]];
}

// op�rateur de remplacement bas� sur le ranking d'un individu de la population
//   par un nouveau individu donn� en argument
void population::remplacement_ranking(chromosome*individu, float taux_ranking)
{
	double variable_aleatoire = Random::aleatoire(10000)/10000.0;
	int T = taille_pop;
	int i = 0;
	taux_ranking   = taux_ranking/100;
	double portion = ((double(i)*2.0/double(T*(T-1)))+taux_ranking)/(1.0+T*taux_ranking);

	while(variable_aleatoire>portion)
	{
		i++;
		portion += ((double(i)*2.0/double(T*(T-1)))+taux_ranking)/(1.0+T*taux_ranking);
	}
	if(i>=T)
		i = Random::aleatoire(T);
	int ind = ordre[i];
	individus[ind]->copier(individu);
	individus[ind]->fitness = individu->fitness;
}

// affichage de la population, de son rang et de sa fitness
void population::afficher()
{
	cout << "Poputalion de " << taille_pop << " individus :" << endl;
	for (int i=0; i<taille_pop; i++)
	{
		cout << "individu " << i << ", rang : " << ordre[i] << " ";
		individus[i]->afficher();
	}
}
