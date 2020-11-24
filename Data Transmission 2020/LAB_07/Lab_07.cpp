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
Sygnal sygProstokatny(vector<double> y, double T, double f, double xmin, double xmax) {
	Sygnal wynik;
	int j = 0;
	for (double i = xmin; i < xmax; i = i + (1 / f)) {
		if (i > T* (j + 1)) {
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
		wynik.y.push_back(wsk(syg.x[i], syg.y[i]));
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

Sygnal Skroc(Sygnal in, int iloscBitow, double f, double Tb) {
	Sygnal wynik;
	for (int i = 0; i < iloscBitow * f * Tb; i++) {
		wynik.x.push_back(in.x[i]);
		wynik.y.push_back(in.y[i]);
	}
	return wynik;
}

Sygnal WyliczP(Sygnal in, double Tb, double f) {
	Sygnal out;
	out.x = in.x;
	out.ymin = 0;
	out.ymax = 0;
	for (int i = 0; i < in.x.size() / (Tb * f) - 1; i++) {
		double pole = 0;
		for (int j = i * Tb * f; j < (i + 1) * Tb * f; j++) {
			pole += (1.0 / f) * ((in.y[j] + in.y[j + 1]) / 2.0);
			out.y.push_back(pole);
		}
		if (i + 1 > in.x.size() / (Tb * f) - 1)
			out.y.push_back(pole);
		
	}
	for (int i = 0; i < out.y.size(); i++) {
		if (out.y[i] < out.ymin)
			out.ymin = out.y[i];
		if (out.y[i] > out.ymax)
			out.ymax = out.y[i];
	}
	return out;
}

Sygnal WyliczM(Sygnal in, double h) {
	Sygnal out;
	out.x = in.x;
	out.ymin = 0;
	out.ymax = 1;
	for (int i = 0; i < in.x.size() / (Tb * f) - 1; i++) {
		double maxPole = h-1;
		for (int j = i * Tb * f; j < (i + 1) * Tb * f; j++) {
			if (maxPole < in.y[j]) {
				maxPole=in.y[j];
			}
		}
		for (int j = i * Tb * f; j < (i + 1) * Tb * f; j++) {
			double y = maxPole < h ? 0 : 1;
			out.y.push_back(y);
		}
		if (i + 1 > in.x.size() / (Tb * f) - 1) {
			double y = maxPole < h ? 0 : 1;
			out.y.push_back(y);
		}

	}
	return out;
}

int main() {
	//Słowo ABC
	string a = "ABC";
	vector<double> out = S2BS(a, true);
	for (vector<double>::iterator it = out.begin(); it != out.end(); ++it) {
		cout << *it;
	}
	vector<double> s0, s1;
	for (int i = 0; i < out.size(); i++) {
		s1.push_back(1);
		s0.push_back(0);
	}
	cout << endl;
	Sygnal prost = sygProstokatny(out, Tb, f, 0, out.size() * Tb);
	Sygnal pomocniczy1 = sygProstokatny(s1, Tb, f, 0, s1.size() * Tb);
	Sygnal pomocniczy0 = sygProstokatny(s0, Tb, f, 0, s0.size() * Tb);
	rysuj("Prostokatny", 0, out.size() * Tb, 0, 1, prost.x, prost.y);
	double(*wskza)(double a, double b) = za;
	double(*wskzf)(double a, double b) = zf;
	double(*wskzp)(double a, double b) = zp;

	Sygnal za1 = wyliczMod(prost, za);
	Sygnal sa = wyliczMod(pomocniczy1, za);
	Sygnal zp1 = wyliczMod(prost, zp);
	Sygnal sp = wyliczMod(pomocniczy1, zp);
	Sygnal zf1 = wyliczMod(prost, zf);
	Sygnal sf1 = wyliczMod(pomocniczy1, zf);
	Sygnal sf2 = wyliczMod(pomocniczy0, zf);
	Sygnal xa = za1 * sa;
	Sygnal xp = zp1 * sp;
	Sygnal xf1 = zf1 * sf1;
	Sygnal xf2 = zf1 * sf2;
	Sygnal pa = WyliczP(xa, Tb, f);
	Sygnal pp = WyliczP(xp, Tb, f);
	Sygnal pf = WyliczP(xf1 - xf2, Tb, f);
	Sygnal ma = WyliczM(pa, 1.5);
	Sygnal mp = WyliczM(pp, 0.01);
	Sygnal mf = WyliczM(pf, 0.01);

	rysuj("Za(t)", 0, out.size() * Tb, za1.ymin, za1.ymax, za1.x, za1.y);
	rysuj("xa(t)", 0, out.size() * Tb, xa.ymin, xa.ymax, xa.x, xa.y);
	rysuj("pa(t)", 0, out.size() * Tb, pa.ymin, pa.ymax, pa.x, pa.y);
	rysuj("Ma(t)", 0, out.size() * Tb, ma.ymin, ma.ymax, ma.x, ma.y);

	rysuj("Zp(t)", 0, out.size() * Tb, zp1.ymin, zp1.ymax, zp1.x, zp1.y);
	rysuj("xp(t)", 0, out.size() * Tb, xp.ymin, xp.ymax, xp.x, xp.y);
	rysuj("pp(t)", 0, out.size() * Tb, pp.ymin, pp.ymax, pp.x, pp.y);
	rysuj("Mp(t)", 0, out.size() * Tb, mp.ymin, mp.ymax, mp.x, mp.y);

	rysuj("Zf(t)", 0, out.size() * Tb, zf1.ymin, zf1.ymax, zf1.x, zf1.y);
	rysuj("xf1(t)", 0, out.size() * Tb, xf1.ymin, xf1.ymax, xf1.x, xf1.y);
	rysuj("xf2(t)", 0, out.size() * Tb, xf2.ymin, xf2.ymax, xf2.x, xf2.y);
	rysuj("pf(t)", 0, out.size() * Tb, pf.ymin, pf.ymax, pf.x, pf.y);
	rysuj("Mf(t)", 0, out.size() * Tb, mf.ymin, mf.ymax, mf.x, mf.y);
}