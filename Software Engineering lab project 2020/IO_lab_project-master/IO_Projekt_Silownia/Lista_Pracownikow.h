#pragma once
#include "Osoba.h"
#include <vector>
class Lista_Pracownikow
{
private:
	static Lista_Pracownikow* inst_;   // The one, single instance
	Lista_Pracownikow() {};
	Lista_Pracownikow(const Lista_Pracownikow&);
	Lista_Pracownikow& operator=(const Lista_Pracownikow&);
	vector<Osoba*> pracownicy;
public:
	static Lista_Pracownikow* getInstance();
	OsobaError Dodaj_Pracownika(Osoba* osoba);
	void Wyswietl();
	vector<Osoba*> GetPracownicy() { return pracownicy; };
};

