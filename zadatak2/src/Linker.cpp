#define _CRT_SECURE_NO_WARNINGS
#include "Linker.h"
#include "Lista.h"
#include <sstream>
#include <iostream>
#include "PocetnaAdresaSekcija.h"
#include "Main.h"
#include <stdio.h>

using namespace std;

TabelaSekcija Linker::tab_sekcija;
TabelaSimbola Linker::tab_simbola;
list<Lista> Linker::po_redu_sekcije;
TabelaRelokacionihZapisa Linker::rel_zapisi;
ListaSadrzajaSekcije Linker::sadrzaji_sek;
int Linker::brojac_tren_fajl = 1;
TabelaSekcija Linker::pomocna_tabela_sekcija;
int Linker::drugi_brojac_tren_fajla = 1;
list<int> Linker::koliko_rel_zapisa_ima_u_jednom_fajlu;
ListaSadrzajaSekcije Linker::po_redu_sadrzaji;

//g++ -o prog Main.cpp Asembler.cpp Linker.cpp Simb.cpp Sekcija.cpp PocetnaAdresaSekcija.cpp ListaSadrzajaSekcije.cpp Lista.cpp RelokacioniZapis.cpp TabelaSimbola.cpp TabelaSekcija.cpp SadrzajSekcije.cpp TabelaRelokacionihZapisa.cpp

void Linker::linker(ifstream& fajl) {
	string red;
	bool sekcija = false, simbol = false, relok=false, sadrzaj=false;
	list<Sekcija> sekcije_ovog_elem_liste;
	while (getline(fajl, red)) {
		if (red == "") {
			continue;
		}
		else if (red == "Tabela sekcija") {
			//cout << "Uslo ovde";
			sekcija = true;
			simbol = false;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela simbola") {
			//cout << "Udje u tab simb";
			sekcija = false;
			simbol = true;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela relokacionih zapisa") {
			relok = true;
			sekcija = false;
			simbol = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Sadrzaj sekcija") {
			relok = false;
			sekcija = false;
			simbol = false;
			sadrzaj = true;
			continue;
		}
		if (sekcija == true) {
			//cout << "I sad ovde";
			/*istringstream pomoc(red);
			string sekcija, rbr, vel;
			pomoc >> sekcija;
			pomoc >> rbr;
			pomoc >> vel; 
			int pom_br = 0;
			if (!tab_sekcija.da_li_postoji_sekcija(sekcija)) {
				if (tab_sekcija.duzina() != 0) {
					pom_br = tab_sekcija.duzina() + 1;
				}
			}
			else {
				pom_br = tab_sekcija.nadjiSekciju(sekcija).getRbr();
			}
			
			sekcije_ovog_elem_liste.push_back(Sekcija(sekcija, pom_br, stoi(vel)));
			if (!tab_sekcija.da_li_postoji_sekcija(sekcija)) {
				int broj = 0;
				if (tab_sekcija.duzina() != 0) {
					int broj = tab_sekcija.duzina() + 1;
				}
				Sekcija s = Sekcija(sekcija, broj, stoi(vel));
				tab_sekcija.dodajSekciju(s);
			}
			else {
				tab_sekcija.nadjiSekciju(sekcija).postaviVelicinu(tab_sekcija.nadjiSekciju(sekcija).getVelicina() + stoi(vel));
			}*/
			//cout << sekcije_ovog_elem_liste.size();
		}
		else if (simbol == true) {
			istringstream pomoc(red);
			string naziv, ofset, sekcija, loc_glob, rbr;
			pomoc >> naziv;
			pomoc >> ofset;
			pomoc >> sekcija;
			pomoc >> loc_glob;
			pomoc >> rbr;
			int pom = stoi(ofset);

			if (tab_simbola.da_li_postoji_simbol(naziv)) {
				//ovo moram da proverim
				Simb& tren_simb = tab_simbola.dohvatiSimbolZaMenjanje(naziv);
				if (tren_simb.dohvatiLocGlob() == "global" && loc_glob == "global" && sekcija!="UND") {
					cout << "Simbol je visestruko definisan "<<naziv;
					exit(1);
				}
				int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
				
				if (sekcija != "UND") {
					int poc_adr = nadjiPocetnuAdresu(sekcija);
					Simb s = Simb(naziv, pom + poc_adr, sek, loc_glob, tab_simbola.dohvatiDuzinu());
					s.postaviNazivSekcije(sekcija);
					tab_simbola.dodajSimb(s);
					//tab_simbola.dohvatiSimbolZaMenjanje(naziv).postaviNazivSekcije(sekcija);
				}
			}
			else {
				int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
				
				if (sekcija != "UND") {
					int poc_adr = nadjiPocetnuAdresu(sekcija);	
					Simb s = Simb(naziv, pom + poc_adr, sek, loc_glob, tab_simbola.dohvatiDuzinu());
					s.postaviNazivSekcije(sekcija);
					tab_simbola.dodajSimb(s);
					//tab_simbola.dohvatiSimbolZaMenjanje(naziv).postaviNazivSekcije(sekcija);
				}
				
			}
		}
		else if (relok == true) {
			istringstream pomoc(red);
			string sekcija, ofset, tip_relokacije, ime_simb, adend;
			pomoc >> sekcija;//ovo da se ispravi da bude string u ispisu u fajlu
			pomoc >> ofset;
			pomoc >> tip_relokacije;
			pomoc >> ime_simb;
			pomoc >> adend;
			int poc_adr = nadjiPocetnuAdresu(sekcija);
			int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
			//sta da radim sa rednim brojem
			int rbr= tab_simbola.nadjiSimbPoImenuISekciji(ime_simb, sekcija).dohvatiRbr();
			/*if (tab_simbola.nadjiSimbPoImenuISekciji(ime_simb, sekcija).getLabel() != "greska") {
				rbr= tab_simbola.nadjiSimbPoImenuISekciji(ime_simb, sekcija).dohvatiRbr();
			}*/
			RelokacioniZapis r = RelokacioniZapis(sek, stoi(ofset) + poc_adr, tip_relokacije, rbr, stoi(adend));
			r.postaviImeSekcije(sekcija);
			r.postaviImeSimbola(ime_simb);
			rel_zapisi.dodajRelokacioniZapis(r);
			RelokacioniZapis rr = RelokacioniZapis(sek, stoi(ofset) + poc_adr, tip_relokacije, rbr, stoi(adend));
			rr.postaviImeSekcije(sekcija);
			rr.postaviImeSimbola(ime_simb);
			pamcenje_rel_zapisa.push_back(rr);
		}
		else if (sadrzaj == true) {
			istringstream pomoc(red);
			string naziv, kod;
			pomoc >> naziv;
			pomoc >> kod;
			if (sadrzaji_sek.da_li_postoji_sadrzajSekcije(naziv)) {
				SadrzajSekcije& s = sadrzaji_sek.dohvatiSekciju(naziv);
				s.dodajKod(kod);
			}
			else {
				sadrzaji_sek.dodajSadrzajSekcije(SadrzajSekcije(naziv));
				SadrzajSekcije& s = sadrzaji_sek.dohvatiSekciju(naziv);
				s.dodajKod(kod);
			}
			//izmena koda

		}
	}
	//ovde je formiran i kod za tu sekciju i ako uzmem nesto da menjam to ce biti unutar njega, ako dodje neki novi kod u istu sekciju to ce biti unutar tog novog dela da se menja pa mislim da mogu ovde da radim to menjanje
	list<RelokacioniZapis>::iterator iterat;
	cout << "Pamcenje rel zapisa "<<pamcenje_rel_zapisa.size() << endl;
	
	for (iterat = pamcenje_rel_zapisa.begin(); iterat != pamcenje_rel_zapisa.end(); ++iterat) {
		string tip_rel = (*iterat).dohvatiTipRelokacije();
		string ime_sek = (*iterat).dohvatiImeSekcije();
		string ime_simb = (*iterat).dohvatiImeSimbola();
		int sek = (*iterat).dohvatiSekciju();
		int ofset = (*iterat).dohvatiOfset();
		//int poc_adr = nadjiPocetnuAdresu(ime_sek);
		int poc_adr = tab_sekcija.pocetnaAdresa(ime_sek);
		int rbr_simb = (*iterat).dohvatiRbrSimbola();
		//zbog ovog ovde mi baca exception
		string kod=sadrzaji_sek.dohvatiKod(ime_sek, ofset-poc_adr);
		int vidi = ofset - poc_adr;
		//cout<<endl << "Ovo je vidi " + vidi << endl;
		int bla;
		if (tip_rel == "R_X86_64_PC16") {
			char* c = const_cast<char*>(kod.c_str());
			int vred_int = (int)strtol(c, nullptr, 16);
			int o = tab_simbola.dohvatiSimbPoRbr(rbr_simb).dohvatiOfset();
			int za_menajnje = o + vred_int-ofset;
			string heksa = decToHexa(za_menajnje);
			string krajnje = "";
			if (heksa.length() < 4) {
				int nes = 4 - heksa.length();
				for (int i = 0; i < nes; i++) {
					krajnje += "0";
				}
				krajnje += heksa;
			}
			else {
				krajnje = heksa;
			}
			sadrzaji_sek.izmeniKod(ime_sek, ofset - poc_adr, krajnje);
		}
		else if (tip_rel == "R_X86_64_16") {
			char* c = const_cast<char*>(kod.c_str());
			int vred_int = (int)strtol(c, nullptr, 16);
			int o = tab_simbola.dohvatiSimbPoRbr(rbr_simb).dohvatiOfset();
			int za_menajnje = o + vred_int;
			string heksa = decToHexa(za_menajnje);
			string krajnje = "";
			if (heksa.length() < 4) {
				int nes = 4 - heksa.length();
				for (int i = 0; i < nes; i++) {
					krajnje += "0";
				}
				krajnje += heksa;
			}
			else {
				krajnje = heksa;
			}
			sadrzaji_sek.izmeniKod(ime_sek, ofset - poc_adr, krajnje);
		}
		else if (tip_rel == "R_X86_64noninst_16") {
			kod = kod.substr(2,4)+kod.substr(0, 2);
			char* c = const_cast<char*>(kod.c_str());
			int vred_int = (int)strtol(c, nullptr, 16);
			int o = tab_simbola.dohvatiSimbPoRbr(rbr_simb).dohvatiOfset();
			int za_menajnje = o + vred_int;
			string heksa = decToHexa(za_menajnje);
			string krajnje = "";
			if (heksa.length() < 4) {
				int nes = 4 - heksa.length();
				for (int i = 0; i < nes; i++) {
					krajnje += "0";
				}
				krajnje += heksa;
			}
			else {
				krajnje = heksa;
			}
			krajnje = krajnje.substr(2, 4) + krajnje.substr(0, 2);
			sadrzaji_sek.izmeniKod(ime_sek, ofset - poc_adr, krajnje);
		}

	}
	pamcenje_rel_zapisa.clear();
	
	
	
	/*cout << tab_sekcija << endl;
	cout << tab_simbola << endl;
	cout << rel_zapisi << endl;
	cout << sadrzaji_sek << endl;*/
	brojac_tren_fajl++;
}

int Linker::nadjiPocetnuAdresu(string naziv) {
	int vrati = tab_sekcija.dohvatiVelicinuCeleSekcije(naziv);
	//int vrati = 0;
	int lok_brojac = 1;
	list<Lista>::iterator i;
	for (i = this->po_redu_sekcije.begin(); i != po_redu_sekcije.end(); i++) {
		if (lok_brojac == brojac_tren_fajl) {
			return vrati;
		}
		else {
			vrati += (*i).nadjiPocetnuAdresu(naziv);
			lok_brojac++;
		}
	}
	return vrati;
}

int Linker::nadjiPocetnuAdrZaHex(string naziv) {
	int vrati = 0;
	list<Lista>::iterator i;
	int lok_brojac = 1;
	for (i = this->po_redu_sekcije.begin(); i != po_redu_sekcije.end(); i++) {
		if (lok_brojac == brojac_tren_fajl) {
			vrati += (*i).nadjiAdrZaHex(naziv);
		}
		else {

		}
	}
	return 0;
}


void Linker::napraviTabeluSekcija(ifstream& fajl) {
	string red;
	bool sekcija = false, simbol = false, relok = false, sadrzaj = false;
	list<Sekcija> sekcije_ovog_elem_liste;
	while (getline(fajl, red)) {
		if (red == "") {
			continue;
		}
		else if (red == "Tabela sekcija") {
			//cout << "Uslo ovde";
			sekcija = true;
			simbol = false;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela simbola") {
			sekcija = false;
		}

		if (sekcija == true) {
			//cout << "I sad ovde";
			istringstream pomoc(red);
			string sekcija, rbr, vel;
			pomoc >> sekcija;
			pomoc >> rbr;
			pomoc >> vel;
			int pom_br = 0;
			if (!tab_sekcija.da_li_postoji_sekcija(sekcija)) {
				if (tab_sekcija.duzina() != 0) {
					pom_br = tab_sekcija.duzina() + 1;
				}
			}
			else {
				pom_br = tab_sekcija.nadjiSekciju(sekcija).getRbr();
			}

			sekcije_ovog_elem_liste.push_back(Sekcija(sekcija, pom_br, stoi(vel)));
			if (!tab_sekcija.da_li_postoji_sekcija(sekcija)) {
				int broj = 0;
				if (tab_sekcija.duzina() != 0) {
					broj = tab_sekcija.duzina();
				}
				Sekcija s = Sekcija(sekcija, broj, stoi(vel));
				tab_sekcija.dodajSekciju(s);
			}
			else {
				int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
				Sekcija& s = tab_sekcija.nadjiSekcijuZaPromenuVelicine(sek);
				s.postaviVelicinu(s.getVelicina() + stoi(vel));
				//tab_sekcija.nadjiSekciju(sekcija).postaviVelicinu(tab_sekcija.nadjiSekciju(sekcija).getVelicina() + stoi(vel));
			}
			//cout << sekcije_ovog_elem_liste.size();
		}
	}
	po_redu_sekcije.push_back(sekcije_ovog_elem_liste);
}

void Linker::ispis(string ime) {
	list<Simb>::iterator it;
	for (it = lista_nerazresenih.begin(); it != lista_nerazresenih.end(); it++) {
		if (tab_simbola.da_li_postoji_simbol((*it).getLabel())) {
			continue;
		}
		else {
			cout << "Nisu razreseni svi simboli";
			exit(1);
		}
	}
	ofstream fajl;
	fajl.open(ime);
	//fajl << tab_sekcija << endl;
	//fajl << tab_simbola << endl;
	//fajl << rel_zapisi << endl;
	//fajl << po_redu_sadrzaji << endl;
	//fajl << endl;
	this->ispisiListuSadrzaja(po_redu_sadrzaji, fajl);
	fajl.close();
}

void Linker::ispisZaLinkable(string ime_fajla) {
	ofstream fajl;
	fajl.open(ime_fajla);
	fajl << tab_sekcija << endl;
	fajl << tab_simbola << endl;
	fajl << rel_zapisi << endl;
	sadrzaji_sek.ispisiSveSadrzaje(fajl);
	fajl.close();
}

void Linker::linkable(ifstream& fajl) {
	string red;
	bool sekcija = false, simbol = false, relok = false, sadrzaj = false;
	list<Sekcija> sekcije_ovog_elem_liste;
	while (getline(fajl, red)) {
		if (red == "") {
			continue;
		}
		else if (red == "Tabela sekcija") {
			//cout << "Uslo ovde";
			sekcija = true;
			simbol = false;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela simbola") {
			//cout << "Udje u tab simb";
			sekcija = false;
			simbol = true;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela relokacionih zapisa") {
			relok = true;
			sekcija = false;
			simbol = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Sadrzaj sekcija") {
			relok = false;
			sekcija = false;
			simbol = false;
			sadrzaj = true;
			continue;
		}
		if (sekcija == true) {
			continue;
		}
		else if (simbol == true) {
			istringstream pomoc(red);
			string naziv, ofset, sekcija, loc_glob, rbr;
			pomoc >> naziv;
			pomoc >> ofset;
			pomoc >> sekcija;
			pomoc >> loc_glob;
			pomoc >> rbr;
			int pom = stoi(ofset);

			if (tab_simbola.da_li_postoji_simbol(naziv)) {
				if (tab_sekcija.da_li_postoji_sekcija(naziv)) {
					continue;	
				}
				else if (tab_simbola.dohvatiSimbol(naziv).dohvatiNazivSekcije() == "UND") {
					int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
					int poc_adr = this->nadjiPocetnuAdrZaLinkable(sekcija);
					int poc_adr_sekc = tab_sekcija.pocetnaAdresa(sekcija);
					Simb& s = tab_simbola.dohvatiSimbolZaMenjanje(naziv);
					s.postaviNazivSekcije(sekcija);
					s.postaviBrSekcija(sek);
					s.postaviOfset(poc_adr+pom);
				}
				else {
					if (sekcija == "UND") {
						continue;
					}
					else {
						cout << "Simbol " << naziv << " je visestruko definisan.";
						exit(1);
					}
					
				}
				
			}
			else {
				if (tab_sekcija.da_li_postoji_sekcija(naziv)) {//znaci dodajem sekciju
					int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
					Simb s = Simb(naziv, 0, sek, "local", tab_simbola.dohvatiDuzinu());
					s.postaviNazivSekcije(sekcija);
					tab_simbola.dodajSimb(s);
				}
				else {
					int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
					int poc_adr = this->nadjiPocetnuAdrZaLinkable(sekcija);
					int poc_adr_sekc = tab_sekcija.pocetnaAdresa(sekcija);
					Simb s = Simb(naziv, pom + poc_adr, sek, loc_glob, tab_simbola.dohvatiDuzinu());
					s.postaviNazivSekcije(sekcija);
					tab_simbola.dodajSimb(s);
				}
				

			}
		}
		else if (relok == true) {
			istringstream pomoc(red);
			string sekcija, ofset, tip_relokacije, ime_simb, adend;
			pomoc >> sekcija;//ovo da se ispravi da bude string u ispisu u fajlu
			pomoc >> ofset;
			pomoc >> tip_relokacije;
			pomoc >> ime_simb;
			pomoc >> adend;
			int poc_adr = this->nadjiPocetnuAdrZaLinkable(sekcija);
			int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
			//sta da radim sa rednim brojem
			int rbr = tab_simbola.dohvatiSimbol(ime_simb).dohvatiRbr();
			RelokacioniZapis r = RelokacioniZapis(sek, stoi(ofset) + poc_adr, tip_relokacije, rbr, stoi(adend));
			r.postaviImeSekcije(sekcija);
			r.postaviImeSimbola(ime_simb);
			rel_zapisi.dodajRelokacioniZapis(r);
		}
		else if (sadrzaj == true) {
			istringstream pomoc(red);
			string naziv, kod;
			pomoc >> naziv;
			pomoc >> kod;
			if (sadrzaji_sek.da_li_postoji_sadrzajSekcije(naziv)) {
				SadrzajSekcije& s = sadrzaji_sek.dohvatiSekciju(naziv);
				s.dodajKod(kod);
			}
			else {
				sadrzaji_sek.dodajSadrzajSekcije(SadrzajSekcije(naziv));
				SadrzajSekcije& s = sadrzaji_sek.dohvatiSekciju(naziv);
				s.dodajKod(kod);
			}

		}
	}
	brojac_tren_fajl++;
}

void Linker::hex(ifstream& fajl) {
	string red;
	bool sekcija = false, simbol = false, relok = false, sadrzaj = false;
	list<Sekcija> sekcije_ovog_elem_liste;
	int koliko_rel_zapisa = 0;
	while (getline(fajl, red)) {
		if (red == "") {
			continue;
		}
		else if (red == "Tabela sekcija") {
			//cout << "Uslo ovde";
			sekcija = true;
			simbol = false;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela simbola") {
			//cout << "Udje u tab simb";
			sekcija = false;
			simbol = true;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela relokacionih zapisa") {
			relok = true;
			sekcija = false;
			simbol = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Sadrzaj sekcija") {
			relok = false;
			sekcija = false;
			simbol = false;
			sadrzaj = true;
			continue;
		}
		if (sekcija == true) {
			continue;
		}
		else if (simbol == true) {
			istringstream pomoc(red);
			string naziv, ofset, sekcija, loc_glob, rbr;
			pomoc >> naziv;
			pomoc >> ofset;
			pomoc >> sekcija;
			pomoc >> loc_glob;
			pomoc >> rbr;
			int pom = stoi(ofset);
			if (sekcija == "UND" && loc_glob == "global") {
				int poc_adr = nadjiPocetnuAdresu(sekcija);
				Simb s = Simb(naziv, -1, -1, loc_glob, lista_nerazresenih.size());
				s.postaviNazivSekcije("UND");
				lista_nerazresenih.push_back(s);
			}
			if (naziv == sekcija) {//znaci dodaje se sekcija
				if (pomocna_tabela_sekcija.da_li_postoji_sekcija(naziv)) {
					int postavi = this->nadjiTrenVelicinu(naziv);
					pomocna_tabela_sekcija.nadjiSekciju(naziv).postaviVelicinu(postavi);
				}
				else {
					Sekcija s = Sekcija(sekcija);
					s.postaviPocetnuAdresu(tab_sekcija.nadjiSekciju(sekcija).dohvatiPocetnuAdresu());
					s.postaviVelicinu(0);
					pomocna_tabela_sekcija.dodajSekciju(s);
				}
			}
			if (tab_simbola.da_li_postoji_simbol(naziv)) {
				if (tab_sekcija.da_li_postoji_sekcija(naziv)) {
					continue;
				}
				else if (loc_glob == "local") {
					continue;
				}
				else if (sekcija == "UND") {
					continue;
				}
				else {
					cout << "Simbol " << naziv << " je visestruko definisan.";
					exit(1);
				}

			}
			else {
				if (tab_sekcija.da_li_postoji_sekcija(naziv)) {//znaci dodajem sekciju
					int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
					Simb s = Simb(naziv, tab_sekcija.nadjiSekciju(naziv).dohvatiPocetnuAdresu(), sek, "local", tab_simbola.dohvatiDuzinu());
					s.postaviNazivSekcije(sekcija);
					tab_simbola.dodajSimb(s);
				}
				else {
					if (loc_glob == "global" && sekcija!="UND") {
						int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
						int poc_adr = pomocna_tabela_sekcija.nadjiSekciju(sekcija).getVelicina();
						int poc_adr_sekc = tab_sekcija.nadjiSekciju(sekcija).dohvatiPocetnuAdresu();
						Simb s = Simb(naziv, pom + poc_adr_sekc + poc_adr, sek, loc_glob, tab_simbola.dohvatiDuzinu());
						s.postaviNazivSekcije(sekcija);
						tab_simbola.dodajSimb(s);
					}
				}
			}
		}
		else if (relok == true) {
			istringstream pomoc(red);
			string sekcija, ofset, tip_relokacije, ime_simb, adend;
			pomoc >> sekcija;//ovo da se ispravi da bude string u ispisu u fajlu
			pomoc >> ofset;
			pomoc >> tip_relokacije;
			pomoc >> ime_simb;
			pomoc >> adend;
			int poc_adr = this->nadjiTrenVelicinu(sekcija);

			int sek = tab_sekcija.nadjiSekciju(sekcija).getRbr();
			int rbr = tab_simbola.nadjiSimbPoImenuISekciji(ime_simb, sekcija).dohvatiRbr();
			RelokacioniZapis r = RelokacioniZapis(sek, stoi(ofset) + poc_adr, tip_relokacije, rbr, stoi(adend));
			r.postaviImeSekcije(sekcija);
			r.postaviImeSimbola(ime_simb);
			rel_zapisi.dodajRelokacioniZapis(r);
			RelokacioniZapis rr = RelokacioniZapis(sek, stoi(ofset) + poc_adr, tip_relokacije, rbr, stoi(adend));
			rr.postaviImeSekcije(sekcija);
			rr.postaviImeSimbola(ime_simb);
			pamcenje_rel_zapisa.push_back(rr);
			koliko_rel_zapisa++;
		}
		else if (sadrzaj == true) {
			istringstream pomoc(red);
			string naziv, kod;
			pomoc >> naziv;
			pomoc >> kod;
			if (sadrzaji_sek.da_li_postoji_sadrzajSekcije(naziv)) {
				SadrzajSekcije& s = sadrzaji_sek.dohvatiSekciju(naziv);
				s.dodajKod(kod);
			}
			else {
				sadrzaji_sek.dodajSadrzajSekcije(SadrzajSekcije(naziv));
				SadrzajSekcije& s = sadrzaji_sek.dohvatiSekciju(naziv);
				s.dodajKod(kod);
			}
		}
	}
	
	//pamcenje_rel_zapisa.clear();
	brojac_tren_fajl++;
	koliko_rel_zapisa_ima_u_jednom_fajlu.push_back(koliko_rel_zapisa);
}

void Linker::srediSadrzaj() {
	//ovde je formiran i kod za tu sekciju i ako uzmem nesto da menjam to ce biti unutar njega, ako dodje neki novi kod u istu sekciju to ce biti unutar tog novog dela da se menja pa mislim da mogu ovde da radim to menjanje
	list<int>::iterator hehe;
	int koliko_obradjeno=0;
	hehe = koliko_rel_zapisa_ima_u_jednom_fajlu.begin();
	list<RelokacioniZapis>::iterator iterat;
	for (iterat = pamcenje_rel_zapisa.begin(); iterat != pamcenje_rel_zapisa.end(); iterat++) {
		string tip_rel = (*iterat).dohvatiTipRelokacije();
		string ime_sek = (*iterat).dohvatiImeSekcije();
		string ime_simb = (*iterat).dohvatiImeSimbola();
		int sek = (*iterat).dohvatiSekciju();
		int ofset = (*iterat).dohvatiOfset();

		int poc_adr = tab_sekcija.nadjiSekciju(ime_simb).dohvatiPocetnuAdresu();
		int rbr_simb;
		//zbog ovog ovde mi baca exception
		string kod = sadrzaji_sek.dohvatiKod(ime_sek, ofset);
		bool ima = tab_simbola.da_li_postoji_simbol(ime_simb);
		if (tab_simbola.da_li_postoji_simbol(ime_simb) && !tab_sekcija.da_li_postoji_sekcija(ime_simb)) {
			rbr_simb = tab_simbola.dohvatiSimbol(ime_simb).dohvatiRbr();
			if (tip_rel == "R_X86_64_PC16") {
				char* c = const_cast<char*>(kod.c_str());
				int vred_int = (int)strtol(c, nullptr, 16);
				int o = tab_simbola.dohvatiSimbPoRbr(rbr_simb).dohvatiOfset();
				int poc_sekcije = tab_sekcija.nadjiSekciju(ime_sek).dohvatiPocetnuAdresu();
				int za_menajnje = o + vred_int - ofset-poc_sekcije-2;
				string heksa = decToHexaZaNegativne(za_menajnje);
				string krajnje = "";
				if (heksa.length() < 4) {
					int nes = 4 - heksa.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += heksa;
				}
				else {
					krajnje = heksa;
				}
				sadrzaji_sek.izmeniKod(ime_sek, ofset, krajnje);
			}
			else if (tip_rel == "R_X86_64_16") {
				char* c = const_cast<char*>(kod.c_str());
				int vred_int = (int)strtol(c, nullptr, 16);
				int o = tab_simbola.dohvatiSimbPoRbr(rbr_simb).dohvatiOfset();
				int za_menajnje = o + vred_int;
				string heksa = decToHexa(za_menajnje);
				string krajnje = "";
				if (heksa.length() < 4) {
					int nes = 4 - heksa.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += heksa;
				}
				else {
					krajnje = heksa;
				}
				sadrzaji_sek.izmeniKod(ime_sek, ofset, krajnje);
			}
			else if (tip_rel == "R_X86_64noninst_16") {
				kod = kod.substr(2, 4) + kod.substr(0, 2);
				char* c = const_cast<char*>(kod.c_str());
				int vred_int = (int)strtol(c, nullptr, 16);
				int o = tab_simbola.dohvatiSimbPoRbr(rbr_simb).dohvatiOfset();
				int za_menajnje = o + vred_int;
				string heksa = decToHexa(za_menajnje);
				string krajnje = "";
				if (heksa.length() < 4) {
					int nes = 4 - heksa.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += heksa;
				}
				else {
					krajnje = heksa;
				}
				krajnje = krajnje.substr(2, 4) + krajnje.substr(0, 2);
				sadrzaji_sek.izmeniKod(ime_sek, ofset, krajnje);
			}
		}
		else {//onda u odnosu na sekciju racunam za rel zapis
			if (tip_rel == "R_X86_64_PC16") {
				char* c = const_cast<char*>(kod.c_str());
				int vred_int = (int)strtol(c, nullptr, 16);
				int stv_poc_sekc = pokusaj(ime_simb);
				int poc_sekcije = tab_sekcija.nadjiSekciju(ime_sek).dohvatiPocetnuAdresu();
				int za_menjanje = vred_int + poc_adr - ofset-poc_sekcije-2;
				string heksa = decToHexaZaNegativne(za_menjanje);
				string krajnje = "";
				if (heksa.length() < 4) {
					int nes = 4 - heksa.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += heksa;
				}
				else {
					krajnje = heksa;
				}
				sadrzaji_sek.izmeniKod(ime_sek, ofset, krajnje);
			}
			else if (tip_rel == "R_X86_64_16") {
				char* c = const_cast<char*>(kod.c_str());
				int vred_int = (int)strtol(c, nullptr, 16);
				int stv_poc_sekc = pokusaj(ime_simb);
				int za_menjanje = vred_int + poc_adr;
				string heksa = decToHexa(za_menjanje);
				string krajnje = "";
				if (heksa.length() < 4) {
					int nes = 4 - heksa.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += heksa;
				}
				else {
					krajnje = heksa;
				}
				sadrzaji_sek.izmeniKod(ime_sek, ofset, krajnje);
			}
			else if (tip_rel == "R_X86_64noninst_16") {
				kod = kod.substr(2, 4) + kod.substr(0, 2);
				char* c = const_cast<char*>(kod.c_str());
				int vred_int = (int)strtol(c, nullptr, 16);
				//int o = tab_simbola.dohvatiSimbPoRbr(rbr_simb).dohvatiOfset();
				int za_menajnje = poc_adr + vred_int;
				string heksa = decToHexa(za_menajnje);
				string krajnje = "";
				if (heksa.length() < 4) {
					int nes = 4 - heksa.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += heksa;
				}
				else {
					krajnje = heksa;
				}
				krajnje = krajnje.substr(2, 4) + krajnje.substr(0, 2);
				sadrzaji_sek.izmeniKod(ime_sek, ofset, krajnje);
			}
		}
		koliko_obradjeno++;
		if (koliko_obradjeno == (*hehe)) {
			drugi_brojac_tren_fajla++;
			list<int>::iterator nadji_sl;
			if (hehe == koliko_rel_zapisa_ima_u_jednom_fajlu.end()) {
				break;
			}
			for (nadji_sl = koliko_rel_zapisa_ima_u_jednom_fajlu.begin(); nadji_sl != koliko_rel_zapisa_ima_u_jednom_fajlu.end(); nadji_sl++) {
				if (nadji_sl == hehe) {
					nadji_sl++; 
					/*if (nadji_sl == koliko_rel_zapisa_ima_u_jednom_fajlu.end()) {
						break;
					}*/
					list<int>::iterator novi=nadji_sl;
					hehe =novi;
					koliko_obradjeno = 0;
					break;
				
				}
			}
		}
	}

	for (int s = 0; s < tab_sekcija.duzina(); s++) {
		SadrzajSekcije m = sadrzaji_sek.dohvatiSadrzajPoNazivuSekcije(tab_sekcija.nadjiSekcijuPoRBr(s).getNaziv());
		po_redu_sadrzaji.dodajSadrzajSekcije(m);
	}
	for (int m = 0; m < po_redu_sadrzaji.duzina(); m++) {
		SadrzajSekcije s=po_redu_sadrzaji.dohvatiSadrzaj(m);
		za_ispis_sadrzaj.dodajKod(s.dohvatiKod());
	}
	
}

string Linker::decToHexa(int n)
{
	if (n == 0) {
		return "0";
	}
	char hexaDeciNum[100];
	int i = 0;
	while (n != 0) {
		int temp = 0;
		temp = n % 16;
		if (temp < 10) {
			hexaDeciNum[i] = temp + 48;
			i++;
		}
		else {
			hexaDeciNum[i] = temp + 55;
			i++;
		}
		n = n / 16;
	}
	string pom = "";
	for (int j = i - 1; j >= 0; j--) {
		pom += hexaDeciNum[j];
	}

	return pom;
}

int Linker::nadjiPocetnuAdrZaLinkable(string naziv) {
	list<Lista>::iterator i;
	int lok_brojac = 1;
	int vrati = 0;
	for (i = po_redu_sekcije.begin(); i != po_redu_sekcije.end(); i++) {
		if (lok_brojac == brojac_tren_fajl) {
			return vrati;
		}
		else {
			vrati += (*i).nadjiAdrZaLinkable(naziv);
			lok_brojac++;
		}
	}
	return vrati;
}
//ovo je odakle stv pocinje sekcija npr 2.0 i tako to
int Linker::pokusaj(string naziv) {
	int vrati = 0;
	list<Lista>::iterator i;
	vrati += tab_sekcija.nadjiSveVelPreMene(naziv);
	int pomoc = 1;
	for (i = po_redu_sekcije.begin(); i != po_redu_sekcije.end(); i++) {
		if (!pomoc == drugi_brojac_tren_fajla) {
			vrati += (*i).nadjiAdrZaHex(naziv);
			pomoc++;
		}
		else {
			return vrati;
		}
	}
	return vrati;
}

void Linker::izbaciIzListe(string ime_simb) {
	list<Simb>::iterator i;
	for (i = lista_nerazresenih.begin(); i != lista_nerazresenih.end(); i++) {
		if ((*i).getLabel() == ime_simb) {
			lista_nerazresenih.erase(i);
			break;
		}
	}
}

void Linker::napraviTabeluSekcijaZaHex(ifstream& fajl) {
	string red;
	bool sekcija = false, simbol = false, relok = false, sadrzaj = false;
	list<Sekcija> sekcije_ovog_elem_liste;
	while (getline(fajl, red)) {
		if (red == "") {
			continue;
		}
		else if (red == "Tabela sekcija") {
			//cout << "Uslo ovde";
			sekcija = true;
			simbol = false;
			relok = false;
			sadrzaj = false;
			continue;
		}
		else if (red == "Tabela simbola") {
			sekcija = false;
		}

		if (sekcija == true) {
			//cout << "I sad ovde";
			istringstream pomoc(red);
			string sekcija, rbr, vel;
			pomoc >> sekcija;
			pomoc >> rbr;
			pomoc >> vel;
			int pom_br = 0;
			if (!pomocna_tabela_sekcija.da_li_postoji_sekcija(sekcija)) {
				if (pomocna_tabela_sekcija.duzina() != 0) {
					pom_br = pomocna_tabela_sekcija.duzina() + 1;
				}
			}
			else {
				pom_br = pomocna_tabela_sekcija.nadjiSekciju(sekcija).getRbr();
			}

			sekcije_ovog_elem_liste.push_back(Sekcija(sekcija, pom_br, stoi(vel)));
			if (!pomocna_tabela_sekcija.da_li_postoji_sekcija(sekcija)) {
				int broj = 0;
				if (pomocna_tabela_sekcija.duzina() != 0) {
					broj = pomocna_tabela_sekcija.duzina();
				}
				Sekcija s = Sekcija(sekcija, broj, stoi(vel));
				pomocna_tabela_sekcija.dodajSekciju(s);
			}
			else {
				int sek = pomocna_tabela_sekcija.nadjiSekciju(sekcija).getRbr();
				Sekcija& s = pomocna_tabela_sekcija.nadjiSekcijuZaPromenuVelicine(sek);
				s.postaviVelicinu(s.getVelicina() + stoi(vel));
				//tab_sekcija.nadjiSekciju(sekcija).postaviVelicinu(tab_sekcija.nadjiSekciju(sekcija).getVelicina() + stoi(vel));
			}
			
		}
	}
	po_redu_sekcije.push_back(sekcije_ovog_elem_liste);
}


void Linker::poredjajSekcije(list<PocetnaAdresaSekcija> l) {
	list<PocetnaAdresaSekcija>::iterator iter;
	for (iter = l.begin(); iter != l.end(); iter++) {
		string p = (*iter).dohvatiPocAdr();
		char* c = const_cast<char*>(p.c_str());
		int vred_int = (int)strtol(c, nullptr, 16);
		int ve = pomocna_tabela_sekcija.nadjiSekciju((*iter).dohvatiImeSekcije()).getVelicina();
		if (tab_sekcija.duzina() == 0) {

		}
		else {
			if (tab_sekcija.nadjiSekcijuPoRBr(tab_sekcija.duzina() - 1).dohvatiPocetnuAdresu() + tab_sekcija.nadjiSekcijuPoRBr(tab_sekcija.duzina() - 1).getVelicina() > vred_int) {
				cout << "Sekcije se preklapaju";
				exit(1);
			}
		}
		Sekcija s = Sekcija((*iter).dohvatiImeSekcije(), tab_sekcija.duzina(), ve);
		s.postaviPocetnuAdresu(vred_int);
		//s.postaviVelicinu(pomocna_tabela_sekcija.nadjiSekciju((*iter).dohvatiImeSekcije()).getVelicina());
		tab_sekcija.dodajSekciju(s);
	}
	int i = 0;
	//cout << "ovo je pom" << pomocna_tabela_sekcija.duzina();
	while (tab_sekcija.duzina() < pomocna_tabela_sekcija.duzina()) {
		Sekcija pom=pomocna_tabela_sekcija.nadjiSekcijuPoRBr(i);
		if (tab_sekcija.da_li_postoji_sekcija(pom.getNaziv())) {
			i++;
		}
		else {
			Sekcija nova = Sekcija(pom.getNaziv(), tab_sekcija.duzina(), pom.getVelicina());
			//nova.postaviVelicinu(pom.getVelicina());
			nova.postaviPocetnuAdresu(tab_sekcija.nadjiElemPreMene(pom.getNaziv()));
			tab_sekcija.dodajSekciju(nova);
			i = 0;
		}
	}
	pomocna_tabela_sekcija.izbaciSve();
}

int Linker::nadjiTrenVelicinu(string naziv) {
	list<Lista>::iterator it;
	int pom_br = 1;
	int vrati = 0;
	for (it = this->po_redu_sekcije.begin(); it != po_redu_sekcije.end(); it++) {
		if (pom_br == brojac_tren_fajl) {
			return vrati;
		}
		else {
			vrati += (*it).nadjiAdrZaHex(naziv);
			pom_br++;
		}
	}
	return vrati;
}

string Linker::decToHexaZaNegativne(int br) {
	char hex_string[20];
	sprintf(hex_string, "%X", br);
	string nest = hex_string;
	string novi = "";
	if (nest.length() > 4) {
		novi += nest[nest.length() - 4];
		novi += nest[nest.length() - 3];
		novi += nest[nest.length() - 2];
		novi += nest[nest.length() - 1];
	}
	else {
		novi = nest;
	}
	return novi;
}

void Linker::ispisiListuSadrzaja(ListaSadrzajaSekcije li, ofstream& fajl) {
	int koliko_popunjeno = 0;
	int adresa;
	for (int i = 0; i < li.duzina(); i++) {
		if (i == 0) {
			int poc_sekcije = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(i).dohvatiNaziv()).dohvatiPocetnuAdresu();
			int velicina = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(i).dohvatiNaziv()).getVelicina();
			int j = i + 1;
			int koliko_nula = 0;
			int pravi_pocetak = poc_sekcije;
			while (poc_sekcije % 8 != 0) {
				poc_sekcije--;
				koliko_nula++;
			}
			
			string poc_adr_heksa = decToHexa(poc_sekcije);
			string kod = li.dohvatiSadrzaj(i).dohvatiKod();
			adresa = poc_sekcije + 8;
			string krajnje = "";
			if (poc_adr_heksa.length() < 4) {
				int nes = 4 - poc_adr_heksa.length();
				for (int k = 0; k < nes; k++) {
					krajnje += "0";
				}
				krajnje += poc_adr_heksa;
			}
			else {
				krajnje = poc_adr_heksa;
			}
			int brojac = 0;
			fajl << krajnje << " ";
			for (int b = 0; b < koliko_nula; b++) {
				fajl << "00 ";
				brojac+=2;
				koliko_popunjeno++;
			}
			//int brojac = 0;
			for (int m = 0; m < kod.length(); m++) {
				fajl << kod[m];
				
				if ((brojac+1) % 2 == 0) {
					fajl << " ";
					koliko_popunjeno++;
				}
				if ((brojac+1) % 16 == 0) {
					//brojac += 8;
					koliko_popunjeno = 0;
					//if (brojac != kod.length() - 1) {
						if (m != kod.length() - 1) {
							fajl << endl;

							string is = decToHexa(adresa);
							string krajnje = "";
							if (is.length() < 4) {
								int nes = 4 - is.length();
								for (int i = 0; i < nes; i++) {
									krajnje += "0";
								}
								krajnje += is;
							}
							else {
								krajnje = is;
							}
							fajl << krajnje << " ";
							adresa += 8;
						}
						
						//poc_sekcije += 8;
				//	}
					
				}
				brojac++;
			}
			int sledece = i + 1;
			if (sledece == li.duzina()) {
				if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
				if (koliko_popunjeno == 0)break;
				for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
					fajl << "00 ";
				}
				koliko_popunjeno = 0;
				break;
			}
			if (pravi_pocetak + velicina == tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu()) {//nastavlja se pa nema 0

			}
			else {
				int poc_adr_sled = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu();
				int prava_adr_sled = poc_adr_sled;
				while (poc_adr_sled % 8 != 0) {
					poc_adr_sled--;
				}
				if (pravi_pocetak + velicina >= poc_adr_sled && pravi_pocetak + velicina <= prava_adr_sled) {

				}
				else {
					if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
					if (koliko_popunjeno == 0)continue;
					for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
						fajl << "00 ";
					}
					koliko_popunjeno = 0;
				}
			}
		}
		else {
			int preth = i - 1;
			//int koliko_popunjeno = 0;
			int poc_sekc_preth= tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(preth).dohvatiNaziv()).dohvatiPocetnuAdresu();
			int vel_preth= tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(preth).dohvatiNaziv()).getVelicina();
			int poc_sekcije = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(i).dohvatiNaziv()).dohvatiPocetnuAdresu();
			int pravi_pocetak = poc_sekcije;
			int velicina = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(i).dohvatiNaziv()).getVelicina();
			string kod = li.dohvatiSadrzaj(i).dohvatiKod();
			if (poc_sekc_preth + vel_preth == poc_sekcije) {//nastavlja se
				int broj = 0;
				int koliko_uzeto = 0;
				int ispisano = 0;
				if (koliko_popunjeno != 0) {
					koliko_uzeto = 8 - koliko_popunjeno;
					int pravo_koliko_popu = koliko_popunjeno;
					for (int s = 0; s < 8 - pravo_koliko_popu; s++) {
						int doo = ispisano + 2;
						for (int m = ispisano; m < doo && m<kod.length(); m++) {
							fajl << kod[m];
							if ((m + 1) % 2 == 0) {
								fajl << " ";
								koliko_popunjeno++;
								ispisano += 2;
								broj++;
								if (broj == s) {
									//koliko_uzeto = m;
									koliko_popunjeno = 0;
									break;
								}
							}
						}

					}
				}
				if (koliko_uzeto * 2 >= kod.length()) {
					int sledece = i + 1;
					if (sledece == li.duzina()) {
						if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
						if (koliko_popunjeno == 0)break;
						for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
							fajl << "00 ";
						}
						koliko_popunjeno = 0;
						break;
					}
					if (pravi_pocetak + velicina == tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu()) {//nastavlja se pa nema 0

					}
					else {
						int poc_adr_sled = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu();
						int prava_adr_sled = poc_adr_sled;
						while (poc_adr_sled % 8 != 0) {
							poc_adr_sled--;
						}
						if (pravi_pocetak + velicina >= poc_adr_sled && pravi_pocetak + velicina <= prava_adr_sled) {

						}
						else {
							if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
							if (koliko_popunjeno == 0)continue;
							for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
								fajl << "00 ";
							}
							koliko_popunjeno = 0;
						}
					}
					continue;
				}
				fajl << endl;
				string is = decToHexa(adresa);
				string krajnje = "";
				if (is.length() < 4) {
					int nes = 4 - is.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += is;
				}
				else {
					krajnje = is;
				}
				fajl << krajnje << " ";
				adresa += 8;
				int pompom = 0;
				for (int n = koliko_uzeto*2; n < kod.length(); n++) {
					fajl << kod[n];
					if ((pompom + 1) % 2 == 0 ) {
						fajl << " ";
						koliko_popunjeno++;
					}
					if ((pompom + 1) % 16 == 0) {
						//brojac += 8;
						//koliko_popunjeno = 0;
						if (pompom != kod.length() - 1) {
							if (n != kod.length()-1) {
								fajl << endl;
								string is = decToHexa(adresa);
								string krajnje = "";
								if (is.length() < 4) {
									int nes = 4 - is.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += is;
								}
								else {
									krajnje = is;
								}
								fajl << krajnje << " ";
								adresa += 8;
								koliko_popunjeno = 0;
							}
							
							//poc_sekcije += 8;
						}

					}
					pompom++;
				}
				int sledece = i + 1;
				if (sledece >= li.duzina()) {
					if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
					for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
						fajl << "00 ";
					}
					koliko_popunjeno = 0;
					break;
				}
				if (pravi_pocetak + velicina == tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu()) {//nastavlja se pa nema 0

				}
				else {
					int poc_adr_sled = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu();
					int prava_adr_sled = poc_adr_sled;
					while (poc_adr_sled % 8 != 0) {
						poc_adr_sled--;
					}
					if (pravi_pocetak + velicina >= poc_adr_sled && pravi_pocetak + velicina <= prava_adr_sled) {

					}
					else {
						if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
						if (koliko_popunjeno == 0)continue;
						for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
							fajl << "00 ";
						}
						koliko_popunjeno = 0;
					}
				}
			}
			else {

				int koliko_nula = 0;
				while (poc_sekcije % 8 != 0) {
					poc_sekcije--;
					koliko_nula++;
				}
				if (poc_sekc_preth + vel_preth >= poc_sekcije) {
					int pommmm = koliko_popunjeno;
					for (int b = 0; b < koliko_nula-pommmm; b++) {
						fajl << "00 ";
						//brojac += 2;
						koliko_popunjeno++;
					}
					int brojac = koliko_popunjeno * 2;
					if (brojac >= kod.length()) {
						int sledece = i + 1;
						if (sledece == li.duzina()) {
							if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
							if (koliko_popunjeno == 0)break;
							for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
								fajl << "00 ";
							}
							koliko_popunjeno = 0;
							break;
						}
						if (pravi_pocetak + velicina == tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu()) {//nastavlja se pa nema 0

						}
						else {
							int poc_adr_sled = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu();
							int prava_adr_sled = poc_adr_sled;
							while (poc_adr_sled % 8 != 0) {
								poc_adr_sled--;
							}
							if (pravi_pocetak + velicina >= poc_adr_sled && pravi_pocetak + velicina <= prava_adr_sled) {

							}
							else {
								if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
								if (koliko_popunjeno == 0)continue;
								for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
									fajl << "00 ";
								}
								koliko_popunjeno = 0;
							}
						}
						continue;
					}

					for (int m = 0; m < kod.length(); m++) {
						fajl << kod[m];

						if ((brojac + 1) % 2 == 0) {
							fajl << " ";
							koliko_popunjeno++;
						}
						if ((brojac + 1) % 16 == 0) {
							//brojac += 8;
							koliko_popunjeno = 0;
							//if (brojac != kod.length() - 1) {
							if (m != kod.length() - 1) {
								fajl << endl;

								string is = decToHexa(adresa);
								string krajnje = "";
								if (is.length() < 4) {
									int nes = 4 - is.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += is;
								}
								else {
									krajnje = is;
								}
								fajl << krajnje << " ";
								adresa += 8;
							}

							//poc_sekcije += 8;
					//	}

						}
						brojac++;
					}
					int sledece = i + 1;
					if (sledece == li.duzina()) {
						if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
						if (koliko_popunjeno == 0) break;
						for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
							fajl << "00 ";
						}
						koliko_popunjeno = 0;
						break;
					}
					if (pravi_pocetak + velicina == tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu()) {//nastavlja se pa nema 0

					}
					else {
						int poc_adr_sled = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu();
						int prava_adr_sled = poc_adr_sled;
						while (poc_adr_sled % 8 != 0) {
							poc_adr_sled--;
						}
						if (pravi_pocetak + velicina >= poc_adr_sled && pravi_pocetak + velicina <= prava_adr_sled) {

						}
						else {
							if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
							if (koliko_popunjeno == 0)continue;
							for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
								fajl << "00 ";
							}
							koliko_popunjeno = 0;
						}
					}
				}
				else {
					string poc_adr_heksa = decToHexa(poc_sekcije);
					string kod = li.dohvatiSadrzaj(i).dohvatiKod();
					adresa = poc_sekcije;
					string is = decToHexa(adresa);
					string krajnje = "";
					if (is.length() < 4) {
						int nes = 4 - is.length();
						for (int k = 0; k < nes; k++) {
							krajnje += "0";
						}
						krajnje += is;
					}
					else {
						krajnje = is;
					}
					fajl << endl << krajnje << " ";
					int brojac = 0;
					adresa += 8;

					for (int b = 0; b < koliko_nula; b++) {
						fajl << "00 ";
						brojac += 2;
						koliko_popunjeno++;
					}
					//int brojac = 0;
					if (brojac >= kod.length()) {
						int sledece = i + 1;
						if (sledece == li.duzina()) {
							if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
							if (koliko_popunjeno == 0)break;
							for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
								fajl << "00 ";
							}
							koliko_popunjeno = 0;
							break;
						}
						if (pravi_pocetak + velicina == tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu()) {//nastavlja se pa nema 0

						}
						else {
							int poc_adr_sled = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu();
							int prava_adr_sled = poc_adr_sled;
							while (poc_adr_sled % 8 != 0) {
								poc_adr_sled--;
							}
							if (pravi_pocetak + velicina >= poc_adr_sled && pravi_pocetak + velicina <= prava_adr_sled) {

							}
							else {
								if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
								if (koliko_popunjeno == 0)continue;
								for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
									fajl << "00 ";
								}
								koliko_popunjeno = 0;
							}
						}
						continue;
					}
					for (int m = 0; m < kod.length(); m++) {
						fajl << kod[m];

						if ((brojac + 1) % 2 == 0) {
							fajl << " ";
							koliko_popunjeno++;
						}
						if ((brojac + 1) % 16 == 0) {
							//brojac += 8;
							if (m != kod.length() - 1) {
								fajl << endl;
								koliko_popunjeno = 0;
								string is = decToHexa(adresa);
								string krajnje = "";
								if (is.length() < 4) {
									int nes = 4 - is.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += is;
								}
								else {
									krajnje = is;
								}
								fajl << krajnje << " ";
								adresa += 8;
								//poc_sekcije += 8;
							}

						}
						brojac++;
					}
					int sledece = i + 1;
					if (sledece == li.duzina()) {
						if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
						if (koliko_popunjeno == 0)break;
						for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
							fajl << "00 ";
						}
						koliko_popunjeno = 0;
						break;
					}
					if (pravi_pocetak + velicina == tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu()) {//nastavlja se pa nema 0

					}
					else {
						int poc_adr_sled = tab_sekcija.nadjiSekciju(li.dohvatiSadrzaj(sledece).dohvatiNaziv()).dohvatiPocetnuAdresu();
						int prava_adr_sled = poc_adr_sled;
						while (poc_adr_sled % 8 != 0) {
							poc_adr_sled--;
						}
						if (pravi_pocetak + velicina >= poc_adr_sled && pravi_pocetak + velicina <= prava_adr_sled) {

						}
						else {
							if (koliko_popunjeno > 8)koliko_popunjeno -= 8;
							if (koliko_popunjeno == 0)continue;
							for (int kk = 0; kk < 8 - koliko_popunjeno; kk++) {
								fajl << "00 ";
							}
							koliko_popunjeno = 0;
						}
					}
				}
				
			}
		}
		
	}
}


void Linker::proveriDaLiTrebaDaSeBriseRelZapis() {
	TabelaRelokacionihZapisa t;
	for (int i = 0; i < rel_zapisi.duzina(); i++) {
		RelokacioniZapis r = rel_zapisi.dohvatiRelZapis(i);
		if (!tab_sekcija.da_li_postoji_sekcija(r.dohvatiImeSimbola()) && tab_simbola.dohvatiSimbol(r.dohvatiImeSimbola()).dohvatiLocGlob()=="global" && tab_simbola.dohvatiSimbol(r.dohvatiImeSimbola()).dohvatiNazivSekcije()!="UND") {
			if (tab_simbola.dohvatiSimbol(r.dohvatiImeSimbola()).dohvatiNazivSekcije() == r.dohvatiImeSekcije() && r.dohvatiTipRelokacije() == "R_X86_64_PC16") {//ovde bi onda trebalo da se obrise
				//ovde mislim da moram da izmenim sadrzaj sekcije
				string kod_za_izmenu = sadrzaji_sek.dohvatiKod(r.dohvatiImeSekcije(), r.dohvatiOfset());
				int zameni = tab_simbola.dohvatiSimbol(r.dohvatiImeSimbola()).dohvatiOfset() + r.dohvatiAdend() - r.dohvatiOfset();
				string heksa = decToHexaZaNegativne(zameni);
				string krajnje = "";
				if (heksa.length() < 4) {
					int nes = 4 - heksa.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += heksa;
				}
				else {
					krajnje = heksa;
				}
				sadrzaji_sek.izmeniKod(r.dohvatiImeSekcije(), r.dohvatiOfset(), krajnje);
			}
			else {
				RelokacioniZapis he = RelokacioniZapis(r.dohvatiSekciju(), r.dohvatiOfset(), r.dohvatiTipRelokacije(), r.dohvatiRbrSimbola(), r.dohvatiAdend());
				he.postaviImeSekcije(r.dohvatiImeSekcije());
				he.postaviImeSimbola(r.dohvatiImeSimbola());
				t.dodajRelokacioniZapis(he);
			}
		}
		else {
			RelokacioniZapis he = RelokacioniZapis(r.dohvatiSekciju(), r.dohvatiOfset(), r.dohvatiTipRelokacije(), r.dohvatiRbrSimbola(), r.dohvatiAdend());
			he.postaviImeSekcije(r.dohvatiImeSekcije());
			he.postaviImeSimbola(r.dohvatiImeSimbola());
			t.dodajRelokacioniZapis(he);
		}
		
	}
	if (t.duzina() < rel_zapisi.duzina()) {//tad treba da izmenim tabelu rel zapisa
		rel_zapisi.ocistiSve();
		for (int j = 0; j < t.duzina(); j++) {
			RelokacioniZapis r = t.dohvatiRelZapis(j);
			RelokacioniZapis dodaj = RelokacioniZapis(r.dohvatiSekciju(), r.dohvatiOfset(), r.dohvatiTipRelokacije(), r.dohvatiRbrSimbola(), r.dohvatiAdend());
			dodaj.postaviImeSekcije(r.dohvatiImeSekcije());
			dodaj.postaviImeSimbola(r.dohvatiImeSimbola());
			rel_zapisi.dodajRelokacioniZapis(dodaj);
		}
	}
	//nakon ovog bi trebalo da promene rel zapisi
}