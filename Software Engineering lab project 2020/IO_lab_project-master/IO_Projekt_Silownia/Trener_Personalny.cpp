#include "Trener_Personalny.h"

TreningError Trener_Personalny::Zaplanuj_trening(Data data, Czas godzina, Czas czas_trwania, string opis, Klient* klient,int id)
{
	string status = "Zaplanowany";
	Trening* nowy_trening = new Trening(data, godzina, czas_trwania, id, status, opis, klient, this);
	if (!klient->JestDostepny(data,godzina,czas_trwania)) {
		delete nowy_trening;
		return TreningError::KlientZajety;
	}
	if (klient->GetKarnet() == nullptr || !klient->GetKarnet()->GetStatus()) {
		delete nowy_trening;
		return TreningError::BrakWaznegoKarnetu;
	}
	if (!this->JestDostepny(data, godzina, czas_trwania)) {
		delete nowy_trening;
		return TreningError::TrenerZajety;
	}
	vector<Wydarzenie*> wydarzenia_klienta = klient->GetWydarzenia();
	for (int i = 0; i < wydarzenia_klienta.size(); i++) {
		if (wydarzenia_klienta[i]->GetId() == id)
			return TreningError::WrongId;
	}
	for (int i = 0; i < lista_wydarzen.size(); i++) {
		if (lista_wydarzen[i]->GetId() == id)
			return TreningError::WrongId;
	}
	klient->Dodaj_wydarzenie(nowy_trening);
	this->Dodaj_wydarzenie(nowy_trening);
	return TreningError::NoError;
}

bool Trener_Personalny::Edytuj_trening(Trening* trening, Data data, Czas godzina, Czas czas_trwania)
{
	trening->Edytuj(data, godzina,czas_trwania);
	return true;
}

bool Trener_Personalny::Usun_trening(Trening* trening)
{
	delete trening;
	trening = NULL;
	return true;
}

bool Trener_Personalny::Dodaj_opis(Trening* trening, string opis)
{
	trening->SetOpis(opis);
	return true;
}

bool Trener_Personalny::Usun_opis(Trening* trening)
{
	trening->SetOpis(string());
	return true;
}

bool Trener_Personalny::Wyswietl_opis(Trening* trening)
{
	cout<<trening->GetOpis()<<endl;
	return true;
}

bool Trener_Personalny::Zmien_status(Trening* trening, string status)
{
	trening->SetStatus(status);
	return true;
}

bool Trener_Personalny::Dodaj_wydarzenie(Wydarzenie* wydarzenie)
{
	lista_wydarzen.push_back(wydarzenie);
	return true;
}

void Trener_Personalny::Wyswietl_kalendarz()
{
	cout << "Wydarzenia:" << endl;
	for (int i = 0; i < lista_wydarzen.size(); i++) {
		if (lista_wydarzen[i] != NULL)
			lista_wydarzen[i]->Wyswietl();
		else {
			lista_wydarzen.erase(lista_wydarzen.begin() + i);
			i--;
		}
	}
}

bool Trener_Personalny::JestDostepny(Data data, Czas poczatek, Czas dlugoscTrwania)
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

Wydarzenie* Trener_Personalny::GetWydarzenieWithID(int id)
{
	for (int i = 0; i < lista_wydarzen.size(); i++) {
		if (lista_wydarzen[i]->GetId() == id) {
			return lista_wydarzen[i];
		}
	}
	return nullptr;
}
