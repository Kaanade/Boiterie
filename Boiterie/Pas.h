
using namespace std;
#include <string>

#pragma once
class Pas
{
	

private :
	double  doubleAppuiRecep, phaseAppui, doubleAppuiProp, phaseOscil, doubleIdeal, phaseIdeal;

public:
	Pas();
	~Pas();
	double demiPasG, demiPasD, distTot;
	double debutCycle, dureeCycle, tempsG, tempsD, debutDeriv, finDeriv;
	double time_a, time_b, time_c, time_d, begin_a, begin_b, begin_c, begin_d;
	double percent_A, percent_B, percent_C, percent_D;
	string first;
	void CalculCycle();


};
