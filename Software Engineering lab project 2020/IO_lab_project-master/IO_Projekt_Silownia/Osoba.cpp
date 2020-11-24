#include "Osoba.h"

bool Osoba::zmien_haslo(string nowe_haslo)
{
	this->haslo = nowe_haslo;
	return true;
}

bool Osoba::edytuj_dane(string Imie, string Nazwisko, Data Data_urodzenia)
{
	this->imie = Imie;
	this->nazwisko = Nazwisko;
	this->data_urodzenia = Data_urodzenia;
	return true;
}

void Osoba::Wyswietl()
{
	cout << login << " " << haslo << " " << imie << " " << nazwisko << " " << id << " " << data_urodzenia.dzien << "-" << data_urodzenia.miesiac << "-" << data_urodzenia.rok<< " "<<endl;
}

