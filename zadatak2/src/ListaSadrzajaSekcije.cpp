#include "ListaSadrzajaSekcije.h"
#include <fstream>
void ListaSadrzajaSekcije::dodajSadrzajSekcije(SadrzajSekcije s) {
	this->lista.push_back(s);
}

bool ListaSadrzajaSekcije::da_li_postoji_sadrzajSekcije(string ime) {
	list <SadrzajSekcije> ::iterator i;
	for (i = this->lista.begin(); i != lista.end(); i++) {
		if ((*i).dohvatiNaziv() == ime) {
			return true;
		}
	}
	return false;
}

//proveri jel ce dobro da se menja
SadrzajSekcije& ListaSadrzajaSekcije::dohvatiSekciju(string ime) {
	list<SadrzajSekcije>::iterator i;
	for (i = this->lista.begin(); i != lista.end(); i++) {
		if ((*i).dohvatiNaziv() == ime) {
			return (*i);
		}
	}
	SadrzajSekcije& s=(*i);
	return s;
}



ostream& operator <<(ostream& it, ListaSadrzajaSekcije t) {
	list <SadrzajSekcije> ::iterator iter;
	it << "Sadrzaj sekcija" << endl;
	for (iter = t.lista.begin(); iter != t.lista.end(); ++iter) { //bilo je ++iter
		it << (*iter) << endl;
	}
	//SadrzajSekcije::glob_brojac = 0;
	return it;

}

string ListaSadrzajaSekcije::dohvatiKod(string sekcija, int ofset) {
	list<SadrzajSekcije>::iterator i;
	for (i = lista.begin(); i != lista.end(); i++) {
		if ((*i).dohvatiNaziv() == sekcija) {
			return (*i).nadjiKod(ofset);
		}
	}
	return 0;
}

void ListaSadrzajaSekcije::ispisiSveSadrzaje(ofstream& fajl) {
	list<SadrzajSekcije>::iterator i;
	fajl<<"Sadrzaj sekcija" << endl;
	for (i = lista.begin(); i != lista.end(); i++) {
		(*i).ispisSadrzaja(fajl);
		fajl << "\n";
	}
}

void ListaSadrzajaSekcije::izmeniKod(string sekcija, int ofset, string kodd) {
	list<SadrzajSekcije>::iterator i;
	for (i = lista.begin(); i != lista.end(); i++) {
		if ((*i).dohvatiNaziv() == sekcija) {
			return (*i).izmeniKod(ofset, kodd);
		}
	}
}

int ListaSadrzajaSekcije::duzina() {
	return lista.size();
}

SadrzajSekcije ListaSadrzajaSekcije::dohvatiSadrzaj(int br) {
	int brojac = 0;
	list<SadrzajSekcije>::iterator i;
	for (i = lista.begin(); i != lista.end(); i++) {
		if (brojac == br) {
			return (*i);
		}
		else {
			brojac++;
		}
	}
	return (*i);
}

SadrzajSekcije ListaSadrzajaSekcije::dohvatiSadrzajPoNazivuSekcije(string naziv) {
	int brojac = 0;
	list<SadrzajSekcije>::iterator i;
	for (i = lista.begin(); i != lista.end(); i++) {
		if ((*i).dohvatiNaziv() == naziv) {
			return (*i);
		}
	}
	return (*i);
}