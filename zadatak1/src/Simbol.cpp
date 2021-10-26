#include "Simbol.h"
int Simbol::posrb = 0;

Simbol::Simbol(string labela, string sekcija, int offset, bool local)
{
	this->labela = labela;
	this->sekcija = sekcija;
	this->offset = offset;
	this->local = local;
}

Simbol::Simbol(string labela, string sekcija, int offset, bool local, int rrr)
{
	this->labela = labela;
	this->sekcija = sekcija;
	this->offset = offset;
	this->local = local;
	posrb--;
	rb = rrr;
}

void Simbol::setLocal(bool local)
{
	this->local = local;
}

void Simbol::setOffset(int offset)
{
	this->offset = offset;
}

void Simbol::setSekcija(string sek)
{
	this->sekcija = sek;
}

void Simbol::setLabela(string labela)
{
	this->labela = labela;
}

bool Simbol::getLocal() const
{
	return this->local;
}

int Simbol::getOffset() const
{
	return this->offset;
}

string Simbol::getSekcija() const
{
	return this->sekcija;
}
string Simbol::getLabela() const
{
	return this->labela;
}

int Simbol::getRB() const
{
	return this->rb;
}

Simbol::~Simbol()
{
}


ostream& operator<<(ostream& it, const Simbol& s)
{
	return it << s.getLabela()<< "\t" << s.getOffset() << "\t" << s.getSekcija() << "\t" << (s.getLocal() ? "local" : "global") << "\t" << s.getRB();
}

/*bool operator!=(Simbol s1,Simbol s2)
{
	
	return s2.rb!=s1.rb;
}
*/
