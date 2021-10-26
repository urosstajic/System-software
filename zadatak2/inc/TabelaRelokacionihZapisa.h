#pragma once
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include "RelokacioniZapis.h"

using namespace std;

class TabelaRelokacionihZapisa
{
private:
	list<RelokacioniZapis> relokacioni_zapisi;
public:
	void dodajRelokacioniZapis(RelokacioniZapis r);
	friend ostream& operator<<(ostream& it,  TabelaRelokacionihZapisa& s);
	string decToHex(int n);
	RelokacioniZapis dohvatiRelZapis(int br);
	int duzina();
	void ocistiSve();
};

