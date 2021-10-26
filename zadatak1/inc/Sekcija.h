#pragma once
#include <string>

using namespace std;
class Sekcija
{
private:
	string naziv;
	//int offset;
	//string sadrzaj, objasnjenje;
	static int pom_rbr;
	int velicina=-1;
	int rbr;
	int pocetna_adresa = -1;
public:
	Sekcija(string n);
	Sekcija(string n, int r, int ve);
	string getNaziv();
	int getRbr();
	void postaviVelicinu(int v);
	friend ostream& operator<<(ostream& it, const Sekcija& s);
	int getVelicina();
	void postaviPocetnuAdresu(int p);
	int dohvatiPocetnuAdresu();
};

