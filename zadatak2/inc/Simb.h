#pragma once
#include <iostream>
#include <string>
using namespace std;

class Simb
{
private:
	string labela;
	int sekcija;
	int ofset;
	int rbr;
	string local_global;
	static int pom_rbr;
	string naziv_sekcije;
public:
	Simb(string la, int sekc, int ofs, string loc);
	Simb(string la, int sekc, int ofs, string loc, int b);
	string getLabel();
	friend ostream& operator<<(ostream& it, const Simb& s);
	void simbolGlobalan();
	int dohvatiSekciju();
	int dohvatiRbr();
	string dohvatiLocGlob();
	int dohvatiOfset();
	void postaviNazivSekcije(string naziv);
	string dohvatiNazivSekcije();
	void postaviBrSekcija(int br);
	void postaviOfset(int br);
	void postaviLocGlob(string l);
};

