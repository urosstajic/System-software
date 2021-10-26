#include "Sekcija.h"

Sekcija::Sekcija(string i, string s)
{

	ime = i;
	size = s;
}

Sekcija::Sekcija(string i, string s, int r,string p,string k)
{
	ime = i;
	size = s;
	rb = r;
	pocetnaAdresa = p;
	krajnjaAdresa = k;
}


ostream& operator<<(ostream& it, const Sekcija& s)
{
	return it << s.ime << "\t" << s.size << "\t" << s.rb << "\t" << s.pocetnaAdresa << "\t" << s.krajnjaAdresa;
}

void Sekcija::promeniVel(int n)
{
	int vel = stoi(this->size, nullptr, 16);
	vel += n;
	string s5;
	char hex_string[20];

	sprintf(hex_string, "%X", vel); //convert number to hex
	s5 = hex_string;
	
	string s6;
	char hex_stringg[20];

	sprintf(hex_stringg, "%X", vel-1); //convert number to hex
	s6 = hex_stringg;
	

	this->size = s5;
	this->krajnjaAdresa = s6;

}
