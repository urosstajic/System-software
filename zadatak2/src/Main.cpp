#include "Main.h"
#include <iostream>
#include <fstream>
#include "Asembler.h"
#include "Linker.h"
#include <iostream>
#include "PocetnaAdresaSekcija.h"
#include <list>

using namespace std;


int main(int argc, char* argv[]) {
	if (argc < 4) {
		cout << "Nema dovoljno argumenata za pokretanje programa";
		return 0;
	}
	string asm_link=argv[1];
	if (asm_link == "asembler") {
		string ulazni_f, izlazni_f;
		ulazni_f = argv[4];
		izlazni_f = argv[3];
		ifstream ulaz;
		ulaz.open(ulazni_f);
		Asembler a;
		a.prvi_prolaz(ulaz);
		ulaz.close();
		ulaz.open(ulazni_f);
		a.drugi_prolaz(ulaz, izlazni_f);
		ulaz.close();
	}
	else if (asm_link == "linker") {
		list<PocetnaAdresaSekcija> poc_adrese;
		string ime_izlaznog;
		list<string> imena_ulaznih;
		Linker l;
		string oo = argv[2];
		if (oo == "-hex") {
			for (int i = 3; i < argc; i++) {
				string pom = argv[i];
				if (pom.find("-place") != string::npos) {
					string ime_sek, adresa;
					string novi_pom = pom.substr(7, pom.length());
					int deli = novi_pom.find('@');
					ime_sek = novi_pom.substr(0, deli);
					adresa = novi_pom.substr(deli + 1, novi_pom.length());
					adresa = adresa.substr(2, adresa.length());
					
					poc_adrese.push_back(PocetnaAdresaSekcija(adresa, ime_sek));

					
				}
			}
			for (int i = 3; i < argc; i++) {//izlazni
				string pom = argv[i];
				if (pom.find(".hex") != string::npos) {
					ime_izlaznog = pom;
					int vr = pom.find(".hex");
					ime_izlaznog = pom.substr(0, vr);
					ime_izlaznog += ".hex";
				}
			}
			for (int i = 3; i < argc; i++) {//ulazni
				string pom = argv[i];
				if (pom.find(".o") != string::npos) {
					int vr = pom.find(".o");
					string ime = pom.substr(0, vr);
					ime += ".o";
					imena_ulaznih.push_back(ime);
				}
			}
			list<string>::iterator iter;
			for (iter = imena_ulaznih.begin(); iter != imena_ulaznih.end(); iter++) {
				ifstream ulaz;
				ulaz.open((*iter));
				l.napraviTabeluSekcijaZaHex(ulaz);
				ulaz.close();
			}
			poc_adrese.sort([](PocetnaAdresaSekcija p1, PocetnaAdresaSekcija p2) {
				return p1.dohvatiPocAdr() < p2.dohvatiPocAdr();
			});
			/*list<PocetnaAdresaSekcija>::iterator pom;
			for (pom = poc_adrese.begin(); pom != poc_adrese.end(); pom++) {
				cout << (*pom).dohvatiPocAdr();
			}*/
			l.poredjajSekcije(poc_adrese);
			for (iter = imena_ulaznih.begin(); iter != imena_ulaznih.end(); iter++) {
				ifstream ulaz;
				ulaz.open((*iter));
				l.hex(ulaz);
				ulaz.close();
			}
			l.srediSadrzaj();

			l.ispis(ime_izlaznog);
		}
		else if (oo == "-linkable") {
			for (int i = 3; i < argc; i++) {//izlazni
				string pom = argv[i];
				if (pom.find(".o") != string::npos) {
					ime_izlaznog = pom;
					int vr = pom.find(".o");
					ime_izlaznog = pom.substr(0, vr);
					ime_izlaznog += ".o";
					break;
				}
			}
			int koji=0;
			for (int i = 3; i < argc; i++) {//ulazni
				string pom = argv[i];
				if (pom.find(".o") != string::npos) {
					if(koji!=0){
						int vr = pom.find(".o");
						string ime = pom.substr(0, vr);
						ime += ".o";
						imena_ulaznih.push_back(ime);
					}	
					koji++;
				}
			}
			list<string>::iterator iter;
			for (iter = imena_ulaznih.begin(); iter != imena_ulaznih.end(); iter++) {
				ifstream ulaz;
				ulaz.open((*iter));
				l.napraviTabeluSekcija(ulaz);
				ulaz.close();
			}
			//l.poredjajSekcije(poc_adrese);
			for (iter = imena_ulaznih.begin(); iter != imena_ulaznih.end(); iter++) {
				ifstream ulaz;
				ulaz.open((*iter));
				l.linkable(ulaz);
				ulaz.close();
			}
			//l.srediSadrzaj();
			l.proveriDaLiTrebaDaSeBriseRelZapis();
			l.ispisZaLinkable(ime_izlaznog);
		}
	}
	
	//string ime = argv[1];
	//if (ime == "asembler") {
		/*ifstream ulazni, ulazni2;
		ulazni.open("tekstfajl.txt");
		ulazni2.open("tekst1.txt");
		Asembler a;
		a.prvi_prolaz(ulazni);
		a.drugi_prolaz(ulazni2);
		ulazni.close();
		ulazni2.close();*/
	//}
	/*	cout << endl;
		Linker l;
		ifstream fajlic, fajl3;
		fajlic.open("fajl3.txt");
		fajl3.open("fajl2.txt");
		l.napraviTabeluSekcijaZaHex(fajlic);
		l.napraviTabeluSekcijaZaHex(fajl3);
		list<PocetnaAdresaSekcija> p;*/
		
		///p.push_back(PocetnaAdresaSekcija("0010", "ivt"));
		//p.push_back(PocetnaAdresaSekcija("0020", "myData"));
		/*l.poredjajSekcije(p);
		fajlic.close();
		fajl3.close();
		ifstream drugifajl, treci;
		drugifajl.open("fajl3.txt");
		treci.open("fajl2.txt");
		l.hex(drugifajl);
		l.hex(treci);
		drugifajl.close();
		treci.close();
		l.srediSadrzaj();
		l.ispis();*/

	return 0;
}