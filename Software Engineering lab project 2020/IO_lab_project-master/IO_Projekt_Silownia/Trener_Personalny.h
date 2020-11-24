#pragma once
#include "Klient.h"
#include "Trening.h"
class Trening;
class Trener_Personalny :
	public Osoba
{
private:
	int przepracowane_godziny=0;
	int nr_telefonu;
	vector<Wydarzenie*> lista_wydarzen;
public:
	Trener_Personalny(string Login, string Haslo, string Imie, string Nazwisko, int Id, Data Data_urodzenia, int nr_tel) :
		Osoba(Login, Haslo, Imie, Nazwisko, Id, Data_urodzenia), nr_telefonu(nr_tel) {};
	TreningError Zaplanuj_trening(Data data, Czas godzina, Czas czas_trwania,string opis, Klient* klient,int id);
	bool Edytuj_trening(Trening* trening, Data data, Czas godzina, Czas czas_trwania);
	bool Usun_trening(Trening* trening);
	bool Dodaj_opis(Trening* trening, string opis);
	bool Usun_opis(Trening* trening);
	bool Wyswietl_opis(Trening* trening);
	bool Zmien_status(Trening* trening, string status);
	bool Dodaj_wydarzenie(Wydarzenie* wydarzenie);
	void Wyswietl_kalendarz();
	bool JestDostepny(Data data, Czas poczatek, Czas dlugoscTrwania);
	vector<Wydarzenie*> GetWydarzenia() { return lista_wydarzen; };
	Wydarzenie* GetWydarzenieWithID(int id);
};

