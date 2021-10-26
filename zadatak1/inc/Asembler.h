#pragma once
#include "TabelaSimbola.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include "TabelaSekcija.h"
#include "ListaSadrzajaSekcije.h"
#include "RelokacioniZapis.h"
#include "TabelaRelokacionihZapisa.h"

using namespace std;

class Asembler
{
private:
	TabelaSimbola tabela_simb;
	TabelaSekcija tabela_sekcija;
	ListaSadrzajaSekcije lista_sadrz_sekcije;
	TabelaRelokacionihZapisa tab_rel_zapisa;
public:
	void prvi_prolaz(ifstream& ulazfajl);
	void drugi_prolaz(ifstream& ulazfajl, string ime_izlaznog);
	string decToHexa(int n);
	string decToHexaZaNegativne(int br);
};

