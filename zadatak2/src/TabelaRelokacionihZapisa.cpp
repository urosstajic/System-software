#include "TabelaRelokacionihZapisa.h"
#include <iostream>
#include <string>
using namespace std;
void TabelaRelokacionihZapisa::dodajRelokacioniZapis(RelokacioniZapis r) {
	this->relokacioni_zapisi.push_back(r);
}

string TabelaRelokacionihZapisa::decToHex(int n)
{
	char hexaDeciNum[100];
	int i = 0;
	while (n != 0) {
		int temp = 0;
		temp = n % 16;
		if (temp < 10) {
			hexaDeciNum[i] = temp + 48;
			i++;
		}
		else {
			hexaDeciNum[i] = temp + 55;
			i++;
		}
		n = n / 16;
	}
	string pom = "";
	for (int j = i - 1; j >= 0; j--) {
		pom += hexaDeciNum[j];
	}

	return pom;
}

ostream& operator<<(ostream& it, TabelaRelokacionihZapisa& s) {
	it << "Tabela relokacionih zapisa" << endl;
	list <RelokacioniZapis> ::iterator iter;
	for (iter = s.relokacioni_zapisi.begin(); iter != s.relokacioni_zapisi.end(); ++iter) { //bilo je ++iter
		it << (*iter) << "\n"; //ne iterira dobro
	}
	return it;

}

RelokacioniZapis TabelaRelokacionihZapisa::dohvatiRelZapis(int br) {
	list<RelokacioniZapis>::iterator i;
	int broj = 0;
	for (i = this->relokacioni_zapisi.begin(); i != relokacioni_zapisi.end(); i++) {
		if (br == broj) {
			return (*i);
		}
		else {
			broj++;
		}
	}
	return RelokacioniZapis(-1, -1, "nista", -1, -1);
}

int TabelaRelokacionihZapisa::duzina() {
	return relokacioni_zapisi.size();
}

void TabelaRelokacionihZapisa::ocistiSve() {
	relokacioni_zapisi.clear();
}
