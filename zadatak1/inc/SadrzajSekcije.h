#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class SadrzajSekcije
{
private:
	string sekcija;
	string kod;
	
public:
	static int glob_brojac;
	SadrzajSekcije(string s);
	void dodajKod(string kod);
	string dohvatiNaziv();
	friend ostream& operator<<(ostream& it, SadrzajSekcije t);
	int dohvati_glob_brojac();
	void postavi_glob_brojac();
	string nadjiKod(int ofset);
	void ispisSadrzaja(ofstream& fajl);
	void izmeniKod(int ofset, string kodd);
	string dohvatiKod();
};

