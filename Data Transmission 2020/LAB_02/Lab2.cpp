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
int E = 4, D = 1, C = 5, B = 2, A = 2;
double f = B;
double fi = C * PI;
double Amp = 1.0;
int q = 16;

double s(double t) {
	return Amp * sin(2 * PI * f * t + fi);
}
double k(double t,double q) {
	return (int)((1 + t) * pow(2, q - 1));
}
void rysuj(string nazwa, double xmin, double xmax, double ymin, double ymax,vector<double> x, vector<double> y) {
	Gnuplot gp;
	gp << "set xrange ["<<xmin<<":"<<xmax<<"]\nset yrange [" << ymin << ":" << ymax << "]\n";
	gp << "plot '-' with lines linestyle 1 title '"<<nazwa
		<< "'\n";
	gp.send1d(make_pair(x, y));
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
}
void wylicz(string nazwa, double t, double xmin, double xmax, bool kwantyzacja,double q) {
	vector<double> x, y;
	double ymin=0, ymax=0;

	int   i;
	for (i = 0; i < ((xmax - xmin) / t); i++) {
		x.push_back(xmin + t * i);
		y.push_back(s(x[i]));
		if (kwantyzacja)
			y[i] = k(y[i],q);
		if (i == 0) {
			ymin = y[i];
			ymax = ymin;
		}
		if (y[i] < ymin)
			ymin = y[i];
		if (y[i] > ymax)
			ymax = y[i];
	}
	rysuj( nazwa, xmin- (xmax - xmin) / 10, xmax+ (xmax - xmin) / 10, ymin- (ymax - ymin) / 10, ymax+(ymax-ymin)/10,x,y);
}

int main() {

	float t = 1.0 / 60000.0;
	wylicz("s(t)", t, 0, A, false, 0);
	wylicz("k(t) q=16 zakres (0;2)", t, 0, A, true, 16);
	wylicz("k(t) q=8 zakres (0;2)", t, 0, A, true,8);
	wylicz("k(t) q=4 zakres (0;2)", t, 0, A, true, 4);
}