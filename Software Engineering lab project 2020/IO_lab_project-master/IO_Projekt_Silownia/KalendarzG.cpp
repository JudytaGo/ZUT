#include "KalendarzG.h"

KalendarzG* KalendarzG::inst_ = NULL;

KalendarzG* KalendarzG::getInstance() {
	if (inst_ == NULL) {
		inst_ = new KalendarzG();
	}
	return(inst_);
}

bool KalendarzG::Dodaj_Wydarzenie(Wydarzenie* wydarzenie)
{
	wydarzenia.push_back(wydarzenie);
	return true;
}

void KalendarzG::Wyswietl()
{
	for (int i = 0; i < wydarzenia.size(); i++) {
		if (wydarzenia[i] != NULL) {
			wydarzenia[i]->Wyswietl();
		}
		else {
			wydarzenia.erase(wydarzenia.begin() + i);
			i--;
		}
	}
}
