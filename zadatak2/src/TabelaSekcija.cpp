#include "TabelaSekcija.h"
#include <iostream>

using namespace std;
void TabelaSekcija::dodajSekciju(Sekcija s) {
	this->tabela_sekcija.push_back(s);
}

bool TabelaSekcija::da_li_postoji_sekcija(string naziv) {
	list<Sekcija>::iterator i;
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			//vec postoji u sekciji element
			return true;
		}
	}
	return false;
}

Sekcija TabelaSekcija::nadjiSekciju(string n) {
	list<Sekcija>::iterator i;
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getNaziv() == n) {
			//cout << "naslo sekciju" << endl;
			return (*i);
		}
	}
	return Sekcija("ne_postoji");
}

Sekcija TabelaSekcija::nadjiSekcijuPoRBr(int br) {
	list<Sekcija>::iterator i;
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getRbr() == br) {
			return (*i);
		}
	}
	return (*i);
}

int TabelaSekcija::duzina() {
	return this->tabela_sekcija.size();
}

int TabelaSekcija::pocetnaAdresa(string naziv) {
	int vrati = 0;
	list<Sekcija>::iterator i;
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			break;
		}
		else {
			vrati += (*i).getVelicina();
		}
	}

	return vrati;
}

ostream& operator<<(ostream& it, TabelaSekcija t) {
	list <Sekcija> ::iterator iter;
	it << "Tabela sekcija" << endl;
	//it << "Naziv simbola" << "\t" << "OFSET" << "\t" << "Naziv sekcije" << "\t" << "local/global" << "\t" << "Redni broj"<<"\n";
	for (iter = t.tabela_sekcija.begin(); iter != t.tabela_sekcija.end(); ++iter) {
		it << (*iter) << "\n"; //ne iterira dobro
	}

	return it;
}

Sekcija& TabelaSekcija::nadjiSekcijuZaPromenuVelicine(int br) {
	list<Sekcija>::iterator i;
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getRbr() == br) {
			return (*i);
		}
	}
	return (*i);
}

int TabelaSekcija::dohvatiVelicinuCeleSekcije(string naziv) {
	list<Sekcija>::iterator i;
	int pom = 0;
	if (nadjiSekciju(naziv).getNaziv() == "ne_postoji") {
		return 0;
	}
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			return pom;
		}
		else {
			pom += (*i).getVelicina();
		}
	}
	return pom;
}

int TabelaSekcija::nadjiSveVelPreMene(string naziv) {
	list<Sekcija>::iterator i;
	int pom = 0;
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			return pom;
		}
		else {
			pom += (*i).getVelicina();
		}
	}
	return pom;
}

int TabelaSekcija::nadjiElemPreMene(string naziv) {
	list<Sekcija>::iterator i;
	if (tabela_sekcija.empty()) {
		return 0;
	}
	list<Sekcija>::iterator preth=tabela_sekcija.begin();
	int vrati = 0;
	for (i = tabela_sekcija.begin(); i != tabela_sekcija.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			break;
		}
		else {
			preth = i;
		}
	}
	
	return (*preth).dohvatiPocetnuAdresu() + (*preth).getVelicina();
}

void TabelaSekcija::izbaciSve() {
	tabela_sekcija.clear();
}
