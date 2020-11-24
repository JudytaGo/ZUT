#pragma once
#include "Wydarzenie.h"
#include <vector>
using namespace std;
class KalendarzG
{
private:
	static KalendarzG* inst_;   // The one, single instance
	KalendarzG() {};
	KalendarzG(const KalendarzG&);
	KalendarzG& operator=(const KalendarzG&);
	vector<Wydarzenie*> wydarzenia;
public:
	static KalendarzG* getInstance();
	bool Dodaj_Wydarzenie(Wydarzenie* wydarzenie);
	void Wyswietl();
	vector<Wydarzenie*> GetWydarzenia() { return wydarzenia; };
};


