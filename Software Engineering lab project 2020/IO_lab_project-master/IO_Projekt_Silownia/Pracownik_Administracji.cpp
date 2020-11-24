#include "Pracownik_Administracji.h"

void Pracownik_Administracji::Wyswietl_kalendarz(KalendarzG* kalendarz)
{
	kalendarz->Wyswietl();
}

KarnetError Pracownik_Administracji::Dodaj_karnet(int okres, Klient* klient)
{
	Karnet* nowy_karnet = new Karnet(okres);
	return klient->Dodaj_karnet(nowy_karnet);
}

bool Pracownik_Administracji::Edytuj_Karnet(Karnet* karnet, int nowy_okres)
{
	karnet->Przedluz(nowy_okres);
	return true;
}

TreningError Pracownik_Administracji::Zaplanuj_GrTrening(GrTrening* grTrening)
{
	if ( !grTrening->GetTrener()->JestDostepny(grTrening->GetData(),grTrening->GetGodzina(),grTrening->GetCzasTrwania()) ) {
		delete grTrening;
		return TreningError::TrenerZajety;
	}
	vector<Wydarzenie*> wydarzenia = KalendarzG::getInstance()->GetWydarzenia();
	for (int i = 0; i < wydarzenia.size(); i++) {
		if (wydarzenia[i]->GetId() == grTrening->GetId()) {
			delete grTrening;
			return TreningError::WrongId;
		}
	}
	vector<Wydarzenie*> wydarzenia_trenera = grTrening->GetTrener()->GetWydarzenia();
	for (int i = 0; i < wydarzenia_trenera.size(); i++) {
		if (wydarzenia_trenera[i]->GetId() == grTrening->GetId()) {
			delete grTrening;
			return TreningError::WrongId;
		}
	}
	grTrening->GetTrener()->Dodaj_wydarzenie(grTrening);
	KalendarzG::getInstance()->Dodaj_Wydarzenie(grTrening);
	return TreningError::NoError;
}

bool Pracownik_Administracji::Edytuj_GrTrening(GrTrening* grTrening, Data data, Czas czas, Czas czas_trwania, string opis, int id)
{
	grTrening->EdytujTrening(data, czas, czas_trwania, opis, id);
	return true;
}

bool Pracownik_Administracji::Usun_GrTrening(GrTrening* grTrening)
{
	delete grTrening;
	grTrening = NULL;
	return false;
}

void Pracownik_Administracji::Wyswietl_liste_uczestnikow(GrTrening* grTrening)
{
	grTrening->WyswietlUczestnikow();
}

void Pracownik_Administracji::Wyswietl_GrTrening(GrTrening* grTrening)
{
	grTrening->Wyswietl();
}
