#pragma once
#include <string>
struct Data {
	int dzien=1;
	int miesiac=1;
	int rok=0;
	bool operator==(const Data& other) const {
		return dzien == other.dzien && miesiac == other.miesiac && rok == other.rok;
	}
};

struct Czas {
	int godzina=0;
	int minuty=0;
	bool operator>(const Czas& other) const {
		return (godzina > other.godzina) || (godzina == other.godzina && minuty > other.minuty);
	}
};

enum class OsobaError {
	NoError,
	SameLogin,
	WrongId
};

enum class TreningError {
	NoError,
	TrenerZajety,
	KlientZajety,
	WrongId,
	BrakWaznegoKarnetu
};

enum class KarnetError {
	NoError,
	KarnetJuzPrzypisany
};