#pragma once
#include "Wydarzenie.h"
#include "KalendarzG.h"
#include "Trener_Personalny.h"
class Klient;
class Trener_Personalny;
class GrTrening :
	public Wydarzenie
{
private:
	vector< Klient * > lista_uczestnikow;
	Trener_Personalny* trener;
public:
	GrTrening() {};
	GrTrening(Data Data, Czas Godzina, Czas Czas_trwania, int Id, string Status, string Opis, Trener_Personalny* Trener);
	void Wyswietl() override;
	bool DodajUczestnika(Klient * klient);
	void WyswietlUczestnikow();
	bool EdytujTrening(Data data, Czas czas, Czas czas_trwania, string opis, int id);
	Trener_Personalny* GetTrener() { return trener; };
};

