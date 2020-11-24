#pragma once
#include "Osoba.h"
#include "Klient.h"
#include "GrTrening.h"
#include "KalendarzG.h"
class Pracownik_Administracji :
	public Osoba
{
private:

public:
	Pracownik_Administracji(string Login, string Haslo, string Imie, string Nazwisko, int Id, Data Data_urodzenia) :
		Osoba(Login, Haslo, Imie, Nazwisko,Id, Data_urodzenia) {};
	void Wyswietl_kalendarz(KalendarzG* kalendarz);
	KarnetError Dodaj_karnet(int okres, Klient* klient);
	bool Edytuj_Karnet(Karnet* karnet,int nowy_okres);
	TreningError Zaplanuj_GrTrening(GrTrening* grTrening);
	bool Edytuj_GrTrening(GrTrening* grTrening,Data data, Czas czas, Czas czas_trwania, string opis, int id);
	bool Usun_GrTrening(GrTrening* grTrening);
	void Wyswietl_liste_uczestnikow(GrTrening* grTrening);
	void Wyswietl_GrTrening(GrTrening* grTrening);
};

