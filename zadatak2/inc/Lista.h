#pragma once
#include "Sekcija.h"
#include <list>
#include <string>
#include <iterator>
#include <iostream>

using namespace std;
class Lista
{
private:
	list<Sekcija> koje_sekcije;
public:
	Lista(list<Sekcija> l);
	int nadjiPocetnuAdresu(string naziv);
	int nadjiAdrZaLinkable(string naziv);
	int nadjiAdrZaHex(string naziv);
};

