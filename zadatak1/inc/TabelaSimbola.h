#pragma once
#include<iostream>
#include <string>
#include "Simbol.h"
#include <list>
#include <iterator>
using namespace std;


class TabelaSimbola
{
private:
	


public:
	list <Simbol> tabela;
	TabelaSimbola();
	void dodajSimbol(Simbol s);
	bool proveriLabelu(string label); //vraca true ako moze da se doda u tabelu simbola
	void postaviGlobal(string lab);
	int proveriGlobalnost(string ucitan);
	int dohvatiRB(string ime);
	int dohvatiRBSekcije(string ime);
	int dohvatiOffset(string ucitan);
	string dohvatiSekciju(string ucitan);
	int dohvatiOffsetRB(int rbb);
	Simbol& dohvSimbol(string ime);
	friend ostream& operator<<(ostream& it, TabelaSimbola t);
	~TabelaSimbola();
	Simbol & dohvSimbRBPo(int rb);
	bool postojiLabela(string labela);

	
};

