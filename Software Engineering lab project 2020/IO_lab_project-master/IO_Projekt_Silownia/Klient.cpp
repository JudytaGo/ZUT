#include "Klient.h"

bool Klient::Dodaj_wydarzenie(Wydarzenie* wydarzenie)
{
	lista_wydarzen.push_back(wydarzenie);
	return true;
}

void Klient::Wyswietl_kalendarz()
{
	cout << "Wydarzenia:" << endl;
	for (int i = 0; i < lista_wydarzen.size(); i++) {
		if(lista_wydarzen[i]!=NULL)
			lista_wydarzen[i]->Wyswietl();
		else {
			lista_wydarzen.erase(lista_wydarzen.begin() + i);
			i--;
		}
	}
}

KarnetError Klient::Dodaj_karnet(Karnet* nowy_karnet)
{
	if (karnet != nullptr) {
		return KarnetError::KarnetJuzPrzypisany;
	}
	nowy_karnet->SetId(this->GetId());
	this->karnet = nowy_karnet;
	return KarnetError::NoError;
}

bool Klient::Sprawdz_status()
{
	if (karnet == nullptr || !karnet->GetStatus())
		return false;
	return true;
	
}

void Klient::Wyswietl()
{
	cout << login << " " << haslo << " " << imie << " " << nazwisko << " " << id << " " << data_urodzenia.dzien << "-" << data_urodzenia.miesiac << "-" << data_urodzenia.rok << " "<< nr_telefonu << " " << adres_email << endl;
}

TreningError Klient::Zapisz_do_GrTrening(GrTrening* grTrening)
{
	if (karnet == nullptr || !karnet->GetStatus()) {
		return TreningError::BrakWaznegoKarnetu;
	}
	if (grTrening != nullptr) {
		if (this->JestDostepny(grTrening->GetData(), grTrening->GetGodzina(), grTrening->GetCzasTrwania())) {
			grTrening->DodajUczestnika(this);
			lista_wydarzen.push_back(grTrening);
			return TreningError::NoError;
		}
	}
	return TreningError::KlientZajety;
}

bool Klient::JestDostepny(Data data, Czas poczatek, Czas dlugoscTrwania)
{
	Czas koniec;
	koniec.godzina = 0;
	koniec.minuty = poczatek.minuty + dlugoscTrwania.minuty;
	if (koniec.minuty >= 60) {
		koniec.godzina = 1;
		koniec.minuty = koniec.minuty % 60;
	}
	koniec.godzina += poczatek.godzina + dlugoscTrwania.godzina;

	for (int i = 0; i < lista_wydarzen.size(); i++) {
		if (lista_wydarzen[i]->GetData() == data) {
			Czas koniecLista;
			koniecLista.godzina = 0;
			koniecLista.minuty = lista_wydarzen[i]->GetGodzina().minuty + lista_wydarzen[i]->GetCzasTrwania().minuty;
			if (koniecLista.minuty >= 60) {
				koniecLista.godzina = 1;
				koniecLista.minuty = koniecLista.minuty % 60;
			}
			koniecLista.godzina += lista_wydarzen[i]->GetGodzina().godzina + lista_wydarzen[i]->GetCzasTrwania().godzina;
			if (!(lista_wydarzen[i]->GetGodzina() > koniec || poczatek > koniecLista)) {
				return false;
			}
		}
	}
	return true;
}

Wydarzenie* Klient::GetWydarzenieWithID(int id)
{
	for (int i = 0; i < lista_wydarzen.size(); i++) {
		if (lista_wydarzen[i]->GetId() == id) {
			return lista_wydarzen[i];
		}
	}
	return nullptr;
}