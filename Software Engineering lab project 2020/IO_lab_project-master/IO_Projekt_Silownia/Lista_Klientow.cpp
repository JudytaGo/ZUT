#include "Lista_Klientow.h"
#include "Lista_Pracownikow.h"
Lista_Klientow* Lista_Klientow::inst_ = NULL;

Lista_Klientow* Lista_Klientow::getInstance() {
	if (inst_ == NULL) {
		inst_ = new Lista_Klientow();
	}
	return(inst_);
}

OsobaError Lista_Klientow::Dodaj_Klienta(Klient* klient)
{
	for (int i = 0; i < klienci.size(); i++) {
		if (!klienci[i]->GetLogin().compare(klient->GetLogin())) {
			delete klient;
			return OsobaError::SameLogin;
		}
		if (klienci[i]->GetId() == klient->GetId()) {
			delete klient;
			return OsobaError::WrongId;
		}
	}
	vector<Osoba*> pracownicy = Lista_Pracownikow::getInstance()->GetPracownicy();
	for (int i = 0; i < pracownicy.size(); i++) {
		if (!pracownicy[i]->GetLogin().compare(klient->GetLogin())) {
			delete klient;
			return OsobaError::SameLogin;
		}
		if (pracownicy[i]->GetId() == klient->GetId()) {
			delete klient;
			return OsobaError::WrongId;
		}
	}
	klienci.push_back(klient);
	return OsobaError::NoError;
}

void Lista_Klientow::Wyswietl()
{
	cout << "Lista klientow:" << endl;
	for (int i = 0; i < klienci.size(); i++) {
		klienci[i]->Wyswietl();
	}
}
