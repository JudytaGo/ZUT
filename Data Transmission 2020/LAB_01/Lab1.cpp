#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "plplot\plstream.h"
using namespace std;
#define PI 3.14159265
int E = 4, D = 1, C = 5, B = 9, A = 4;
void rysuj(const char nazwa[], PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, int dlugosc, PLFLT x[], PLFLT y[], int ilosc_pkt=0, PLFLT x1[] = NULL, PLFLT y1[] = NULL) {

	auto pls = new plstream();
	plsdev("wingcc");
	pls->init();
	pls->env(xmin, xmax, ymin, ymax, 0, 0);
	if (ilosc_pkt) {
		pls->col0(3);
		pls->string(ilosc_pkt, x1, y1, "#(728)");
	}
	pls->col0(1);
	pls->lab("x", "y", nazwa);
	pls->line(dlugosc, x, y);
	delete pls;
}
void wylicz(const char nazwa[],PLFLT(*wsk)(PLFLT), PLFLT t, PLFLT xmin, PLFLT xmax) {
	PLFLT* x, * y;
	PLFLT ymin, ymax;
	x = (PLFLT*)malloc(((xmax - xmin) / t) * sizeof(PLFLT));
	y = (PLFLT*)malloc(((xmax - xmin) / t) * sizeof(PLFLT));
	int   i;
	for (i = 0; i < ((xmax - xmin) / t); i++) {
		x[i] = xmin + t * i;
		y[i] = wsk(x[i]);
		if (i == 0) {
			ymin = y[i];
			ymax = ymin;
		}
		if (y[i] < ymin)
			ymin = y[i];
		if (y[i] > ymax)
			ymax = y[i];
	}
	rysuj(nazwa, xmin, xmax, ymin, ymax, ((xmax - xmin) / t), x, y);
	free(x);
	free(y);
}
void wylicz(const char nazwa[], PLFLT(*wsk)(PLFLT,int), PLFLT t, PLFLT xmin, PLFLT xmax,int N) {
	PLFLT* x, * y;
	PLFLT ymin, ymax;
	x = (PLFLT*)malloc(((xmax - xmin) / t) * sizeof(PLFLT));
	y = (PLFLT*)malloc(((xmax - xmin) / t) * sizeof(PLFLT));
	int   i;
	for (i = 0; i < ((xmax - xmin) / t); i++) {
		x[i] = xmin + t * i;
		y[i] = wsk(x[i],N);
		if (i == 0) {
			ymin = y[i];
			ymax = ymin;
		}
		if (y[i] < ymin)
			ymin = y[i];
		if (y[i] > ymax)
			ymax = y[i];
	}
	rysuj(nazwa, xmin, xmax, ymin, ymax, ((xmax - xmin) / t), x, y);
	free(x);
	free(y);
}

void kwadratowa() {
	PLFLT *x,*y;
	double t = 0.01;
	PLFLT ymin, ymax, xmin = -10., xmax = 10.;
	x = (PLFLT*)malloc(((xmax - xmin) / t) * sizeof(PLFLT));
	y = (PLFLT*)malloc(((xmax - xmin) / t) * sizeof(PLFLT));
	int   i;
	for (i = 0; i < ((xmax - xmin) / t); i++) {
		x[i] = xmin + t * i;
		y[i] = A*x[i]*x[i]+B*x[i]+C;
		if (i == 0) {
			ymin = y[i];
			ymax = ymin;
		}
		if (y[i] < ymin)
			ymin = y[i];
		if (y[i] > ymax)
			ymax = y[i];
	}
	PLFLT delta=B*B-4*A*C;
	PLFLT x1[2], y1[2];
	int ilosc_zerowych = 0;
	if (delta > 0) {
		ilosc_zerowych = 2;
		x1[0] = (-B + sqrt(delta)) / (2 * A);
		y1[0] = 0;
		x1[1] = (-B - sqrt(delta)) / (2 * A);
		y1[1] = 0;
	}
	else if (delta == 0) {
		ilosc_zerowych = 1;
		x1[0] = (-B + sqrt(delta)) / 2 * A;
		y1[0] = 0;
	}
	rysuj("Ax^2+Bx+C",xmin, xmax, ymin, ymax, ((xmax - xmin) / t), x, y, ilosc_zerowych, x1, y1);
	free(x);
	free(y);
}
PLFLT x(PLFLT t) {
	return A * t * t + B * t + C;
}
PLFLT y(PLFLT t) {
	return 2*x(t)*x(t)+12*cos(t);
}
PLFLT z(PLFLT t) {
	return sin(2*PI*7*t)*x(t)-0.2*log10(abs(y(t)+PI));
}
PLFLT u(PLFLT t) {
	return sqrt(abs(y(t)*y(t)*z(t)))-1.8 * sin(0.4*t*z(t)*x(t));
}
PLFLT v(PLFLT t) {
	if (t < 0 || t>1)
		return 0;
	if (t < 0.22)
		return (1 - 7 * t) * sin((2 * PI * t * 10) / (t + 0.04));
	if (t < 0.7)
		return 0.63 * t * sin(125 * t);
	if (t <= 1)
		return pow(t, -0.0662) + 0.77 * sin(8 * t);
}
PLFLT p(PLFLT t, int N) {
	PLFLT wynik=0.0;
	for (int i = 1; i < N+1; i++) {
		wynik += (cos(12 * t * i * i) + cos(16 * t * i)) / (i * i);
	}
	return wynik;
}
int main() {
	kwadratowa();
	PLFLT(*wskY)(PLFLT a) = y;
	PLFLT(*wskZ)(PLFLT a) = z;
	PLFLT(*wskU)(PLFLT a) = u;
	PLFLT(*wskV)(PLFLT a) = v;
	PLFLT(*wskP)(PLFLT a,int n) = p;
	PLFLT t = 1.0 / 22050.0;
	wylicz("y(t)", wskY, t, 0, 1);
	wylicz("z(t)", wskZ, t, 0, 1);
	wylicz("u(t)", wskU, t, 0, 1);
	wylicz("v(t)", wskV, t, 0, 1);
	wylicz("p(t) N=2", wskP, t, 0, 1,2);
	wylicz("p(t) N=4", wskP, t, 0, 1, 4);
	wylicz("p(t) N=49", wskP, t, 0, 1, A*10+B);

	return 0;
}