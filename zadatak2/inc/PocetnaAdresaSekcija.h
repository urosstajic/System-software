#pragma once
#include <iostream>
#include <string>

using namespace std;
class PocetnaAdresaSekcija
{private:
	string poc_adr, ime_sekcije;
public:
	PocetnaAdresaSekcija(string p, string i);
	string dohvatiPocAdr();
	string dohvatiImeSekcije();
};

