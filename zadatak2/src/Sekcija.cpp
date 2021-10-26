#include "Sekcija.h"
#include <iostream>
#include <iomanip>

using namespace std;
int Sekcija::pom_rbr = 0;

Sekcija::Sekcija(string n) {
	this->naziv = n;
	//this->objasnjenje = o;
	//this->offset = of;
	//this->sadrzaj = s;
	this->rbr = pom_rbr;
	pom_rbr++;
}

string Sekcija::getNaziv() {
	return this->naziv;
}

int Sekcija::getRbr() {
	return this->rbr;
}

void Sekcija::postaviVelicinu(int v) {
	this->velicina = v;
}

ostream& operator<<(ostream& it, const Sekcija& s)
{
	const char separator = ' ';
	const int nameWidth = 15;
	const int numWidth = 18;
	
	it << left << setw(numWidth) << setfill(separator) << s.naziv;
	it << left << setw(numWidth) << setfill(separator) << s.rbr;
	it << left << setw(numWidth) << setfill(separator) << s.velicina;
	return it;
}

Sekcija::Sekcija(string n, int r, int vel) {
	naziv = n;
	rbr = r;
	velicina = vel;
}

int Sekcija::getVelicina() {
	return this->velicina;
}

void Sekcija::postaviPocetnuAdresu(int p) {
	pocetna_adresa = p;
}

int Sekcija::dohvatiPocetnuAdresu() {
	return pocetna_adresa;
}