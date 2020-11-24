#pragma once
#include "Pomocnicze.h"
using namespace std;
class Wydarzenie
{
	friend class Trening;
	friend class GrTrening;
private:
	Data data;
	Czas godzina;
	Czas czas_trwania;
	int	id;
	string status;
	string opis;
public:
	Wydarzenie() {};
	Wydarzenie(Data Data, Czas Godzina, Czas Czas_trwania, int Id, string Status, string Opis) :
		data(Data), godzina(Godzina), czas_trwania(Czas_trwania), id(Id), status(Status), opis(Opis) {};
	virtual void Wyswietl() = 0;
	Data GetData() { return data; };
	Czas GetGodzina() { return godzina; };
	Czas GetCzasTrwania() { return czas_trwania; };
	int GetId() { return id; };
};

