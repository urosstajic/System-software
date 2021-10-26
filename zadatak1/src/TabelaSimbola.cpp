#include "TabelaSimbola.h"
void TabelaSimbola::dodajSimb(Simb s) {
	this->tabelasimbola.push_back(s);
}

bool TabelaSimbola::da_li_postoji_simbol(string naziv) {
	list<Simb>::iterator i;
	for (i = this->tabelasimbola.begin(); i != this->tabelasimbola.end(); i++) {
		if ((*i).getLabel() == naziv) {
			return true;
		}
	}
	return false;
}

ostream& operator<<(ostream& it, TabelaSimbola t)
{
	list <Simb> ::iterator iter;
	it << "Tabela simbola" << endl;
	//it << "Naziv simbola" << "\t" << "OFSET" << "\t" << "Naziv sekcije" << "\t" << "local/global" << "\t" << "Redni broj"<<"\n";
	for (iter = t.tabelasimbola.begin(); iter != t.tabelasimbola.end(); ++iter) {
		it << (*iter) << "\n"; //ne iterira dobro
	}

	return it;
}

void TabelaSimbola::simbolGlobalan(string ime) {
	list <Simb> ::iterator i;
	for (i = this->tabelasimbola.begin(); i != tabelasimbola.end(); i++) {
		if ((*i).getLabel() == ime) {
			(*i).simbolGlobalan();
			break;
		}
	}
}

Simb TabelaSimbola::dohvatiSimbol(string ime) {
	list <Simb> ::iterator i;
	for (i = this->tabelasimbola.begin(); i != tabelasimbola.end(); i++) {
		if ((*i).getLabel() == ime) {
			return (*i);
		}
	}
	return (*i);
}

Simb& TabelaSimbola::dohvatiSimbolZaMenjanje(string ime) {
	list <Simb> ::iterator i;
	for (i = this->tabelasimbola.begin(); i != tabelasimbola.end(); i++) {
		if ((*i).getLabel() == ime) {
			return (*i);
		}
	}
	return (*i);
}

int TabelaSimbola::dohvatiDuzinu() {
	if (tabelasimbola.empty())return 0;
	else return this->tabelasimbola.size();
}

Simb TabelaSimbola::nadjiSimbPoImenuISekciji(string ime, string sek) {
	list<Simb>::iterator i;
	for (i = tabelasimbola.begin(); i != tabelasimbola.end(); i++) {
		if ((*i).getLabel() == ime && (*i).dohvatiNazivSekcije()==sek) {
			return (*i);
		}
	}
	return Simb("greska", -10, -10, "greska");
}

Simb TabelaSimbola::dohvatiSimbPoRbr(int br) {
	list<Simb>::iterator i;
	for (i = tabelasimbola.begin(); i != tabelasimbola.end(); i++) {
		if ((*i).dohvatiRbr()==br) {
			return (*i);
		}
	}
	return Simb("greska", -10, -10, "greska");
}