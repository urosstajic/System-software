#pragma once

#include <iostream>
#include <list>
#include <iterator>
using namespace std;

#include "Simb.h"
class TabelaSimbola
{
private:
	list<Simb> tabelasimbola;
public:
	void dodajSimb(Simb s);
	bool da_li_postoji_simbol(string naziv);
	friend ostream& operator<<(ostream& it, TabelaSimbola t);
	void simbolGlobalan(string ime);
	Simb dohvatiSimbol(string ime);
	Simb& dohvatiSimbolZaMenjanje(string ime);
	int dohvatiDuzinu();
	Simb nadjiSimbPoImenuISekciji(string ime, string sekc);
	Simb dohvatiSimbPoRbr(int br);
};

