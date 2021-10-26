#include "KodSekcije.h"
#include <iterator>
KodSekcije::KodSekcije(string ime)
{
	this->imeSek = ime;
}

string KodSekcije::getImeSek() const
{
	return this->imeSek;
}

void KodSekcije::setImeSek(string ime)
{
	this->imeSek = ime;
}

void KodSekcije::dodajuKod(string s)
{
	kod+=s;
}

KodSekcije::~KodSekcije()
{
}

ostream& operator<<(ostream& it, KodSekcije t)
{
	it << t.imeSek<< endl;
		
	it << t.kod;

	return it;
}


KodSekcije::KodSekcije(string ime, int i) {
	this->imeSek = ime;
	this->indikator = i;
}