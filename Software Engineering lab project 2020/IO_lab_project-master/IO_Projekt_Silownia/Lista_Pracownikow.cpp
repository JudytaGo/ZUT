#include "Lista_Pracownikow.h"
#include "Lista_Klientow.h"
Lista_Pracownikow* Lista_Pracownikow::inst_ = NULL;

Lista_Pracownikow* Lista_Pracownikow::getInstance() {
	if (inst_ == NULL) {
		inst_ = new Lista_Pracownikow();
	}
	return(inst_);
}

OsobaError Lista_Pracownikow::Dodaj_Pracownika(Osoba* osoba)
{
	for(int i = 0; i < pracownicy.size(); i++) {
		if (!pracownicy[i]->GetLogin().compare(osoba->GetLogin())) {
			delete osoba;
			return OsobaError::SameLogin;
		}
		if (pracownicy[i]->GetId() == osoba->GetId()) {
			delete osoba;
			return OsobaError::WrongId;
		}
	}
	vector<Klient*> klienci = Lista_Klientow::getInstance()->GetKlienci();
	for (int i = 0; i < klienci.size(); i++) {
		if (!klienci[i]->GetLogin().compare(osoba->GetLogin())) {
			delete osoba;
			return OsobaError::SameLogin;
		}
		if (klienci[i]->GetId() == osoba->GetId()) {
			delete osoba;
			return OsobaError::WrongId;
		}
	}
	pracownicy.push_back(osoba);
	return OsobaError::NoError;
}

void Lista_Pracownikow::Wyswietl()
{
	cout << "Lista pracownikow:" << endl;
	for (int i = 0; i < pracownicy.size(); i++) {
		pracownicy[i]->Wyswietl();
	}
}
