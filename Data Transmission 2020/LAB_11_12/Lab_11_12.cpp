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
double N = 2.0, A1 = 1.0, A2 = 2.0, Tb = 1.0, f = 2000.0;
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
			vector<int> b;
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
			vector<int> b;
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
string S2SB(vector<double> in, bool bigEndian) {
	string out;
	
	for (int j = 0; j < in.size() / 8; j++) {
		int outHelper = 0;
		for (int i = 0; i < 8; i++) {
			if (bigEndian) {
				outHelper += pow(2, 7 - i) * in[j*8+i];
			}
		}
		out += outHelper;
	}
	return out;
}

vector<double> Syg2Vec(vector<double> in, double Tb) {
	vector<double> out;
	for (int i = 0; i < int(in.size()/Tb); i++) {
		out.push_back(in[i * Tb + Tb / 2]);
	}
	return out;
}

vector<double> UproszczoneMnozenieMacierzy(vector<vector<double>> X, vector<double> Y) {
	vector<double> out;
	for (int i = 0; i < X.size(); i++) {
		int suma = 0;
		for (int j = 0; j < X[0].size(); j++) {
			suma += X[i][j] * Y[j];
		}
		out.push_back(suma % 2);
	}
	return out;
}
void PrintVector(vector<double> X) {
	if (X.size() == 0) {
		cout << "Po probie naprawy dalej wystapeju blad" << endl;
		return;
	}
	for (vector<double>::iterator it = X.begin(); it != X.end(); ++it) {
		cout << *it;
	}
	cout << endl;
}
void Zaneguj(vector<double> &s, int num) {
	if (s[num] == 0)
		s[num] = 1;
	else
		s[num] = 0;
	return;
}

Sygnal	WyliczCLK(int numBits, double fCLK) {
	Sygnal out;
	int value = 1;
	double help = 0;
	for (int i = 0; i < numBits * f / fCLK; i++) {
		if (help >= f / fCLK / 2) {
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
	double value = 0;
	Sygnal out;
	out.x = sygCLK.x;
	out.ymin = 0;
	out.ymax = 1;
	int j = 0;
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

vector<double> EncodeHamming(vector<vector<double>> G, vector<double> d){
	vector<double> d1(d.begin(), d.begin() + 4);
	vector<double> d2(d.begin()+4, d.end());
	vector<double> out;
	vector<double> temp=(UproszczoneMnozenieMacierzy(G, d1));
	out.insert(out.end(), temp.begin(), temp.end());
	temp = (UproszczoneMnozenieMacierzy(G, d2));
	out.insert(out.end(), temp.begin(), temp.end());
	return out;
}

vector<double> DecodeHamming(vector<vector<double>> H, vector<double> d) {
	vector<double> d1(d.begin(), d.begin() + 7);
	vector<double> d2(d.begin() + 7, d.end());
	vector<double> out;
	vector<double> temp = (UproszczoneMnozenieMacierzy(H, d1));
	int test = temp[0] * 1 + temp[1] * 2 + temp[2] * 4;
	if (test != 0) {
		cout << "Proba naprawy" << endl;
		Zaneguj(d1, test - 1);
	}
	out.push_back(d1[2]);
	out.push_back(d1[4]);
	out.push_back(d1[5]);
	out.push_back(d1[6]);
	temp = (UproszczoneMnozenieMacierzy(H, d2));
	test = temp[0] * 1 + temp[1] * 2 + temp[2] * 4;
	if (test != 0) {
		cout << "Proba naprawy" << endl;
		Zaneguj(d2, test);
	}
	out.push_back(d2[2]);
	out.push_back(d2[4]);
	out.push_back(d2[5]);
	out.push_back(d2[6]);
	return out;
}




vector<vector<double>> createG() {
	vector<vector<double>> out(7);
	double row0[4] = {1,1,0,1};
	out[0].insert(out[0].begin(),row0,row0+4);
	double row1[4] = { 1,0,1,1 };
	out[1].insert(out[1].begin(), row1, row1 + 4);
	double row2[4] = { 1,0,0,0 };
	out[2].insert(out[2].begin(), row2, row2 + 4);
	double row3[4] = { 0,1,1,1 };
	out[3].insert(out[3].begin(), row3, row3 + 4);
	double row4[4] = { 0,1,0,0 };
	out[4].insert(out[4].begin(), row4, row4 + 4);
	double row5[5] = { 0,0,1,0 };
	out[5].insert(out[5].begin(), row5, row5 + 4);
	double row6[4] = { 0,0,0,1 };
	out[6].insert(out[6].begin(), row6, row6 + 4);
	return out;
}

vector<vector<double>> createH() {
	vector<vector<double>> out(3);
	double row0[7] = { 1,0,1,0,1,0,1 };
	out[0].insert(out[0].begin(), row0, row0 + 7);
	double row1[7] = { 0,1,1,0,0,1,1 };
	out[1].insert(out[1].begin(), row1, row1 + 7);
	double row2[7] = { 0,0,0,1,1,1,1 };
	out[2].insert(out[2].begin(), row2, row2 + 7);
	return out;
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
	for (int i = 0; i < in.x.size() / (Tb * f); i++) {
		double pole = 0;
		int x= (i+1) * int(Tb) * int(f)- 1;
		if (x >= out.x.size()) {
			break;
		}
		for (int j = i * Tb * f; j < x; j++) {
			pole += (1.0 / f) * ((in.y[j] + in.y[j + 1]) / 2.0);
			out.y.push_back(pole);
		}
		out.y.push_back(pole);
		if (x + int(Tb) * int(f) >= out.x.size() && out.y.size()!=out.x.size()) {
			out.y.push_back(pole);
		}

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
	for (int i = 0; i < in.x.size() / (Tb * f); i++) {
		double maxPole = h - 1;
		int x = (i + 1) * int(Tb) * int(f) -1;
		if (x >= out.x.size()) {
			break;
		}
		for (int j = i * Tb * f; j < x; j++) {
			if (maxPole < in.y[j]) {
				maxPole = in.y[j];
			}
		}
		for (int j = i * Tb * f; j < x+1; j++) {
			double y = maxPole < h ? 0 : 1;
			out.y.push_back(y);
		}
		double y = maxPole < h ? 0 : 1;
		if (x + int(Tb) * int(f) >= out.x.size() && out.y.size() != out.x.size()) {
			out.y.push_back(y);
		}
	}
	return out;
}
int main() {
	//S³owo C
	bool testujNegacje = true;
	vector<vector<double>> G = createG();
	vector<vector<double>> H = createH();
	string a = "Ala ma kota";
	string wynik;
	cout << "Wejcie: " << a << endl;


	vector<double> out = S2BS(a, true);
	vector<double> EncHam;
	cout << "Kod Wejsciowy" << endl;
	PrintVector(out);
	for (int i = 0; i < out.size() / 8; i++) {
		vector<double> tempVec(out.begin() + i * 8, out.begin() + i * 8 + 8);
		vector<double> tempVec2 = EncodeHamming(G, tempVec);
		EncHam.insert(EncHam.end(),tempVec2.begin(),tempVec2.end());
	}
	cout << "Kod Hamminga:" << endl;
	PrintVector(EncHam);
	

	Sygnal prost = sygProstokatny(EncHam, Tb, f, 0, EncHam.size() * Tb);
	vector<double> s0, s1;
	for (int i = 0; i < EncHam.size(); i++) {
		s1.push_back(1);
		s0.push_back(0);
	}
	Sygnal pomocniczy1 = sygProstokatny(s1, Tb, f, 0, s1.size() * Tb);
	Sygnal pomocniczy0 = sygProstokatny(s0, Tb, f, 0, s0.size() * Tb);

	//rysuj("Prostokatny", 0, out.size() * Tb, -0.1, 1.1, prost.x, prost.y);
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
	/*rysuj("Za(t)", 0, out.size() * Tb, za1.ymin, za1.ymax, za1.x, za1.y);
	rysuj("Zp(t)", 0, out.size() * Tb, zp1.ymin, zp1.ymax, zp1.x, zp1.y);
	rysuj("Zf(t)", 0, out.size() * Tb, zf1.ymin, zf1.ymax, zf1.x, zf1.y);
	*/
	vector<double> SygA = Syg2Vec(ma.y, f);
	vector<double> SygP = Syg2Vec(mp.y, f);
	vector<double> SygF = Syg2Vec(mf.y, f);

	vector<double> DecHamA;
	for (int i = 0; i < SygA.size() / 14; i++) {
		vector<double> tempVec(SygA.begin() + i * 14, SygA.begin() + i * 14 + 14);
		vector<double> tempVec2 = DecodeHamming(H, tempVec);
		DecHamA.insert(DecHamA.end(), tempVec2.begin(), tempVec2.end());
	}	
	cout << "Zdekodowany Hamming dla ASK:" << endl;
	PrintVector(DecHamA);
	wynik=S2SB(DecHamA, true);
	cout << "Wyjscie ASK: " << wynik << endl;

	vector<double> DecHamP;
	for (int i = 0; i < SygP.size() / 14; i++) {
		vector<double> tempVec(SygP.begin() + i * 14, SygP.begin() + i * 14 + 14);
		vector<double> tempVec2 = DecodeHamming(H, tempVec);
		DecHamP.insert(DecHamP.end(), tempVec2.begin(), tempVec2.end());
	}
	cout << "Zdekodowany Hamming dla PSK:" << endl;
	PrintVector(DecHamP);
	wynik = S2SB(DecHamP, true);
	cout << "Wyjscie PSK: " << wynik << endl;

	vector<double> DecHamFs;
	for (int i = 0; i < SygF.size() / 14; i++) {
		vector<double> tempVec(SygF.begin() + i * 14, SygF.begin() + i * 14 + 14);
		vector<double> tempVec2 = DecodeHamming(H, tempVec);
		DecHamFs.insert(DecHamFs.end(), tempVec2.begin(), tempVec2.end());
	}
	cout << "Zdekodowany Hamming dla FSK:" << endl;
	PrintVector(DecHamFs);
	wynik = S2SB(DecHamFs, true);
	cout << "Wyjscie FSK: " << wynik << endl;
}