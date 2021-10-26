#include "Simb.h"
#include <iomanip>
int Simb::pom_rbr = 0;

Simb::Simb(string la, int sekc, int ofs, string loc) {
	this->labela = la;
	this->sekcija = sekc;
	this->ofset = ofs;
	this->rbr = pom_rbr;
	this->local_global = loc;
	pom_rbr++;
}

string Simb::getLabel() {
	return this->labela;
}

ostream& operator<<(ostream& it, const Simb& s)
{
	const char separator = ' ';
	const int nameWidth = 12;
	const int numWidth = 15;

	it << left << setw(numWidth) << setfill(separator) << s.labela;
	it << left << setw(numWidth) << setfill(separator) << s.ofset;
	it << left << setw(numWidth) << setfill(separator) << s.naziv_sekcije;
	it << left << setw(numWidth) << setfill(separator) << s.local_global;
	it << left << setw(numWidth) << setfill(separator) << s.rbr;
	string ispis_sek;
	return it;
}

void Simb::simbolGlobalan() {
	this->local_global = "global";
}

int Simb::dohvatiSekciju() {
	return this->sekcija;
}

int Simb::dohvatiRbr() {
	return this->rbr;
}

string Simb::dohvatiLocGlob() {
	return this->local_global;
}

int Simb::dohvatiOfset() {
	return ofset;
}

Simb::Simb(string la, int ofs, int sekc, string loc, int b) {
	this->labela = la;
	this->sekcija = sekc;
	this->ofset = ofs;
	this->rbr = b;
	this->local_global = loc;
}

void Simb::postaviNazivSekcije(string naziv) {
	naziv_sekcije = naziv;
}

string Simb::dohvatiNazivSekcije() {
	return naziv_sekcije;
}

void Simb::postaviBrSekcija(int br) {
	sekcija = br;
}

void Simb::postaviLocGlob(string g) {
	local_global = g;
}

void Simb::postaviOfset(int br) {
	ofset = br;
}