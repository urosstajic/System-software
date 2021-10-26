#pragma once
#include<iostream>
#include <fstream>
#include <string>
#include <list>
#include "TabelaSimbola.h"
#include <iterator>
#include "KodSekcije.h"
#include "RelTabela.h"
#include "TabelaSekcija.h"
#include "OffsetSekcije.h"
#include "StariRB.h"
#include "Place.h"
#include "PrepravkaRelZapisa.h"
using namespace std;
class Linker
{
public:
	TabelaSimbola tabelasimbola;
	TabelaSekcija tabelasekcija;
	RelTabela relokacionatabela;
	list<KodSekcije> kodovi;
	list<OffsetSekcije> offsetisek;
	list<StariRB> stariredbrojevi;
	KodSekcije& dohvKodSekcije(string ime);
	string dohvKodKodaSekcije(string ime);
	bool prvi = true; // da li je prvi fajl
	bool tabelasek = false;
	bool tabelasimb = false;
	bool kodovi1 = false;
	bool relokacioni = false;
	void linkable(list<string> lista,string smesti);
	void obradaLinkable(ifstream& ulaz);
	void obradaPrvi(ifstream& ulaz);
	void obradaNePrvi(ifstream& ulaz);
	void ispisiKodove();
	void dodajuKodove(KodSekcije s);
	void dodajuOffseteSek(OffsetSekcije sek);
	void dodajuStareRBbrojeve(StariRB r);
	int postojiOffsetSek(string ime);
	int postojiKaoStari(int stari);
	OffsetSekcije& dohvOffsetSek(string ime);
	bool noviBrojDaLiJeLokalniIliEqu(int rbb);
	int noviOffsetIzTabeleSim(int rbb);
	void setujKodSekcije(string ime, string kodic);
	int brojacRelZapisaIzPrethodne=0;
	int globalnibrojaczasimbole = 0;
	int lokalnibrojaczasimbole = 0;
	bool prvifajllinkable = true; 
	//ovo nam sluzi da se u linkable rel zapisi menjaju samo jednom iz prvog fajla
	////////////////za hex
	void hex(list<string> ofajlovi, list<string> place, string smesti);
	list<Place> place;

	ifstream* ulaznifajlovi; ///////////////////////
	int ulaznifajlVelicina = 0;
    void obradiHEX();
	TabelaSekcija hextabelasekcija;
	TabelaSimbola hextabelasimbola;
	string saberi(string s1, string s2);
	string oduzmi(string s1, string s2);
	bool jednako(string s1, string s2);
	string postojiUPlace(string ime);
	bool uporediVece(string s1, string s2); //vraca true ako je vece
	bool uporediManje(string s1, string s2);
	list<OffsetSekcije> offsetisekZASVE;
	RelTabela svirelzapisi;
	int postojiOffsetSekZASVE(string ime);
	list<string> undefinedlabel;
	int postojiOffsetSekIII(string ime, int iii);
	list<PrepravkaRelZapisa> prepravke;
	bool postojiKodSekcije(string ime);
	void ispisiKodoveHEX();
	void preuredikodoveHEX();
	list<KodSekcije> kodoviHEX;
	void ispisiKodoveHEXPravilno(ofstream* izlazni);
	string dohvPocetnuAdresuZaKodSekcije(string ime);
	bool uporediVeceJednako(string s1, string s2);
	
};

