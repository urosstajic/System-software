#include "SadrzajSekcije.h"

int SadrzajSekcije::glob_brojac = 0;

SadrzajSekcije::SadrzajSekcije(string s) {
	this->sekcija = s;
}

void SadrzajSekcije::dodajKod(string k) {
	this->kod.append(k);
}

string SadrzajSekcije::dohvatiNaziv() {
	return this->sekcija;
}

int SadrzajSekcije::dohvati_glob_brojac() {
	return glob_brojac;
}

void SadrzajSekcije::postavi_glob_brojac() {
	glob_brojac = 0;
}

ostream& operator<<(ostream& it, SadrzajSekcije t)
{
	it << t.sekcija << "\n";
	int brojac1 = 8;
	
	it << "0000 ";
	for (int i = 0; i < t.kod.length(); i++) {		
		it << t.kod[i];
		if ((i + 1) % 2 == 0) {
			it << " ";
		}
		if ((i + 1) % 16 == 0) {
			if (i != t.kod.length() - 1) {
				it << "\n";
				char hexaDeciNum[100];
				int j = 0;
				int brojac = brojac1;
				while (brojac != 0) {
					int temp = 0;
					temp = brojac % 16;
					if (temp < 10) {
						hexaDeciNum[j] = temp + 48;
						j++;
					}
					else {
						hexaDeciNum[j] = temp + 55;
						j++;
					}
					brojac = brojac / 16;
				}
				string pom = "";
				for (int k = j - 1; k >= 0; k--) {
					pom += hexaDeciNum[k];
				}
				string ispis = pom;
				string krajnje = "";
				if (ispis.length() < 4) {
					int nes = 4 - ispis.length();
					for (int i = 0; i < nes; i++) {
						krajnje += "0";
					}
					krajnje += ispis;
				}
				it << krajnje << " ";
			}
			//it << krajnje << " ";
			brojac1 += 8;
		}
		
	}
	return it;
}

string SadrzajSekcije::nadjiKod(int ofset) {
	//string vrati = kod.substr(2 * ofset+1, 2 * ofset + 5);//samo ne znam jel vracam 4 cifara
	string kraj = "";
	for (int i = ofset * 2; i < ofset * 2 + 4; i++) {
		kraj += kod[i];
	}
	return kraj;
}

void SadrzajSekcije::ispisSadrzaja(ofstream& fajl) {
	fajl << this->sekcija;
	fajl << " " << kod;
}

void SadrzajSekcije::izmeniKod(int ofset, string kodd) {
	int j = 0;
	for (int i = ofset * 2; i < ofset * 2 + 4; i++) {
		kod[i] = kodd[j];
		j++;
	}
}

string SadrzajSekcije::dohvatiKod() {
	return kod;
}