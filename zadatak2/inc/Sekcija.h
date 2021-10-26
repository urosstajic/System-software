#pragma once
#include <iostream>
#include <string>
using namespace std;

class Sekcija
{
public:
	string ime;
	string size;
	int rb=0; //iz tabele simbola
	string pocetnaAdresa="#";
	string krajnjaAdresa;
	
	Sekcija(string i, string s);
	Sekcija(string i, string s, int r, string p,string k);
	void promeniVel(int n); //heksa vrednost
	friend ostream& operator<<(ostream& it, const Sekcija& s);
};

