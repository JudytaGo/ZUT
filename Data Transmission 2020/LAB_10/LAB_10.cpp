#define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include "gnuplot-iostream.h"

using namespace std;


vector<int> S2BS(string in, bool bigEndian)
{
	vector<int> wynik;
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
vector<int> UproszczoneMnozenieMacierzy(vector<vector<int>> X, vector<int> Y) {
	vector<int> out;
	for (int i = 0; i < X.size(); i++) {
		int suma = 0;
		for (int j = 0; j < X[0].size(); j++) {
			suma += X[i][j] * Y[j];
		}
		out.push_back(suma % 2);
	}
	return out;
}
void PrintVector(vector<int> X) {
	if (X.size() == 0) {
		cout << "Wystapily bledy" << endl;
		return;
	}
	for (vector<int>::iterator it = X.begin(); it != X.end(); ++it) {
		cout << *it;
	}
	cout << endl;
}
void Zaneguj(vector<int> &s, int num) {
	if (s[num] == 0)
		s[num] = 1;
	else
		s[num] = 0;
	return;
}

vector<int> EncodeHamming(vector<vector<int>> G, vector<int> d){
	vector<int> d1(d.begin(), d.begin() + 4);
	vector<int> d2(d.begin()+4, d.end());
	vector<int> out;
	vector<int> temp=(UproszczoneMnozenieMacierzy(G, d1));
	out.insert(out.end(), temp.begin(), temp.end());
	temp = (UproszczoneMnozenieMacierzy(G, d2));
	out.insert(out.end(), temp.begin(), temp.end());
	return out;
}

vector<int> DecodeHamming(vector<vector<int>> H, vector<int> d) {
	vector<int> d1(d.begin(), d.begin() + 7);
	vector<int> d2(d.begin() + 7, d.end());
	vector<int> out;
	vector<int> temp = (UproszczoneMnozenieMacierzy(H, d1));
	int test = temp[0] * 1 + temp[1] * 2 + temp[2] * 4;
	if (test != 0) 
		Zaneguj(d1, test-1);
	out.push_back(d1[2]);
	out.push_back(d1[4]);
	out.push_back(d1[5]);
	out.push_back(d1[6]);
	temp = (UproszczoneMnozenieMacierzy(H, d2));
	test = temp[0] * 1 + temp[1] * 2 + temp[2] * 4;
	if (test != 0)
		Zaneguj(d2, test);
	out.push_back(d2[2]);
	out.push_back(d2[4]);
	out.push_back(d2[5]);
	out.push_back(d2[6]);
	return out;
}




vector<vector<int>> createG() {
	vector<vector<int>> out(7);
	int row0[4] = {1,1,0,1};
	out[0].insert(out[0].begin(),row0,row0+4);
	int row1[4] = { 1,0,1,1 };
	out[1].insert(out[1].begin(), row1, row1 + 4);
	int row2[4] = { 1,0,0,0 };
	out[2].insert(out[2].begin(), row2, row2 + 4);
	int row3[4] = { 0,1,1,1 };
	out[3].insert(out[3].begin(), row3, row3 + 4);
	int row4[4] = { 0,1,0,0 };
	out[4].insert(out[4].begin(), row4, row4 + 4);
	int row5[5] = { 0,0,1,0 };
	out[5].insert(out[5].begin(), row5, row5 + 4);
	int row6[4] = { 0,0,0,1 };
	out[6].insert(out[6].begin(), row6, row6 + 4);
	return out;
}

vector<vector<int>> createH() {
	vector<vector<int>> out(3);
	int row0[7] = { 1,0,1,0,1,0,1 };
	out[0].insert(out[0].begin(), row0, row0 + 7);
	int row1[7] = { 0,1,1,0,0,1,1 };
	out[1].insert(out[1].begin(), row1, row1 + 7);
	int row2[7] = { 0,0,0,1,1,1,1 };
	out[2].insert(out[2].begin(), row2, row2 + 7);
	return out;
}

vector<int> EncoderSECDED(vector<vector<int>> G, vector<int> d) {
	vector<int> out;
	vector<int> EncHam = EncodeHamming(G, d);
	int p4 = 0;
	for (int i = 0; i < EncHam.size(); i++) {
		out.push_back(EncHam[i]);
		p4 += EncHam[i];
		if (i % 7 == 6) {
			out.push_back(p4%2);
			p4 = 0;
		}
	}
	return out;
}

vector<int> DecoderSECDED(vector<vector<int>> H, vector<int> d) {
	vector<int> out;
	vector<int> d1(d.begin(), d.begin() + 7);
	vector<int> d2(d.begin() + 8, d.end()-1);
	vector<int> temp = (UproszczoneMnozenieMacierzy(H, d1));
	int test = temp[0] * 1 + temp[1] * 2 + temp[2] * 4;
	if (test != 0)
		Zaneguj(d1, test-1);
	int suma = 0;
	for (int i = 0; i < d1.size(); i++) {
		suma += d1[i];
	}
	suma = suma % 2;
	if (suma == d[7]) {
		out.push_back(d1[2]);
		out.push_back(d1[4]);
		out.push_back(d1[5]);
		out.push_back(d1[6]);
	}
	else {
		out.clear();
		return out;
	}
	temp = (UproszczoneMnozenieMacierzy(H, d2));
	test = temp[0] * 1 + temp[1] * 2 + temp[2] * 4;
	if (test != 0)
		Zaneguj(d2, test-1);
	suma = 0;
	for (int i = 0; i < d2.size(); i++) {
		suma += d2[i];
	}
	suma = suma % 2;
	if (suma == d[15]) {
		out.push_back(d2[2]);
		out.push_back(d2[4]);
		out.push_back(d2[5]);
		out.push_back(d2[6]);
	}
	else {
		out.clear();
		return out;
	}
	return out;
}

int main() {
	//Słowo C
	bool testujNegacje = true;
	vector<vector<int>> G = createG();
	vector<vector<int>> H = createH();
	string a = "C";
	vector<int> out = S2BS(a, true);
	cout << "Kod Wejsciowy" << endl;
	PrintVector(out);
	vector<int> EncHam = EncodeHamming(G, out);
	cout << "Kod Hamminga:" << endl;
	PrintVector(EncHam);

	vector<int> DecHam = DecodeHamming(H, EncHam);
	cout << "Zdekodowany Hamming:" << endl;
	PrintVector(DecHam);

	EncHam = EncodeHamming(G, out);
	vector<int> EncSECDED = EncoderSECDED(G, out);
	cout << "Kod SECDEC:" << endl;
	PrintVector(EncSECDED);

	vector<int> DecSECDED = DecoderSECDED(H, EncSECDED);
	cout << "Zdekodowany SECDEC:" << endl;
	PrintVector(DecSECDED);

	if (testujNegacje) {
		Zaneguj(EncSECDED, 4);
		Zaneguj(EncSECDED, 5);
	}

	vector<int> DecSECDEDzNegacja2 = DecoderSECDED(H, EncSECDED);
	cout << "Zdekodowany SECDEC po zanegowaniu 2 bitow:" << endl;
	PrintVector(DecSECDEDzNegacja2);
}