#include "RelZapis.h"

RelZapis::RelZapis(string s, int o, string t, int r, int a, bool b)
{
	this->sekcija = s;
	this->offset = o;
	this->tipRealokacije = t;
	this->rb = r;
	this->addend = a;
	this->instrukcija = b;
}

RelZapis::RelZapis(string s, int o, string t, int r, int a, int ii, bool b)
{
	this->sekcija = s;
	this->offset = o;
	this->tipRealokacije = t;
	this->rb = r;
	this->addend = a;
	i = ii;
	this->instrukcija = b;
}

RelZapis::RelZapis(string s, int o, string t, int r, int a)
{
	this->sekcija = s;
	this->offset = o;
	this->tipRealokacije = t;
	this->rb = r;
	this->addend = a;
	
}

RelZapis::RelZapis(string s, int o, string t, int r, int a, int ii)
{
	this->sekcija = s;
	this->offset = o;
	this->tipRealokacije = t;
	this->rb = r;
	this->addend = a;
	i = ii;
	
}

int RelZapis::getOffset() const
{
	return this->offset;
}

string RelZapis::getSekcija() const
{
	return this->sekcija;
}

string RelZapis::getTipRealokacije() const
{
	return this->tipRealokacije;
}

int RelZapis::getRB() const
{
	return this->rb;
}

int RelZapis::getAddend() const
{
	return this->addend;
}

void RelZapis::setRB(int r)
{
	this->rb = r;
}

void RelZapis::setAddend(int local)
{
	this->addend = local;
}

void RelZapis::setOffset(int offset)
{
	this->offset = offset;
}

void RelZapis::setSekcija(string sek)
{
	this->sekcija = sek;
}

void RelZapis::setTipRealokacije(string t)
{
	this->tipRealokacije = t;
}



ostream& operator<<(ostream& it, const RelZapis& s)
{
	return it << s.sekcija << "\t" << s.offset << "\t" << s.tipRealokacije << "\t" << s.rb << "\t" << s.addend;
}


RelZapis::~RelZapis()
{
}