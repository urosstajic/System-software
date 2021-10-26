#pragma once
#include <iostream>
#include <list>
#include <string>
#include "SadrzajSekcije.h"
#include <iterator>
#include <fstream>
using namespace std;

class ListaSadrzajaSekcije
{
private:
	list<SadrzajSekcije> lista;
public:
	void dodajSadrzajSekcije(SadrzajSekcije s);
	bool da_li_postoji_sadrzajSekcije(string ime);
	SadrzajSekcije& dohvatiSekciju(string ime);
	friend ostream& operator <<(ostream& it, ListaSadrzajaSekcije t);
	string dohvatiKod(string sekcija, int ofset);
	void ispisiSveSadrzaje(ofstream& fajl);
	void izmeniKod(string sekcija, int ofset, string kodd);
	int duzina();
	SadrzajSekcije dohvatiSadrzaj(int br);
	SadrzajSekcije dohvatiSadrzajPoNazivuSekcije(string naziv);
};

