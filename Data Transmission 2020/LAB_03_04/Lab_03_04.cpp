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
double f = B;
double fi = C * PI;
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

double s(double t) {
	return Amp * sin(2 * PI * f * t + fi);
}
double k(double t,double q) {
	return (int)((1 + t) * pow(2, q - 1));
}
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
Sygnal ODFT(vector<Zespolona> dft, double xmin, double xmax,int n) {
	Sygnal syg;
	double w=0.0;
	for (double i = 0; i < n;i++) {
		w += (xmax - xmin) / n;
		syg.x.push_back(w);
	}
	for (int i = 0; i < dft.size(); i++) {
		double rzeczywista = 0.0;
		double urojona = 0.0;

		for (int j = 0; j < dft.size(); j++) {
			rzeczywista += dft[j].rzeczywista * cos(2 * PI * i * j / dft.size());
			urojona += dft[j].urojona * sin(2 * PI * i * j / dft.size());
		}
		syg.y.push_back((rzeczywista + urojona) / n);
	}
	syg.ymax = 1;
	syg.ymin = -1;
	return syg;
}

Sygnal wylicz_sygnal(double xmin, double xmax, double t, double(*wsk)(double)) {
	Sygnal wynik;
	for (int i = xmin; i < xmax; i++) {
		wynik.x.push_back(xmin + t * i);
		wynik.y.push_back(wsk(wynik.x[i]));
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
Sygnal wylicz_sygnalP(double xmin, double xmax, double t, double(*wsk)(double,int),int N) {
	Sygnal wynik;
	for (int i = xmin; i < xmax; i++) {
		wynik.x.push_back(xmin + t * i);
		wynik.y.push_back(wsk(wynik.x[i],N));
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

double x(double t) {
	return A * t * t + B * t + C;
}
double y(double t) {
	return 2 * x(t) * x(t) + 12 * cos(t);
}
double z(double t) {
	return sin(2 * PI * 7 * t) * x(t) - 0.2 * log10(abs(y(t) + PI));
}
double u(double t) {
	return sqrt(abs(y(t) * y(t) * z(t))) - 1.8 * sin(0.4 * t * z(t) * x(t));
}
double v(double t) {
	if (t < 0 || t>1)
		return 0;
	if (t < 0.22)
		return (1 - 7 * t) * sin((2 * PI * t * 10) / (t + 0.04));
	if (t < 0.7)
		return 0.63 * t * sin(125 * t);
	if (t <= 1)
		return pow(t, -0.0662) + 0.77 * sin(8 * t);
}
double p(double t, int N) {
	double wynik = 0.0;
	for (int i = 1; i < N + 1; i++) {
		wynik += (cos(12 * t * i * i) + cos(16 * t * i)) / (i * i);
	}
	return wynik;
}
vector<double> oblicz_widmo(vector<Zespolona> dft) {
	vector<double> wynik;
	for (int i = 0; i < dft.size(); i++) {
		wynik.push_back(sqrt(dft[i].rzeczywista * dft[i].rzeczywista + dft[i].urojona * dft[i].urojona));
	}
	return wynik;
}
int main() {
	double(*wskS)(double a) = s;

	double(*wskX)(double a) = x;
	double(*wskY)(double a) = y;
	double(*wskZ)(double a) = z;
	double(*wskU)(double a) = u;
	double(*wskV)(double a) = v;
	double(*wskP)(double a, int n) = p;

	float t = 1.0/483;
	int n = 483;
	int zakres = n*t;
	Sygnal sig = wylicz_sygnal(0, n, t,wskS);
	rysuj("s(x)", 0, zakres, sig.ymin, sig.ymax, sig.x, sig.y);
	vector<Zespolona> dft1 = DFT(sig.y);
	vector<double> widmo=oblicz_widmo(dft1);
	rysuj_amplitudowy("M(k) - s",n,1/t, widmo);

	sig = wylicz_sygnal(0, n, t, wskX);
	vector<Zespolona> dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - x", n, 1 / t, widmo);

	sig = wylicz_sygnal(0, n, t, wskY);
	dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - y", n, 1 / t, widmo);

	sig = wylicz_sygnal(0, n, t, wskZ);
	dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - z", n, 1 / t, widmo);

	sig = wylicz_sygnal(0, n, t, wskU);
	dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - u", n, 1 / t, widmo);

	sig = wylicz_sygnal(0, n, t, wskV);
	dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - v", n, 1 / t, widmo);

	sig = wylicz_sygnalP(0, n, t, wskP,2);
	dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - p N=2", n, 1 / t, widmo);

	sig = wylicz_sygnalP(0, n, t, wskP, 4);
	dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - p N=4", n, 1 / t, widmo);

	sig = wylicz_sygnalP(0, n, t, wskP, 49);
	dft = DFT(sig.y);
	widmo = oblicz_widmo(dft);
	rysuj_amplitudowy("M(k) - p N=49", n, 1 / t, widmo);


	Sygnal sig2 = ODFT(dft1, 0, zakres, n);
	rysuj("y(x) po odwroceniu", 0, zakres, sig2.ymin, sig2.ymax, sig2.x, sig2.y);
}