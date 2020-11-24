#pragma once
#include "Wydarzenie.h"
#include "Trener_Personalny.h"
#include "Klient.h"
class Trener_Personalny;
class Klient;
class Trening:
	public Wydarzenie
{
private:
	Klient * klient;
	Trener_Personalny * trener;
public:
	Trening() {};
	Trening(Data Data, Czas Godzina, Czas Czas_trwania, int Id, string Status, string Opis, Klient* Klient, Trener_Personalny * Trener);
	void Wyswietl() override;
	void SetStatus(string Status) { status = Status; };
	string GetOpis();
	void SetOpis(string Opis);
	void Edytuj(Data data, Czas godzina, Czas czas_trwania);
};

