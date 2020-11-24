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
struct Sygnal {
	vector<double> y;
	vector<double> x;
	double ymin;
	double ymax;
};
void rysuj(string nazwa, double xmin, double xmax, double ymin, double ymax,vector<double> x, vector<double> y,bool histogram=false) {
	Gnuplot gp;
	gp << "set xrange ["<<xmin<<":"<<xmax<<"]\nset yrange [" << ymin << ":" << ymax << "]\n";
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
	rysuj(nazwa, 0, x[N - 1], ymin, ymax, x, widmo,true);
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
		dft.push_back(Zespolona(rzeczywista,urojona));
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
			for(int j=0;j<8;j++)
			{
				(val % 2) ? b.push_back(1) : b.push_back(0);
				val /= 2;
			}
			reverse(b.begin(), b.end());
			wynik.insert(wynik.end(),b.begin(),b.end());
		}
	}
	else {
		for (int i = in.size()-1; i >= 0; i--)
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


double za(double t, double b)
{
	float A1 = b == 0 ? 1 : 2;
	return A1 * sin(2 * PI * fs * t);
}
double zf(double t, double b)
{
	float f1 = b == 0 ? N + 1 / Tb : N + 2 / Tb;
	return Amp * sin(2 * PI * f1 * t);
}
double zp(double t, double b)
{
	float fi1 = b == 0 ? 0 : PI;
	return Amp * sin(2 * PI * fs * t + fi1);
}
Sygnal sygProstokatny(vector<double> y,double T,double f, double xmin, double xmax) {
	Sygnal wynik;
	int j = 0;
	for (double i = xmin; i < xmax; i = i + (1/f)) {
		if (i > T*(j+1)) {
			j++;
		}
		wynik.x.push_back(i);
		wynik.y.push_back(y[j]);
	}
	return wynik;
}
Sygnal wyliczMod(Sygnal syg, double(*wsk)(double, double)) {
	Sygnal wynik;
	for (int i = 0; i < syg.x.size(); i++) {
		wynik.x.push_back(syg.x[i]);
		wynik.y.push_back(wsk(syg.x[i],syg.y[i]));
		if (i == 0) {
			wynik.ymin = wynik.y[i];
			wynik.ymax = wynik.ymin;
		}
		if (wynik.y[i] < wynik.ymin)
			wynik.ymin = wynik.y[i];
		if (wynik.y[i] > wynik.ymax)
			wynik.ymax = wynik.y[i];
	}
	return wynik;
}

Sygnal Skroc(Sygnal in,int iloscBitow,double f, double Tb) {
	Sygnal wynik;
	for (int i = 0; i < iloscBitow * f * Tb; i++) {
		wynik.x.push_back(in.x[i]);
		wynik.y.push_back(in.y[i]);
	}
	return wynik;
}

int main() {
	//Słowo ABC
	string a = "ABC";
	vector<double> out = S2BS(a, true);
	for (vector<double>::iterator it = out.begin(); it != out.end(); ++it) {
		cout << *it;
	}
	cout << endl;
	Sygnal prost = sygProstokatny(out, Tb, f, 0, out.size()*Tb);
	rysuj("Prostokatny", 0, out.size() * Tb, -0.1, 1.1, prost.x, prost.y);
	double(*wskza)(double a,double b) = za;
	double(*wskzf)(double a, double b) = zf;
	double(*wskzp)(double a, double b) = zp;

	Sygnal za1=wyliczMod(prost, za);
	Sygnal zf1= wyliczMod(prost, zf);
	Sygnal zp1 = wyliczMod(prost, zp);
	rysuj("Za1", 0, out.size() * Tb, za1.ymin - 0.1 * za1.ymin, za1.ymax+0.1*za1.ymax, za1.x, za1.y);
	rysuj("Zf1", 0, out.size() * Tb, zf1.ymin - 0.1 * zf1.ymin, zf1.ymax + 0.1 * zf1.ymax, zf1.x, zf1.y);
	rysuj("Zp1", 0, out.size() * Tb, zp1.ymin - 0.1 * zp1.ymin, zp1.ymax + 0.1 * zp1.ymax, zp1.x, zp1.y);
	Sygnal prost2 = Skroc(prost, 10,f,Tb);
	Sygnal za2 = wyliczMod(prost2, za);
	Sygnal zf2 = wyliczMod(prost2, zf);
	Sygnal zp2 = wyliczMod(prost2, zp);
	rysuj("Za2", 0, 10 * Tb, za2.ymin - 0.1 * za2.ymin, za2.ymax + 0.1 * za2.ymax, za2.x, za2.y);
	rysuj("Zf2", 0, 10 * Tb, zf2.ymin - 0.1 * zf2.ymin, zf2.ymax + 0.1 * zf2.ymax, zf2.x, zf2.y);
	rysuj("Zp2", 0, 10 * Tb, zp2.ymin - 0.1 * zp2.ymin, zp2.ymax + 0.1 * zp2.ymax, zp2.x, zp2.y);

	rysuj_amplitudowy("M(k) - za1", za1.x.size(), f, oblicz_widmo(DFT(za1.y)));
	rysuj_amplitudowy("M(k) - zf1", zf1.x.size(), f, oblicz_widmo(DFT(zf1.y)));
	rysuj_amplitudowy("M(k) - zp1", zp1.x.size(), f, oblicz_widmo(DFT(zp1.y)));

}