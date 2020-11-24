#define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "gnuplot-iostream.h"

using namespace std;
#define PI 3.14159265
int E = 4, D = 1, C = 5, B = 9, A = 4;
double N = 2.0, A1 = 1.0, A2 = 2.0, Tb = 1.0, f = 200.0;
double fs = 2;
double fi = B;
double Amp = 1.0;
int q = 16;
struct Zespolona {
	double rzeczywista;
	double urojona;
	Zespolona(double a, double b) {
		rzeczywista = a;
		urojona = b;
	}
};
class Sygnal {
public:
	vector<double> y;
	vector<double> x;
	double ymin;
	double ymax;
	Sygnal operator*(Sygnal const& prawy) const {
		Sygnal nowy;
		nowy.ymin = 0;
		nowy.ymax = 0;
		nowy.x = this->x;
		for (int i = 0; i < this->x.size(); i++) {
			nowy.y.push_back(this->y[i] * prawy.y[i]);
			if (nowy.ymin > nowy.y[i])
				nowy.ymin = nowy.y[i];
			if (nowy.ymax < nowy.y[i])
				nowy.ymax = nowy.y[i];
		}
		return nowy;
	}
	Sygnal operator+(Sygnal const& prawy) const {
		Sygnal nowy;
		nowy.ymin = 0;
		nowy.ymax = 0;
		nowy.x = this->x;
		for (int i = 0; i < this->x.size(); i++) {
			nowy.y.push_back(this->y[i] + prawy.y[i]);
			if (nowy.ymin > nowy.y[i])
				nowy.ymin = nowy.y[i];
			if (nowy.ymax < nowy.y[i])
				nowy.ymax = nowy.y[i];
		}
		return nowy;
	}
	Sygnal operator-(Sygnal const& prawy) const {
		Sygnal nowy;
		nowy.ymin = 0;
		nowy.ymax = 0;
		nowy.x = this->x;
		for (int i = 0; i < this->x.size(); i++) {
			nowy.y.push_back(this->y[i] - prawy.y[i]);
			if (nowy.ymin > nowy.y[i])
				nowy.ymin = nowy.y[i];
			if (nowy.ymax < nowy.y[i])
				nowy.ymax = nowy.y[i];
		}
		return nowy;
	}
};
void rysuj(string nazwa, double xmin, double xmax, double ymin, double ymax, vector<double> x, vector<double> y, bool histogram = false) {
	Gnuplot gp;
	gp << "set xrange [" << xmin << ":" << xmax << "]\nset yrange [" << ymin - (ymax - ymin) * 0.1 << ":" << ymax + (ymax - ymin) * 0.1 << "]\n";
	if (histogram) {
		gp << "plot '-' with impulses title '" << nazwa
			<< "'\n";
	}
	else {
		gp << "plot '-' with lines linestyle 1 title '" << nazwa
			<< "'\n";
	}
	gp.send1d(make_pair(x, y));
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
}

void rysuj_amplitudowy(string nazwa, int N, double fs, vector<double> widmo) {
	double ymin = 0;
	double ymax = 0;
	vector<double> x;
	for (int i = 0; i < widmo.size(); i++) {

		widmo[i] = 10 * log10(widmo[i]);
		x.push_back(i * fs / N);
		if (widmo[i] < ymin)
			ymin = widmo[i];
		if (widmo[i] > ymax)
			ymax = widmo[i];
	}
	rysuj(nazwa, 0, x[N - 1], ymin, ymax, x, widmo, true);
}

vector<Zespolona> DFT(vector<double> sygnal) {
	vector<Zespolona> dft;
	for (int i = 0; i < sygnal.size(); i++) {
		double rzeczywista = 0.0;
		double urojona = 0.0;

		for (int j = 0; j < sygnal.size(); j++) {
			rzeczywista += sygnal[j] * cos(-2 * PI * i * j / sygnal.size());
			urojona += sygnal[j] * sin(-2 * PI * i * j / sygnal.size());
		}
		dft.push_back(Zespolona(rzeczywista, urojona));
	}
	return dft;
}

vector<double> oblicz_widmo(vector<Zespolona> dft) {
	vector<double> wynik;
	for (int i = 0; i < dft.size(); i++) {
		wynik.push_back(sqrt(dft[i].rzeczywista * dft[i].rzeczywista + dft[i].urojona * dft[i].urojona));
	}
	return wynik;
}


vector<double> S2BS(string in, bool bigEndian)
{
	vector<double> wynik;
	if (bigEndian) {
		for (int i = 0; i < in.size(); i++)
		{
			int val = int(in[i]);
			vector<double> b;
			for (int j = 0; j < 8; j++)
			{
				(val % 2) ? b.push_back(1) : b.push_back(0);
				val /= 2;
			}
			reverse(b.begin(), b.end());
			wynik.insert(wynik.end(), b.begin(), b.end());
		}
	}
	else {
		for (int i = in.size() - 1; i >= 0; i--)
		{
			int val = int(in[i]);
			vector<double> b;
			for (int j = 0; j < 8; j++)
			{
				(val % 2) ? b.push_back(1) : b.push_back(0);
				val /= 2;
			}
			reverse(b.begin(), b.end());
			wynik.insert(wynik.end(), b.begin(), b.end());
		}
	}
	return wynik;
}
Sygnal	WyliczCLK(int numBits,double fCLK) {
	Sygnal out;
	int value = 1;
	double help = 0;
	for (int i = 0; i < numBits * f/fCLK; i++) {
		if (help >= f/fCLK/2) {
			value = (value + 1) % 2;
			help = 0;
		}
		out.x.push_back(i / f);
		out.y.push_back(value);
		help++;
		}
	out.ymin = 0;
	out.ymax = 1;

	return out;
}

Sygnal WyliczTTL(vector<double> y, Sygnal sygCLK) {
	bool CLKup = false;
	double value=0;
	Sygnal out;
	out.x = sygCLK.x;
	out.ymin = 0;
	out.ymax = 1;
	int j=0;
	for (int i = 0; i < sygCLK.x.size(); i++) {
		if (!CLKup && sygCLK.y[i] == 1) {
			value = y[j];
			j++;
			CLKup = true;
		}
		if (CLKup && sygCLK.y[i] == 0) {
			CLKup = false;
		}
		out.y.push_back(value);
	}
	return out;
}
Sygnal WyliczBAMI(vector<double> y, Sygnal sygCLK) {
	bool CLKup = false;
	double value = 0;
	bool plus = true;
	Sygnal out;
	out.x = sygCLK.x;
	out.ymin = -1;
	out.ymax = 1;
	int j = 0;
	for (int i = 0; i < sygCLK.x.size(); i++) {
		if (!CLKup && sygCLK.y[i] == 1) {
			value = y[j]*(plus ? 1 : -1);
			if (y[j] == 1) {
				plus = !plus;
			}
			j++;
			CLKup = true;
		}
		if (CLKup && sygCLK.y[i] == 0) {
			CLKup = false;
		}
		out.y.push_back(value);
	}
	return out;
}
Sygnal WyliczNRZI(vector<double> y, Sygnal sygCLK) {
	bool CLKup = true;
	double value = 0;
	bool plus = true;
	Sygnal out;
	out.x = sygCLK.x;
	out.ymin = -1;
	out.ymax = 1;
	int j = 0;
	for (int i = 0; i < sygCLK.x.size(); i++) {
		if (CLKup && sygCLK.y[i] == 0) {
			if (y[j] == 1) {
				value = plus ? 1 : -1;
				plus = !plus;
			}
			j++;
			CLKup = false;
		}
		if (!CLKup && sygCLK.y[i] == 1) {
			CLKup = true;
		}
		out.y.push_back(value);
	}
	return out;
}
Sygnal WyliczManchester(vector<double> y, Sygnal sygCLK) {
	bool CLKup = true;
	double value = 0;
	Sygnal out;
	out.x = sygCLK.x;
	out.ymin = -1;
	out.ymax = 1;
	int j = 0;
	for (int i = 0; i < sygCLK.x.size(); i++) {
		if (CLKup && sygCLK.y[i] == 0) {
			value = y[j] ? -1 : 1;
			j++;
			CLKup = false;
		}
		if (!CLKup && sygCLK.y[i] == 1) {
			if (y[j] == y[j - 1]) {
				value = -value;
			}
			CLKup = true;
		}
		out.y.push_back(value);
	}
	return out;
}

vector<double> DecodeBAMI(Sygnal in, Sygnal sygCLK) {
	bool CLKup = false;
	double value = 0;
	vector<double> out;
	int j = 0;
	for (int i = 0; i < sygCLK.x.size(); i++) {
		if (CLKup && sygCLK.y[i] == 0) {
			CLKup = false;
		}
		if (!CLKup && sygCLK.y[i] == 1) {
			if (abs(in.y[i]) == 1)
				out.push_back(1);
			else
				out.push_back(0);
			CLKup = true;
		}
	}
	return out;
}

vector<double> DecodeNRZI(Sygnal in, Sygnal sygCLK) {
	bool CLKup = true;
	double value = 0;
	vector<double> out;
	int j = 0;
	for (int i = 0; i < sygCLK.x.size(); i++) {
		if (CLKup && sygCLK.y[i] == 0) {
			if (in.y[i] == value)
				out.push_back(0);
			else
				out.push_back(1);
			CLKup = false;
			value = in.y[i];
		}
		if (!CLKup && sygCLK.y[i] == 1) {
			CLKup = true;
		}
	}
	return out;
}

vector<double> DecodeManchester(Sygnal in, Sygnal sygCLK) {
	bool CLKup = true;
	double value = 0;
	vector<double> out;
	int j = 0;
	for (int i = 0; i < sygCLK.x.size(); i++) {
		if (CLKup && sygCLK.y[i] == 0) {
			if(in.y[i]==1)
				out.push_back(0);
			else
				out.push_back(1);
			CLKup = false;
		}
		if (!CLKup && sygCLK.y[i] == 1) {
			CLKup = true;
		}
	}
	return out;
}
int main() {
	//Słowo ABC
	string a = "AB";
	vector<double> out = S2BS(a, true);
	for (vector<double>::iterator it = out.begin(); it != out.end(); ++it) {
		cout << *it;
	}
	cout << endl;
	Sygnal CLK = WyliczCLK(16, 10.0);
	rysuj("CLK", 0, CLK.x.size()/f, CLK.ymin, CLK.ymax, CLK.x, CLK.y);

	Sygnal TTL = WyliczTTL(out, CLK);
	rysuj("TTL", 0, TTL.x.size() / f, TTL.ymin, TTL.ymax, TTL.x, TTL.y);

	Sygnal BAMI = WyliczBAMI(out, CLK);
	rysuj("BAMI", 0, BAMI.x.size() / f, BAMI.ymin, BAMI.ymax, BAMI.x, BAMI.y);

	Sygnal NRZI = WyliczNRZI(out, CLK);
	rysuj("NRZI", 0, NRZI.x.size() / f, NRZI.ymin, NRZI.ymax, NRZI.x, NRZI.y);
	
	Sygnal Manchester = WyliczManchester(out, CLK);
	rysuj("Manchester", 0, Manchester.x.size() / f, Manchester.ymin, Manchester.ymax, Manchester.x, Manchester.y);


	out = DecodeBAMI(BAMI, CLK);
	for (vector<double>::iterator it = out.begin(); it != out.end(); ++it) {
		cout << *it;
	}
	cout << endl;

	out = DecodeNRZI(NRZI, CLK);
	for (vector<double>::iterator it = out.begin(); it != out.end(); ++it) {
		cout << *it;
	}
	cout << endl;

	out = DecodeManchester(Manchester, CLK);
	for (vector<double>::iterator it = out.begin(); it != out.end(); ++it) {
		cout << *it;
	}
	cout << endl;
}