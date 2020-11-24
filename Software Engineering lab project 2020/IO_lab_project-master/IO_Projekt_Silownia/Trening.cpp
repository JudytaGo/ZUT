#include "Trening.h"

Trening::Trening(Data Data, Czas Godzina, Czas Czas_trwania, int Id, string Status, string Opis, Klient* Klient, Trener_Personalny* Trener) :
	Wydarzenie(Data, Godzina, Czas_trwania, Id, Status, Opis), klient(Klient), trener(Trener)
{
}

void Trening::Wyswietl()
{
	cout << "Trening " << data.rok <<"."<< data.dzien << "." << data.dzien << " " << godzina.godzina << ":" << godzina.minuty <<" Trener: "<< trener->GetNazwisko() << " Klient: " << klient->GetNazwisko() << endl;
}

void Trening::SetOpis(string Opis)
{
	this->opis = Opis;
}

void Trening::Edytuj(Data data, Czas godzina, Czas czas_trwania)
{
	this->data = data;
	this->godzina = godzina;
	this->czas_trwania = czas_trwania;
}

string Trening::GetOpis()
{
	return opis;
}
