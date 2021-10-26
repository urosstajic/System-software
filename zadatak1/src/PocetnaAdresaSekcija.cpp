#include "PocetnaAdresaSekcija.h"
PocetnaAdresaSekcija::PocetnaAdresaSekcija(string p, string i) {
	poc_adr = p;
	ime_sekcije = i;
}

string PocetnaAdresaSekcija::dohvatiImeSekcije() {
	return ime_sekcije;
}

string PocetnaAdresaSekcija::dohvatiPocAdr() {
	return poc_adr;
}