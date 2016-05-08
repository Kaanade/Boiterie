#include "Pas.h"


Pas::Pas()
{
	demiPasD = 0;
	demiPasG = 0;
	dureeCycle = 0;
}


Pas::~Pas()
{
}

void Pas::CalculCycle()
{
	// Cycle Ideal
	this->doubleIdeal = this->dureeCycle * 0.1;
	this->phaseIdeal = this->dureeCycle * 0.4;

	// Cycle Réel
	this->percent_A = 100 * time_a / (finDeriv - debutDeriv);
	this->percent_B = 100 * time_b / (finDeriv - debutDeriv);
	this->percent_C = 100 * time_c / (finDeriv - debutDeriv);
	this->percent_D = 100 * time_d / (finDeriv - debutDeriv);
}