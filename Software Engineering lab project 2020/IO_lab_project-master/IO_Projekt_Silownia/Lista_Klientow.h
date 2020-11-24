#pragma once
#include "Klient.h"
class Lista_Klientow
{
private:
	static Lista_Klientow* inst_;   // The one, single instance
	Lista_Klientow() {};
	Lista_Klientow(const Lista_Klientow&);
	Lista_Klientow& operator=(const Lista_Klientow&);
	vector<Klient*> klienci;
public:
	static Lista_Klientow* getInstance();
	OsobaError Dodaj_Klienta(Klient* klient);
	void Wyswietl();
	vector<Klient*> GetKlienci() { return klienci; };
};

