#pragma once
#include "Pomocnicze.h"
class Karnet
{
private:
	bool status;
	Data data_zakupu;
	int dni_waznosci;
	int id;
public:
	Karnet(int Okres);
	bool GetStatus() { return status; };
	bool Przedluz(int Nowy_okres);
	void SetId(int Id) { id = Id; };
};

