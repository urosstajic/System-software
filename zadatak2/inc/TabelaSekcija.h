#pragma once

#include<iostream>
#include <string>
#include "Sekcija.h"
#include <list>
#include <iterator>
using namespace std;

	
class TabelaSekcija
{
public:
	list <Sekcija> tabela;
	friend ostream& operator<<(ostream& it, TabelaSekcija t);
	void dodajSekciju(Sekcija s);
	bool postojiSekcija(string ime);
	Sekcija& dohvSek(string deo);
};

