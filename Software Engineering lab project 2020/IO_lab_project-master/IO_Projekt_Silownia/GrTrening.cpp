#include "GrTrening.h"

GrTrening::GrTrening(Data Data, Czas Godzina, Czas Czas_trwania, int Id, string Status, string Opis, Trener_Personalny* Trener):
	Wydarzenie(Data, Godzina, Czas_trwania, Id, Status, Opis), trener(Trener)
{
}

void GrTrening::Wyswietl()
{
	cout << "Trening grupowy " << data.rok << "." << data.dzien << "." << data.dzien << " " << godzina.godzina << ":" << godzina.minuty << " Trener: " << trener->GetNazwisko() << endl;

}

bool GrTrening::DodajUczestnika(Klient* klient)
{
	lista_uczestnikow.push_back(klient);
	return true;
}

void GrTrening::WyswietlUczestnikow()
{
	for (int i = 0; i < lista_uczestnikow.size(); i++) {
		if (lista_uczestnikow[i] != NULL){
			lista_uczestnikow[i]->Wyswietl();
			}
	}
}

bool GrTrening::EdytujTrening(Data data, Czas czas, Czas czas_trwania, string opis, int id)
{
	this->data = data;
	this->godzina = czas;
	this->czas_trwania = czas_trwania;
	this->opis = opis;
	this->id = id;

	return true;
}
