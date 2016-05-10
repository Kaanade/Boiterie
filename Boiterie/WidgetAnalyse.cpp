#include "WidgetAnalyse.h"

WidgetAnalyse::WidgetAnalyse(QWidget *parent) : QWidget(parent) {

	ankle_left = 14;
	ankle_right = 18;
}


void WidgetAnalyse::setDataSkeleton(vector<vector<Vector4>> & vec_s, vector<double> & vec_t) {
	kinectDevice = KINECT1;
	vec_skeletons = vec_s;
	vec_times = vec_t;
	vec_pas.clear();
}

void WidgetAnalyse::setDataSkeleton(vector<vector<Joint>> & vec_s, vector<double> & vec_t) {
	kinectDevice = KINECT2;
	vec_skeletons2 = vec_s;
	vec_times = vec_t;
	vec_pas.clear();
}



void WidgetAnalyse::longueurPas()
{

	if (kinectDevice == KINECT1)
	{
		for (uint i = 0; i < vec_skeletons.size(); i++) 
		{
			vector<Vector4>  askeleton = vec_skeletons.at(i);
			Vector4 right = askeleton.at(ankle_right);
			Vector4 left = askeleton.at(ankle_left);

			vec_norme.push_back((double)sqrt(pow((right.x - left.x),2) + pow((right.y - left.y),2) + pow((right.z - left.z),2)));
		}
	}
	else
	{
		for (uint i = 0; i < vec_skeletons2.size(); i++) 
		{
			vector<Joint> & askeleton = vec_skeletons2.at(i);
			Joint right = askeleton.at(ankle_right);
			Joint left = askeleton.at(ankle_left);

			vec_norme.push_back(sqrt(pow((right.Position.X - left.Position.X), 2) + pow((right.Position.Y - left.Position.Y), 2) + pow((right.Position.Z - left.Position.Z), 2)));
		}
	}
	
}

void WidgetAnalyse::demiPas()
{
	bool dansPas = false, dansDeb = false;
	bool dansHaut = false, dansBas = false;
	int nbDemiPas = 0,pos_max,pos_min;
	double min, max, temps,maxTotal;
	string pied;
	vector<pair<double,int>> vec_norme_pos;
	Pas pas;
	double moyenne,seuil_haut,seuil_bas;

	
	moyenne = CalculMoyenne(vec_norme);
	seuil_haut = moyenne + moyenne *0.1;
	seuil_bas = moyenne - moyenne *0.1;
	
	int a = 0;

	for (uint i = 0; i < vec_norme.size(); i++)
	{
		if (dansHaut == true)
		{
			vec_norme_pos.push_back(make_pair(vec_norme.at(i), i));
		}
			

		if (vec_norme.at(i) > seuil_haut && dansHaut == false)
		{
			dansHaut = true;
			dansBas = false;
		}
		else if (vec_norme.at(i) < seuil_bas && dansHaut == true)
		{
			nbDemiPas++;
			if (dansHaut == true)
			{
				max = max_element2(vec_norme_pos).first;
				pos_max = max_element2(vec_norme_pos).second;
				pied = quelPied(pos_max);
				if (pied == "gauche")
				{
					pas.demiPasG = max;
					pas.tempsG = temps;
					if (pas.demiPasD == 0)
						pas.first = "gauche";
				}
				else
				{
					pas.demiPasD = max;
					pas.tempsD = temps;
					if (pas.demiPasG == 0)
						pas.first = "droit";
				}
			}
			vec_norme_pos.clear();
			if (nbDemiPas == 2)
			{
				pas.distTot = pas.demiPasD + pas.demiPasG;
				vec_pas.push_back(pas);
				Pas * pas = new Pas();
				nbDemiPas = 0;
			}
			dansHaut = false;
			dansBas = true;
		}
	}

	/*for (uint i = 0; i < vec_norme.size(); i++)
	{
		double test = vec_norme.at(i);
		double test2 = vec_times.at(i);
		if (vec_norme.at(i) < double(0.25) && dansPas == false)
		{
			dansPas = true;
			dansDeb = true;
			pas.debutCycle = vec_times.at(i) / 1000;
		}

		if ( (vec_norme.at(i) > double(0.25)) && (dansDeb == true) )
			dansDeb = false;

		else if (vec_norme.at(i) < double(0.25) && (dansPas == true) && (dansDeb == false))
		{
			nbDemiPas++;
			min = min_element(vec_norme_pos).first;
			pos_min = min_element(vec_norme_pos).second;
			max = max_element2(vec_norme_pos).first;
			pos_max = max_element2(vec_norme_pos).second;
			pied = quelPied(pos_max);
			temps = (vec_times.at(i) - vec_times.at(pos_min)) /1000;

			if (pied == "gauche")
			{
				pas.demiPasG = max;
				pas.tempsG = temps;
				if (pas.demiPasD == 0)
					pas.first = "gauche";
			}
			else
			{
				pas.demiPasD = max;
				pas.tempsD = temps;
				if (pas.demiPasG == 0)
					pas.first = "droit";
			}
				
			
			vec_norme_pos.clear();
			dansDeb = true;

			if (nbDemiPas == 2)
			{
				pas.dureeCycle = pas.tempsD + pas.tempsG;
				pas.distTot = pas.demiPasD + pas.demiPasG;
				vec_pas.push_back(pas);
				Pas * pas = new Pas();
				nbDemiPas = 0;
				
			}
			

		}

		if (dansPas == true)
		{
			vec_norme_pos.push_back(make_pair(vec_norme.at(i), i));
		}
		
		a = 0;
		
	}*/

	a = 0;

}

void WidgetAnalyse::derivee()
{
	//Calcul de la dérivée en m/s
	for (uint i = 0; i < vec_norme.size() -1; i++)
	{
		vec_derivee.push_back( abs( (vec_norme.at(i + 1) - vec_norme.at(i)) / ((vec_times.at(i + 1) - vec_times.at(i)) / 1000)));
	}

	//Pour éviter le bruit on va lisser la courbe de la dérivée
	vec_smooth.push_back( (vec_derivee.at(0) + vec_derivee.at(1)) / 2);

	for (uint i = 1; i < vec_derivee.size() - 2; i++)
	{
		vec_smooth.push_back( (vec_derivee.at(i - 1) + vec_derivee.at(i) + vec_derivee.at(i + 1)) / 3);
	}

	vec_smooth.push_back( (vec_derivee.at(vec_derivee.size() - 2) + vec_derivee.at(vec_derivee.size() - 1)) / 2);

	int a = 0;
}

void WidgetAnalyse::phases()
{
	//Ici 0 pas vaut 1. (Du aux vectors)
	int nb_pas = 0;
	int pass_seuil = 0;
	//Quand on rentre dans une phase A ou C on utilise dansSeuil, pour éviter les passages de seuils non désiré on utilise dansSeuil2
	bool dansSeuil = false, dansSeuil2 = false, commence = false, commence2 =false;
	double test;
	Pas & pas = vec_pas.at(nb_pas);
	Phase new_phase;
	Phase old_phase;
	new_phase.time = 0;
	new_phase.phase = 'z';
	new_phase.nb_pas = 1000;
	vec_phases.push_back(new_phase);

	for (uint i = 1; i < vec_smooth.size(); i++)
	{
		double test = vec_smooth.at(i);
		int test2 = vec_times.at(i);
		//&& pass_seuil % 2 == 0
		if (vec_smooth.at(i) > 0.8 && commence2 == false)
		{
			commence2 = true;
		}
			
		if (i == 88)
			int a = 0;

		old_phase = vec_phases.at(i - 1);
		if ( vec_smooth.at(i) < pas.distTot && commence == true && dansSeuil2 == false )
		{
			pass_seuil++;
			dansSeuil2 = true;
		}
		else if ( vec_smooth.at(i) > pas.distTot && dansSeuil2 == true)
		{
			dansSeuil2 = false;
		}

		//Si la dérivée est en dessous de la longueur du pas et qu'on a pas encore commencé
		if ( vec_smooth.at(i) < vec_norme.at(i) && old_phase.phase == 'z' && dansSeuil == false && vec_norme.at(i) > vec_norme.at(i - 1)  && vec_smooth.at(i) < vec_smooth.at(i-1) && commence2 == true)
		{
			dansSeuil = true;
			new_phase.nb_pas = nb_pas;
			new_phase.phase = 'a';
			new_phase.time = vec_times.at(i) / 1000;
			vec_phases.push_back(new_phase);
		}
		// On rentre dans la phase C
		else if (vec_smooth.at(i) < vec_norme.at(i) && old_phase.phase == 'b' && dansSeuil == false && vec_norme.at(i) > vec_norme.at(i - 1))
		{
			dansSeuil = true;
			new_phase.nb_pas = nb_pas;
			new_phase.phase = 'c';
			new_phase.time = vec_times.at(i) / 1000;
			vec_phases.push_back(new_phase);
		}
		// On rentre dans la phase A d'un nouveau pas
		else if (vec_smooth.at(i) < vec_norme.at(i) && old_phase.phase == 'd' && dansSeuil == false)
		{
			nb_pas++;
			new_phase.nb_pas = nb_pas;
			new_phase.phase = 'a';
			new_phase.time = vec_times.at(i) / 1000;
			vec_phases.push_back(new_phase);
			if (nb_pas >= vec_pas.size())
				break;
			pas = vec_pas.at(nb_pas);
			dansSeuil = true;

		}
		// On rentre dans la phase B
		else if (vec_smooth.at(i) > vec_norme.at(i) && old_phase.phase == 'a' && dansSeuil == true)
		{
			// On test si on a commencé le premier pas
			if (commence == false)
				commence = true;
			dansSeuil = false;
			new_phase.nb_pas = nb_pas;
			new_phase.phase = 'b';
			new_phase.time = vec_times.at(i) / 1000;
			vec_phases.push_back(new_phase);
		}
		// On rentre dans la phase D
		else if (vec_smooth.at(i) > vec_norme.at(i) && old_phase.phase == 'c' && dansSeuil == true)
		{
			dansSeuil = false;
			new_phase.nb_pas = nb_pas;
			new_phase.phase = 'd';
			new_phase.time = vec_times.at(i) / 1000;
			vec_phases.push_back(new_phase);
		}
		// On ne change pas de phase
		else
		{
			new_phase.nb_pas = nb_pas;
			new_phase.phase = old_phase.phase;
			new_phase.time = vec_times.at(i) / 1000;
			vec_phases.push_back(new_phase);
		}
		int a = 0;

	}


	nb_pas = 0;
	pas = vec_pas.at(nb_pas);

	for (uint i = 1; i < vec_phases.size(); i++)
	{
		double begin;
		new_phase = vec_phases.at(i);
		old_phase = vec_phases.at(i - 1);

		if (old_phase.phase != new_phase.phase)
		{
			//On reflechit par rapport à l'ancienne phase
			if (old_phase.phase == 'z')
			{
				begin = new_phase.time;
				pas.debutDeriv = new_phase.time;
				pas.begin_a = new_phase.time;
			}
			else if (old_phase.phase == 'a')
			{
				begin = new_phase.time;
				pas.time_a = new_phase.time - pas.begin_a;
				pas.begin_b = new_phase.time;
			}
			else if (old_phase.phase == 'b')
			{
				begin = new_phase.time;
				pas.time_b = new_phase.time - pas.begin_b;
				pas.begin_c = new_phase.time;
			}
			else if (old_phase.phase == 'c')
			{
				begin = new_phase.time;
				pas.time_c = new_phase.time - pas.begin_c;
				pas.begin_d = new_phase.time;

			}
			else if (old_phase.phase == 'd')
			{
				begin = new_phase.time;
				pas.time_d = new_phase.time - pas.begin_d;
				pas.finDeriv = new_phase.time;
				nb_pas++;
				if (nb_pas >= vec_pas.size())
					break;
				pas = vec_pas.at(nb_pas);
				pas.begin_a = new_phase.time;
			}

		}
	}

	int a = 0;

}


void WidgetAnalyse::calculCycles()
{

	Pas & pas = vec_pas.at(0);
	pas.CalculCycle();
	for (uint i = 1; i < vec_pas.size(); i++)
	{
		pas = vec_pas.at(i);
		pas.CalculCycle();
	}

	int a = 0;
}


pair<double, int> WidgetAnalyse::max_element2(vector<pair<double, int>> vec_norme_pos)
{
	pair<double, int> p;
	double max = -1000.00;
	int max_i = 0;

	for (uint i = 0; i < vec_norme_pos.size(); i++)
	{
		if (vec_norme_pos.at(i).first > max)
		{
			max = vec_norme_pos.at(i).first;
			max_i = vec_norme_pos.at(i).second;
		}
			
	}

	p = make_pair(max, max_i);

	return p;
}

pair<double, int> WidgetAnalyse::min_element(vector<pair<double, int>> vec_norme_pos)
{
	pair<double, int> p;

	double min = 1000.00;
	int min_i = 0;

	for (uint i = 0; i < vec_norme_pos.size(); i++)
	{
		if (vec_norme_pos.at(i).first < min)
		{
			min = vec_norme_pos.at(i).first;
			min_i = vec_norme_pos.at(i).second;
		}
	}

	p = make_pair(min, min_i);

	return p;
}

string WidgetAnalyse::quelPied(int pos)
{
	if (kinectDevice == KINECT1)
	{
		
			vector<Vector4>  askeleton = vec_skeletons.at(pos);
			Vector4 right = askeleton.at(ankle_right);
			Vector4 left = askeleton.at(ankle_left);

			if (right.z > left.z)
				return "gauche";
			else
				return "droite";
	}
	else
	{
			vector<Joint> & askeleton = vec_skeletons2.at(pos);
			Joint right = askeleton.at(ankle_right);
			Joint left = askeleton.at(ankle_left);

			if (right.Position.Z > left.Position.Z)
				return "gauche";
			else
				return "droite";
	}


}

double WidgetAnalyse::CalculMoyenne(vector<double> vec_norme)
{
	double moyenne, somme;

	for (uint i = 0; i < vec_norme.size(); i++)
	{
		somme += vec_norme.at(i);
	}

	moyenne = somme / vec_norme.size();

	return moyenne;
}

vector<Pas> WidgetAnalyse::getVecPas()
{
	return this->vec_pas;
}

void WidgetAnalyse::reset()
{
	vec_pas.clear();
	vec_norme.clear();
	vec_derivee.clear();
	vec_smooth.clear();
	vec_norme_pos.clear();
	vec_skeletons.clear();
	vec_skeletons2.clear();
	vec_times.clear();
	vec_phases.clear();
}

