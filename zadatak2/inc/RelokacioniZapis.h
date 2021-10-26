#pragma once
#include <string>
#include <iostream>

using namespace std;
class RelokacioniZapis
{
private:
	int sekcija, offset, adend, rbr_simbol;
	string tip_relokacije, ime_sekcije, ime_simbola;
public:
	RelokacioniZapis(int sek, int ofs, string tip, int r, int a);
	friend ostream& operator<<(ostream& it,const RelokacioniZapis& s);
	void postaviImeSekcije(string ime);
	void postaviImeSimbola(string ime);
	int dohvatiSekciju();
	string dohvatiTipRelokacije();
	string dohvatiImeSekcije();
	string dohvatiImeSimbola();
	int dohvatiOfset();
	int dohvatiRbrSimbola();
	int dohvatiAdend();
};

