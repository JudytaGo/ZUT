// IO_Projekt_Silownia.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include "Klient.h"
#include "Trener_Personalny.h"
#include "Pomocnicze.h"
#include "Lista_Klientow.h"
#include "KalendarzG.h"
#include "Pracownik_Administracji.h"
#include "Lista_Pracownikow.h"
#include <iostream>
void PrintResult(OsobaError error) {
	switch (error)
	{
	case OsobaError::NoError:
		cout << "Udalo sie dodac osobe!" << endl;
		break;
	case OsobaError::SameLogin:
		cout << "Login jest juz zajety!" << endl;
		break;
	case OsobaError::WrongId:
		cout << "Id jest juz zajete!" << endl;
		break;
	default:
		break;
	}
	
}
void PrintResult(KarnetError error) {
	switch (error)
	{
	case KarnetError::NoError:
		cout << "Udalo sie dodac karnet!" << endl;
		break;
	case KarnetError::KarnetJuzPrzypisany:
		cout << "Karnet byl juz przypisany!" << endl;
		break;
	default:
		break;
	}

}
void PrintResult(TreningError error) {
	switch (error)
	{
	case TreningError::NoError:
		cout << "Udalo sie dodac trening!" << endl;
		break;
	case TreningError::TrenerZajety:
		cout << "Trener jest juz zajety w tym terminie!" << endl;
		break;
	case TreningError::KlientZajety:
		cout << "Klient jest juz zajety w tym terminie!" << endl;
		break;
	case TreningError::WrongId:
		cout << "Id jest juz zajete!" << endl;
		break;
	case TreningError::BrakWaznegoKarnetu:
		cout << "Brak waznego karnetu!" << endl;
		break;
	default:
		break;
	}

}

int main()
{
	Data data;
	Data data2;
	data2.dzien = 5;
	Czas czas;
	//Tworzymy kilka przykladowych osob. klient2 ma powtorzone id a klient4 ma powtorzony login,wiec powinno sie nie udac ich pozniejsze dodanie
	Pracownik_Administracji* pracownik = new Pracownik_Administracji(string("Janek"), string("123456"), string("Jan"), string("Kowalski"), 0, data);
	Trener_Personalny* trener = new Trener_Personalny(string("Olka"), string("123"), string("Ola"), string("Oliwkiewicz"), 501, data, 666555444);
	Trener_Personalny* trener2 = new Trener_Personalny(string("Jolka"), string("123"), string("Jola"), string("Joliwkiewicz"), 502, data, 666555444);
	Klient* klient = new Klient(string("Adamek"), string("321"), string("Adam"), string("Maj"), data, 2, 33333333, string("test@wp.pl"));
	Klient* klient2 = new Klient(string("Adamek2"), string("3214"), string("Adam2"), string("Maj2"), data, 0, 33333333, string("test2@wp.pl"));
	Klient* klient3 = new Klient(string("Janek2"), string("3215"), string("Adam3"), string("Maj3"), data, 5, 33333333, string("test3@wp.pl"));
	Klient* klient4 = new Klient(string("Janek2"), string("3215"), string("Adam3"), string("Maj3"), data, 12, 33333333, string("test3@wp.pl"));
	
	cout << "Przykladowe wyswietlenie pracownika i trenera:" << endl;
	pracownik->Wyswietl();
	trener->Wyswietl();

	//Dodawanie tych osob
	cout << "Dodawanie po kolei pracownika,2 trenerow i 4 klientow" << endl;
	PrintResult(Lista_Pracownikow::getInstance()->Dodaj_Pracownika(pracownik));
	PrintResult(Lista_Pracownikow::getInstance()->Dodaj_Pracownika(trener));
	PrintResult(Lista_Pracownikow::getInstance()->Dodaj_Pracownika(trener2));
	PrintResult(Lista_Klientow::getInstance()->Dodaj_Klienta(klient));
	PrintResult(Lista_Klientow::getInstance()->Dodaj_Klienta(klient2));//id zajete
	PrintResult(Lista_Klientow::getInstance()->Dodaj_Klienta(klient3));
	PrintResult(Lista_Klientow::getInstance()->Dodaj_Klienta(klient4));//login zajety


	cout << "Tworzenie 3 grupowych wydarzen w tym samym czasie dla jednego trenera, przy czym 2 sa w tym samym czasie:" << endl;
	GrTrening* grTrening =  new GrTrening(data, czas, czas, 3, string("utworzony"), string("opis"), trener);
	GrTrening* grTrening2 = new GrTrening(data, czas, czas, 4, string("utworzony"), string("opis"), trener);
	GrTrening* grTrening3 = new GrTrening(data2, czas, czas, 4, string("utworzony"), string("opis"), trener);
	PrintResult(pracownik->Zaplanuj_GrTrening(grTrening));
	PrintResult(pracownik->Zaplanuj_GrTrening(grTrening2));//termin zajety u trenera
	PrintResult(pracownik->Zaplanuj_GrTrening(grTrening3));

	cout << "Wyswietl kalendarz zajec grupowych:" << endl;
	pracownik->Wyswietl_kalendarz(KalendarzG::getInstance());

	cout << "Sprawdz czy klient ma wazny karnet:" << endl;
	cout <<(klient->Sprawdz_status() ? "Aktywny." : "Nieaktywny." )<< endl;
	PrintResult(pracownik->Dodaj_karnet(5, klient));
	cout << (klient->Sprawdz_status() ? "Aktywny." : "Nieaktywny." ) << endl;

	cout << "Klienci zapisuja sie do treningu grupowego:" << endl;
	PrintResult(klient->Zapisz_do_GrTrening(grTrening));
	PrintResult(klient3->Zapisz_do_GrTrening(grTrening));//brak karnetu

	cout << "Lista uczestnikow tego treningu:" << endl;
	pracownik->Wyswietl_liste_uczestnikow(grTrening);

	cout << "Zaplanuj trening zwyczajny z klientem, ktory w tym czasie ma trening grupowy:" << endl;
	PrintResult(trener2->Zaplanuj_trening(data, czas, czas, string("opis treningu"), klient,7));
	cout << "Wyswietl kalendarz tego klienta:" << endl;
	klient->Wyswietl_kalendarz();

	Lista_Klientow::getInstance()->Wyswietl();

}
