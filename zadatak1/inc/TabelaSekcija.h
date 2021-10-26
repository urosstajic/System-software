#pragma once
#include "Sekcija.h"
#include <list>
#include <iterator>

using namespace std;
class TabelaSekcija
{
private:
	list<Sekcija> tabela_sekcija;
public:
	void dodajSekciju(Sekcija s);
	bool da_li_postoji_sekcija(string naziv);
	Sekcija nadjiSekciju(string naziv);
	Sekcija nadjiSekcijuPoRBr(int br);
	int duzina();
	int pocetnaAdresa(string naziv);
	friend ostream& operator<<(ostream& it, TabelaSekcija t);
	Sekcija& nadjiSekcijuZaPromenuVelicine(int br);
	int dohvatiVelicinuCeleSekcije(string naziv);
	int nadjiSveVelPreMene(string naziv);
	int nadjiElemPreMene(string naziv);
	void izbaciSve();
};

