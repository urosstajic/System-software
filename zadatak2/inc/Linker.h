#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include "TabelaSekcija.h"
#include "ListaSadrzajaSekcije.h"
#include "RelokacioniZapis.h"
#include "TabelaRelokacionihZapisa.h"
#include "TabelaSimbola.h"
#include "Lista.h"
#include "PocetnaAdresaSekcija.h"


using namespace std;
class Linker
{
private:
	static TabelaSimbola tab_simbola;
	static TabelaSekcija tab_sekcija;
	static list<Lista> po_redu_sekcije;
	static TabelaRelokacionihZapisa rel_zapisi;
	static ListaSadrzajaSekcije sadrzaji_sek;
	static ListaSadrzajaSekcije po_redu_sadrzaji;
	static int brojac_tren_fajl;
	static int drugi_brojac_tren_fajla;
	list<RelokacioniZapis> pamcenje_rel_zapisa;
	list<Simb> lista_nerazresenih;
	static TabelaSekcija pomocna_tabela_sekcija;
	static list<int> koliko_rel_zapisa_ima_u_jednom_fajlu;
	SadrzajSekcije za_ispis_sadrzaj=SadrzajSekcije("za_ispis");
public:
	void linker(ifstream& fajl);
	int nadjiPocetnuAdresu(string naziv);
	void napraviTabeluSekcija(ifstream& fajl);
	void ispis(string ime);
	void linkable(ifstream& fajl);
	void ispisZaLinkable(string ime_fajla);
	void hex(ifstream& fajl);
	int nadjiPocAdrZaKodove(string naziv);
	string decToHexa(int n);
	int nadjiPocetnuAdrZaLinkable(string naziv);
	int nadjiPocetnuAdrZaHex(string naziv);
	int pokusaj(string naziv);
	void izbaciIzListe(string ime_simb);
	bool razreseniSviSaImenom(string ime_simb, RelokacioniZapis r);
	void napraviTabeluSekcijaZaHex(ifstream& fajl);
	void poredjajSekcije(list<PocetnaAdresaSekcija> l);
	int nadjiTrenVelicinu(string sekcija);
	void srediSadrzaj();
	string decToHexaZaNegativne(int br);
	void ispisiListuSadrzaja(ListaSadrzajaSekcije li, ofstream& fajl);
	void proveriDaLiTrebaDaSeBriseRelZapis();
};

