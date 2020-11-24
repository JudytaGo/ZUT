#include "Karnet.h"

Karnet::Karnet(int Okres)
{
	this->data_zakupu.dzien = 0;
	this->data_zakupu.miesiac = 0;
	this->data_zakupu.rok = 0;
	this->dni_waznosci = Okres;
	this->status = true;
}

bool Karnet::Przedluz(int Nowy_okres)
{
	this->dni_waznosci += Nowy_okres;
	return true;
}
