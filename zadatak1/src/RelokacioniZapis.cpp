#include "RelokacioniZapis.h"
#include <iomanip>
RelokacioniZapis::RelokacioniZapis(int sek, int ofs, string tip, int r, int a) {
	this->sekcija = sek;
	this->offset = ofs;
	this->tip_relokacije = tip;
	this->rbr_simbol = r;
	this->adend = a;
}

ostream& operator<<(ostream& it,const RelokacioniZapis& s)
{
	const char separator = ' ';
	const int nameWidth = 25;
	const int numWidth = 15;

	it << left << setw(numWidth) << setfill(separator) << s.ime_sekcije;
	it << left << setw(numWidth) << setfill(separator) << s.offset;
	it << left << setw(nameWidth) << setfill(separator) << s.tip_relokacije;
	it << left << setw(numWidth) << setfill(separator) << s.ime_simbola;
	it << left << setw(numWidth) << setfill(separator) << s.adend;
	return it;
}

void RelokacioniZapis::postaviImeSekcije(string ime) {
	ime_sekcije = ime;
}

void RelokacioniZapis::postaviImeSimbola(string ime) {
	ime_simbola = ime;
}

string RelokacioniZapis::dohvatiImeSekcije() {
	return ime_sekcije;
}

int RelokacioniZapis::dohvatiSekciju() {
	return sekcija;
}

string RelokacioniZapis::dohvatiImeSimbola() {
	return ime_simbola;
}

string RelokacioniZapis::dohvatiTipRelokacije() {
	return tip_relokacije;
}

int RelokacioniZapis::dohvatiOfset(){
	return offset;
}

int RelokacioniZapis::dohvatiRbrSimbola() {
	return rbr_simbol;
}


int RelokacioniZapis::dohvatiAdend() {
	return adend;
}