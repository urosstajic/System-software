#include "Linker.h"
#include <iterator>
#include <sstream>

//835 linija linkable
//for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) 



bool Linker::postojiKodSekcije(string ime)
{
	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		if ((*iter).getImeSek() == ime) {
		     return true;
		}
	}
	return false;
}





void Linker::setujKodSekcije(string ime, string kodic) {
	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		if ((*iter).getImeSek() == ime) {
			 (*iter).kod=kodic;
			 return;
		}
	}
}



string Linker::dohvKodKodaSekcije(string ime) {
	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		if ((*iter).getImeSek() == ime) {
			return (*iter).kod;
		}
	}
	
}

KodSekcije& Linker::dohvKodSekcije(string ime)
{
	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		if ((*iter).getImeSek()== ime) {
			return (*iter);
		}
	}
	KodSekcije* k=new KodSekcije(ime,-4);
	return *k;
}

int Linker::postojiOffsetSek(string ime)
{
	list <OffsetSekcije> ::iterator iter;
	for (iter = offsetisek.begin(); iter != offsetisek.end(); ++iter) { //bilo je ++iter
		if ((*iter).ime == ime) {
			return (*iter).offset;
		}
	}
	return 0;
}

int Linker::postojiOffsetSekIII(string ime, int iii)
{
	list <OffsetSekcije> ::iterator iter;
	for (iter = offsetisek.begin(); iter != offsetisek.end(); ++iter) { //bilo je ++iter
		if ((*iter).ime == ime && (*iter).itielemnt==iii) {
			return (*iter).offset;
		}
	}
	return 0;
}



int Linker::postojiOffsetSekZASVE(string ime)
{
	list <OffsetSekcije> ::iterator iter;
	for (iter = offsetisekZASVE.begin(); iter != offsetisekZASVE.end(); ++iter) { //bilo je ++iter
		if ((*iter).ime == ime) {
			return (*iter).offset;
		}
	}
	return 0;
}

int Linker::postojiKaoStari(int stari)
{
	list <StariRB> ::iterator iter;
	for (iter = stariredbrojevi.begin(); iter != stariredbrojevi.end(); ++iter) { //bilo je ++iter
		if ((*iter).stari == stari) {
			return (*iter).novi;
		}
	}
	return -5;
}

OffsetSekcije& Linker::dohvOffsetSek(string ime)
{
	list <OffsetSekcije> ::iterator iter;
	for (iter = offsetisek.begin(); iter != offsetisek.end(); ++iter) { //bilo je ++iter
		if ((*iter).ime == ime) {
			return (*iter);
		}
	}
}

bool Linker::noviBrojDaLiJeLokalniIliEqu(int rbb)
{
	list <Simbol> ::iterator iter;
	for (iter = tabelasimbola.tabela.begin(); iter != tabelasimbola.tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getSekcija() == "equ") {
			return true;

		}
		if ((*iter).getRB()==rbb) {
			return (*iter).getLocal();
		}
	}
	
}

int Linker::noviOffsetIzTabeleSim(int rbb)
{
	list <Simbol> ::iterator iter;
	for (iter = tabelasimbola.tabela.begin(); iter != tabelasimbola.tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getRB() == rbb) {
		///////////////vrati iz starih rednih brojeva za sek
			int i = postojiOffsetSek((*iter).getLabela());
			return i;
		}
	}
	return 0;
}

void Linker::dodajuOffseteSek(OffsetSekcije sek)
{
	offsetisek.push_back(sek);
}

void Linker::dodajuStareRBbrojeve(StariRB r)
{
	stariredbrojevi.push_back(r);
}



void Linker::dodajuKodove(KodSekcije s)
{
	kodovi.push_back(s);
}

string Linker::dohvPocetnuAdresuZaKodSekcije(string ime) {
	list <Sekcija> ::iterator iter1;

	for (iter1 = hextabelasekcija.tabela.begin(); iter1 != hextabelasekcija.tabela.end(); ++iter1) {
		if ((*iter1).ime == ime) {
			return (*iter1).pocetnaAdresa;
		}
	}
}

void Linker::ispisiKodoveHEXPravilno(ofstream*  izlazni) {
	
	string adresa;
	int pocetak=0,kraj=0;
	
	string ppp = "";
	string pocetna="#";
	string stringic;
	list <KodSekcije> ::iterator iter;

	for (iter = kodoviHEX.begin(); iter != kodoviHEX.end(); ++iter) { //bilo je ++iter
		if ((*iter).getImeSek() == "equ") {
			continue;
		}

		string adresa = dohvPocetnuAdresuZaKodSekcije((*iter).getImeSek());

		if ((pocetna == "#") || (jednako(pocetna,adresa))) {
			stringic += (*iter).kod;
			char hex_stringg[20];
			sprintf(hex_stringg, "%X", (*iter).kod.size() / 2);
			string sizeofkod = hex_stringg;

			string kk = saberi(adresa, sizeofkod);
			if (pocetna == "#") ppp = adresa;
			pocetna = kk;
			
		}
		else {
			adresa = ppp;
			char hex_stringg[20];
			sprintf(hex_stringg, "%X", stringic.size() / 2);
			string sizeofkod = hex_stringg;

			string krajnja = saberi(adresa, sizeofkod);
			int oduzmii = stoi(adresa, nullptr, 16);

			while (uporediVece(krajnja, adresa)) {


				pocetak = (stoi(adresa, nullptr, 16) - oduzmii) * 2;  //skini joj nule ili x

				string s5; //adresa
				char hex_string[20];
				int pomocna = stoi(adresa, nullptr, 16);
				int pomocnaa = pomocna;
				if (pomocna % 8 != 0) {
					sprintf(hex_string, "%X", (pomocna % 8));
					string xxxx = hex_string;
					adresa = oduzmi(adresa, xxxx);
					pomocnaa = stoi(adresa, nullptr, 16);
				}
				sprintf(hex_string, "%X", pomocnaa); //convert number to hex
				s5 = hex_string;
				for (int i = s5.size(); i < 4; i++) {
					s5 = "0" + s5;
				}
				adresa = s5;
				kraj = pocetak + 16;
				*izlazni << adresa << ": ";
				int i;
				if (pomocna % 8 != 0) {
					for (int kk = 0; kk < pomocna % 8; kk++)
						*izlazni << "00 ";
					kraj = kraj - 2 * (pomocna % 8);
				}
				for (i = pocetak; i < kraj && i < stringic.size(); i = i + 2) {
					*izlazni << stringic[i] << stringic[i + 1] << " ";
				}

				if (!(i < stringic.size())) {
					for (; i < kraj; i = i + 2)
						*izlazni << "00 ";
				}

				*izlazni << endl;

				adresa = saberi(adresa, "8");

			}

			adresa = dohvPocetnuAdresuZaKodSekcije((*iter).getImeSek());
			stringic = (*iter).kod;
			sprintf(hex_stringg, "%X", (*iter).kod.size() / 2);
			sizeofkod = hex_stringg;

			string kk = saberi(adresa, sizeofkod);
			ppp = adresa;
			pocetna = kk;
			
		}

		
	}


	adresa = ppp;

	/////
	char hex_stringg[20];
	sprintf(hex_stringg, "%X", stringic.size() / 2);
	string sizeofkod = hex_stringg;

	string krajnja = saberi(adresa, sizeofkod);
	int oduzmii = stoi(adresa, nullptr, 16);

	while (uporediVece(krajnja, adresa)) {


		pocetak = (stoi(adresa, nullptr, 16) - oduzmii) * 2;  //skini joj nule ili x

		string s5; //adresa
		char hex_string[20];
		int pomocna = stoi(adresa, nullptr, 16);
		int pomocnaa = pomocna;
		if (pomocna % 8 != 0) {
			sprintf(hex_string, "%X", (pomocna % 8));
			string xxxx = hex_string;
			adresa = oduzmi(adresa, xxxx);
			pomocnaa = stoi(adresa, nullptr, 16);
		}
		sprintf(hex_string, "%X", pomocnaa); //convert number to hex
		s5 = hex_string;
		for (int i = s5.size(); i < 4; i++) {
			s5 = "0" + s5;
		}
		adresa = s5;
		kraj = pocetak + 16;
		*izlazni << adresa << ": ";
		int i;
		if (pomocna % 8 != 0) {
			for (int kk = 0; kk < pomocna % 8; kk++)
				*izlazni << "00 ";
			kraj = kraj - 2 * (pomocna % 8);
		}
		for (i = pocetak; i < kraj && i < stringic.size(); i = i + 2) {
			*izlazni << stringic[i] << stringic[i + 1] << " ";
		}

		if (!(i < stringic.size())) {
			for (; i < kraj; i = i + 2)
				*izlazni << "00 ";
		}

		*izlazni << endl;

		adresa = saberi(adresa, "8");
	}
	/////

}

void Linker::ispisiKodove()
{
	cout << "Kodovi" << endl;
	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		cout << (*iter) << endl;
	}
}
void Linker::ispisiKodoveHEX()
{
	list <KodSekcije> ::iterator iter;
	for (iter = kodoviHEX.begin(); iter != kodoviHEX.end(); ++iter) { //bilo je ++iter
		cout << (*iter) << endl;
	}
}
void Linker::obradaPrvi(ifstream& ulaz)
{
	prvi = false;
	string linija;
	getline(ulaz, linija); //mozda ne mora while
	
			if (linija == "TabelaSekcija") {
				tabelasek = true;
				while (tabelasek == true && getline(ulaz, linija) ) {
					istringstream ime(linija);
					string deo;
					
					
					while (ime >> deo  && tabelasek==true) {
						if (deo == "TabelaSimbola") {
							tabelasek = false;
							tabelasimb = true;
							break;
						}
						string  size, rb, poc, kraj;
						ime >> size; ime >> rb; ime >> poc; ime >> kraj;
						int i = stoi(rb);
						Sekcija s(deo,size,i,poc,kraj);
						tabelasekcija.dodajSekciju(s);
					
				    }

			    }
				//cout <<"TabelaSekcija" <<endl<<tabelasekcija;
			

			}
		   
			if (tabelasimb == true) { //tabelasimvbola

				while ( tabelasimb == true && getline(ulaz, linija)) {
					istringstream ime(linija);
					string deo;


					while (ime >> deo && tabelasimb == true) { //deo je labela
						if (deo == "RelokacionaTabela") {
							tabelasimb = false;
							relokacioni = true;
							break;
						}
						string  offset, sek, lokal, rb;
						ime >> offset; ime >> sek; ime >> lokal; ime >> rb;
						int i = stoi(offset);
						bool l;
						if (lokal == "local") l = true;
						else l = false;
						int rbb = stoi(rb);
						Simbol s(deo, sek, i, l);
						tabelasimbola.dodajSimbol(s);
						globalnibrojaczasimbole++;

					}

				}
				//cout <<"TabelaSimbola" <<endl<<tabelasimbola;
			}
			if (relokacioni == true) { //tabelasimbola

				while (relokacioni == true && getline(ulaz, linija)) {
					istringstream ime(linija);
					string deo;


					while (ime >> deo && relokacioni == true) { //deo je labela
						if (deo == "Kodovi") {
							kodovi1 = true;
							relokacioni = false;
							break;
						}

						string  offset, tip, rb, add;
						ime >> offset;  ime >> tip; ime >> rb; ime >> add;
						int i = stoi(offset);
						int j = stoi(rb);
						int k = stoi(add);
						RelZapis r(deo, i, tip, j, k);
						brojacRelZapisaIzPrethodne++;
						relokacionatabela.dodajZapis(r);
					}

				}
				//cout << relokacionatabela;
			}
	

			if (kodovi1 == true) { //relokacioni zapisi

				while (getline(ulaz, linija)) {
					if (linija == "") { break; }
					istringstream ime(linija);
					string deo;


					while (ime >> deo) { //deo je labela
						string masinski;
						getline(ulaz, masinski);
						KodSekcije k(deo);
						k.dodajuKod(masinski);
						dodajuKodove(k);


					}

				}
				kodovi1 = false;
				//ispisiKodove();
				
			}
			
	   
}

void Linker::obradaNePrvi(ifstream& ulaz)
{
	/////////////////////////////////////////////////////////////
	string linija;
	getline(ulaz, linija); //mozda ne mora while

	if (linija == "TabelaSekcija") {
		tabelasek = true;
		while (tabelasek == true && getline(ulaz, linija)) {
			istringstream ime(linija);
			string deo;


			while (ime >> deo && tabelasek == true) {
				if (deo == "TabelaSimbola") {
					tabelasek = false;
					tabelasimb = true;
					break;
				}
				string  size, rb, poc, kraj;
				ime >> size; ime >> rb; ime >> poc; ime >> kraj;
				int i = stoi(rb); //redni broj iz tabele simbola
				if (tabelasekcija.postojiSekcija(deo)) {
					//nadovezi
					Sekcija& istasek = tabelasekcija.dohvSek(deo);
					int n = stoi(size, nullptr, 16);
					int offi = stoi(istasek.size, nullptr, 16); //bice decimalno

					OffsetSekcije o(deo, offi);

					dodajuOffseteSek(o);
					istasek.promeniVel(n); //za neki broj

				}
				else {
					Sekcija s(deo, size, i, poc, kraj);
					tabelasekcija.dodajSekciju(s);
				}
			}

		}
	//	cout << "TabelaSekcija" << endl << tabelasekcija;


	}

	if (tabelasimb == true) { //tabelasimbola

		while (tabelasimb == true && getline(ulaz, linija)) {
			istringstream ime(linija);
			string deo;


			while (ime >> deo && tabelasimb == true) { //deo je labela
				if (deo == "RelokacionaTabela") {
					tabelasimb = false;
					relokacioni = true;
					break;
				}

				string  offset, sek, lokal, rb;
				ime >> offset; ime >> sek; ime >> lokal; ime >> rb;
				int i = stoi(offset);
				bool l;
				if (lokal == "local") l = true;
				else l = false;
				int rbb = stoi(rb);
				int postoji = postojiOffsetSek(sek);
				if (postoji != 0) {
					i += postoji; //menjamo offset
				}

				if (tabelasimbola.proveriLabelu(deo)) { //postoji u tabeli simb
					Simbol& postojiLabela = tabelasimbola.dohvSimbol(deo);

					if (deo == sek) {
						StariRB s(deo, postojiLabela.getRB(), rbb);
						dodajuStareRBbrojeve(s);
					}

					else {

						if (sek == "UND" && postojiLabela.getSekcija() != "UND" && postojiLabela.getLocal()) {
							//	StariRB s(deo, postojiLabela.getRB(), rbb);
							StariRB s(deo, tabelasimbola.dohvatiRBSekcije(postojiLabela.getSekcija()), rbb);
							dodajuStareRBbrojeve(s);
						}
						else if (sek == "UND" && postojiLabela.getSekcija() != "UND" && l==false) {
							StariRB s(deo, postojiLabela.getRB(), rbb);
							dodajuStareRBbrojeve(s);
						}
						else if (sek == "UND" && postojiLabela.getSekcija() == "UND") {
							//StariRB s(deo, postojiLabela.getRB(), rbb); //vrv ovako
							StariRB s(deo, tabelasimbola.dohvatiRBSekcije(postojiLabela.getSekcija()), rbb);
							dodajuStareRBbrojeve(s);
						}
						else if (sek != "UND" && postojiLabela.getSekcija() == "UND") {
							postojiLabela.setSekcija(sek);
							postojiLabela.setLocal(l);
							postojiLabela.setOffset(i);
							if (l) {
								StariRB s(deo, tabelasimbola.dohvatiRBSekcije(postojiLabela.getSekcija()), rbb);
								dodajuStareRBbrojeve(s);
							}
							else {
									StariRB s(deo, postojiLabela.getRB(), rbb); //novi stari
									dodajuStareRBbrojeve(s);
							}
							
						}
						else {
							if (!tabelasekcija.postojiSekcija(deo)) {
								cout << "Visestruka definicija simbola" + deo << endl;
							}

						}
					}
				}
				else {
					Simbol s(deo, sek, i, l);
					tabelasimbola.dodajSimbol(s);
					lokalnibrojaczasimbole++;
					if (l) {
						StariRB ss(deo, tabelasimbola.dohvatiRBSekcije(s.getSekcija()), rbb);
						dodajuStareRBbrojeve(ss);
					}
					else {
						StariRB ss(deo, s.getRB(), rbb);
						dodajuStareRBbrojeve(ss);
					}
					//
					
				}

			}
		}
	

		//cout << "TabelaSimbola" << endl << tabelasimbola;
	}

	if (relokacioni == true) { //tabelasimvbola

		while (relokacioni == true && getline(ulaz, linija)) {
			istringstream ime(linija);
			string deo;


			while (ime >> deo && relokacioni == true) { //deo je labela
				if (deo == "Kodovi") {
					kodovi1 = true;
					relokacioni = false;
					break;
				}

				string  offset, tip, rb, add;
				ime >> offset;  ime >> tip; ime >> rb; ime >> add;
                int i = stoi(offset);
				int j = stoi(rb);
				int k = stoi(add);
                RelZapis r(deo, i, tip, j, k);
				int nn = postojiOffsetSek(deo);
				if (nn!=0) {
					int offi = r.getOffset() + nn;
					r.setOffset(offi);
				}
				int novibr = postojiKaoStari(j);
				if (novibr!=-5) {
					r.setRB(novibr);
				}
				
				
				
				relokacionatabela.dodajZapis(r);
				
			}

		}
		
	}
	if (kodovi1 == true) { //relokacioni zapisi

		while (getline(ulaz, linija)) {
			if (linija == "") { break; }
			istringstream ime(linija);
			string deo;


			while (ime >> deo) { //deo je labela
				string masinski;
				getline(ulaz, masinski);
				KodSekcije& k=dohvKodSekcije(deo);
				
				k.dodajuKod(masinski);
				if (k.indikator == -4 ) {
					k.indikator = 0;
					kodovi.push_back(k);
				}
				int brojac = brojacRelZapisaIzPrethodne;
				
				list <RelZapis> ::iterator iter;
				for (iter = relokacionatabela.tabela.begin(); iter != relokacionatabela.tabela.end(); ++iter) { //bilo je ++iter
					
					if (brojacRelZapisaIzPrethodne > 0 && prvifajllinkable==true) {
						
							//da li postoji stari
							int rb = (*iter).getRB();
							Simbol& simb = tabelasimbola.dohvSimbRBPo(rb);
							if (simb.getLocal() == true && simb.getLabela() != simb.getSekcija()) {
								int rbsek = tabelasimbola.dohvatiRBSekcije(simb.getSekcija());
								(*iter).setRB(rbsek); //na rb sekcije
								//////
								int novioffset = tabelasimbola.dohvatiOffsetRB(rb); //sta treba upisati
								//prepravi u kodu
								string kodic = dohvKodKodaSekcije(simb.getSekcija());
								int lok = (*iter).getOffset(); ////

								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", novioffset); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								if ((*iter).getTipRealokacije() == "R_X86_64noninst_16") {
									kodic[lok * 2] = s5[2];
									kodic[lok * 2 + 1] = s5[3];
									kodic[lok * 2 + 2] = s5[0];
									kodic[lok * 2 + 3] = s5[1];
								}
								else {
									kodic[lok * 2] = s5[0];
									kodic[lok * 2 + 1] = s5[1];
									kodic[lok * 2 + 2] = s5[2];
									kodic[lok * 2 + 3] = s5[3];
								}
								setujKodSekcije(simb.getSekcija(), kodic);
								//////
							}
						
						brojacRelZapisaIzPrethodne--;
						continue;
					}
					brojac++;
					if ((*iter).getSekcija()==deo) {
						if (noviBrojDaLiJeLokalniIliEqu((*iter).getRB()+globalnibrojaczasimbole)) {
							int novioffset = noviOffsetIzTabeleSim((*iter).getRB()+ globalnibrojaczasimbole); //sta treba upisati
							//prepravi u kodu
							string kodic=dohvKodKodaSekcije(deo);
							int lok = (*iter).getOffset(); ////
							
							string s8, s9;
							if ((*iter).getTipRealokacije() == "R_X86_64noninst_16") {
								s8 = kodic.substr(lok * 2, 2);
								s9 = kodic.substr(lok * 2 + 2, 2);
							}
							else {
								s9 = kodic.substr(lok * 2, 2);
								s8 = kodic.substr(lok * 2 + 2, 2);
							}

							int dodaj = stoi(s9 + s8, nullptr, 16); //sta se vec tamo nalazi

							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", novioffset+dodaj); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							if ((*iter).getTipRealokacije() == "R_X86_64noninst_16") {
								kodic[lok * 2] = s5[2];
								kodic[lok * 2 + 1] = s5[3];
								kodic[lok * 2 + 2] = s5[0];
								kodic[lok * 2 + 3] = s5[1];
							}
							else {
								if (s5.size() == 8) {
									kodic[lok * 2] = s5[4];
									kodic[lok * 2 + 1] = s5[5];
									kodic[lok * 2 + 2] = s5[6];
									kodic[lok * 2 + 3] = s5[7];
								}
								kodic[lok * 2] = s5[0];
								kodic[lok * 2 + 1] = s5[1];
								kodic[lok * 2 + 2] = s5[2];
								kodic[lok * 2 + 3] = s5[3];
							}
							setujKodSekcije(deo, kodic);
						}
						else { //globalni je
						///////
							string kodic = dohvKodKodaSekcije(deo);
							int lok = (*iter).getOffset(); ////
							string s5 = "0000";
							if ((*iter).getTipRealokacije() == "R_X86_64noninst_16") {
								kodic[lok * 2] = s5[2];
								kodic[lok * 2 + 1] = s5[3];
								kodic[lok * 2 + 2] = s5[0];
								kodic[lok * 2 + 3] = s5[1];
							}
							else {
								kodic[lok * 2] = s5[0];
								kodic[lok * 2 + 1] = s5[1];
								kodic[lok * 2 + 2] = s5[2];
								kodic[lok * 2 + 3] = s5[3];
							}
							setujKodSekcije(deo, kodic);
							///////
						}
					}
				}
				brojacRelZapisaIzPrethodne = brojac;


			}

		}
		kodovi1 = false;
	//	cout << relokacionatabela;
	//	ispisiKodove();
		globalnibrojaczasimbole += lokalnibrojaczasimbole;
		lokalnibrojaczasimbole = 0;

	}
	//////////////////////////////////////////////////////////
	offsetisek.clear(); // da za novi fajl bude to na 0
	stariredbrojevi.clear();
	prvifajllinkable = false;
}



void Linker::obradaLinkable(ifstream& ulaz)
{
	if (prvi) { //samo sve dodaje
		obradaPrvi(ulaz);
	}
	//sad citaj i vidi treba li da dodas
	obradaNePrvi(ulaz);

}



void Linker::linkable(list<string> lista, string smesti)
{
	
	list<string> ::iterator iter1;
	
	for (iter1 = lista.begin(); iter1 != lista.end(); ++iter1) {
		ifstream  otvori;
		otvori.open((*iter1));
		
		if (otvori.is_open() == false) {
			cout << "Neuspesno otvoreni fajlovi";
			exit(1);
		}
		obradaLinkable(otvori);
		otvori.close();
		
	}

	ofstream izlazni;
	izlazni.open(smesti);
	///pa ovde sve da ispisuje a ne tamo gde sada
	if (izlazni.is_open() == false) {
		cout << "Neuspesno otvoreni fajlovi";
		exit(1);
	}

	izlazni<<"TabelaSekcija" <<endl<< tabelasekcija;
	izlazni << "TabelaSimbola" << endl << tabelasimbola;
	izlazni << relokacionatabela;
	izlazni << "Kodovi" << endl;
	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		izlazni << (*iter) << endl;
	}
	izlazni.close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////HEX//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void Linker::preuredikodoveHEX()
{
	list <Sekcija> ::iterator iter;
	for (iter = hextabelasekcija.tabela.begin(); iter != hextabelasekcija.tabela.end(); ++iter) { //bilo je ++iter
		string ime = (*iter).ime;
		list <KodSekcije> ::iterator iter1;
		for (iter1 = kodovi.begin(); iter1 != kodovi.end(); ++iter1) {
			if (ime == (*iter1).getImeSek()) {
				kodoviHEX.push_back((*iter1));
			}
		}
	}
}

bool Linker::uporediVece(string s1, string s2)
{
	int n = stoi(s1, nullptr, 16);
	int n2 = stoi(s2, nullptr, 16);
	return n > n2;
}

bool Linker::uporediVeceJednako(string s1, string s2)
{
	int n = stoi(s1, nullptr, 16);
	int n2 = stoi(s2, nullptr, 16);
	return n >= n2;
}

bool Linker::uporediManje(string s1, string s2)
{
	int n = stoi(s1, nullptr, 16);
	int n2 = stoi(s2, nullptr, 16);
	return n2 > n;
}

bool Linker::jednako(string s1, string s2) {
	int n = stoi(s1, nullptr, 16);
	int n2 = stoi(s2, nullptr, 16);
	return n2 == n;
}

string Linker::oduzmi(string s1, string s2)
{

	int n = stoi(s1, nullptr, 16);
	int n2 = stoi(s2, nullptr, 16);
	n -= n2;
	string s5;
	char hex_string[20];

	sprintf(hex_string, "%X", n); //convert number to hex
	s5 = hex_string;


	return s5;
}

string Linker::saberi(string s1, string s2)
{
	int n = stoi(s1, nullptr, 16);
	int n2 = stoi(s2, nullptr, 16);
	n += n2;
	string s5;
	char hex_string[20];

	sprintf(hex_string, "%X", n); //convert number to hex
	s5 = hex_string;
	

	return s5;
	
}



string Linker::postojiUPlace(string ime)
{
	list <Place> ::iterator iter;
	for (iter = place.begin(); iter != place.end(); ++iter) { //bilo je ++iter
		if ((*iter).sekcija == ime) {

			int n = stoi((*iter).pocetnaadresa, nullptr, 16);
			
			string s5;
			char hex_string[20];

			sprintf(hex_string, "%X", n); //convert number to hex
			s5 = hex_string;


			return s5;

		}
	}
	return "-1";
}



void Linker::obradiHEX()
{
 //prvo obradjujem sekcije
	string linija;
	for(int i = 0; i < ulaznifajlVelicina; i++) {
		getline(ulaznifajlovi[i], linija); //ucitava tabelu sek
		while (getline(ulaznifajlovi[i], linija)) {
			if (linija == "TabelaSimbola") {
				break;
			}
			istringstream ime(linija);
			string deo;

			while (ime >> deo) {
				string trenutnasek = deo;
				
				OffsetSekcije os(deo,0,i); // za taj iti fajl za tu sek
				offsetisek.push_back(os);
				if (hextabelasekcija.postojiSekcija(deo)) {
					break;
				}
				string size;
				ime >> size; //ucitana velicina u hex
				string nebitno1, nebitno2, nebitno3;
				ime >> nebitno1; ime >> nebitno2; ime >> nebitno3;
				if (deo == "equ") { //equ ne treba da doda
					continue;
				}
				Sekcija s(deo, size);
				string linija1;
				for (int j = i+1; j < ulaznifajlVelicina; j++) {
					getline(ulaznifajlovi[j], linija1); //ucitava tabelu sek
					while (getline(ulaznifajlovi[j], linija1)) {
						if (linija1 == "TabelaSimbola") {
							break;
						}
						istringstream ime(linija1);
						string deo1;

						while (ime >> deo1) {
							if (deo1 == trenutnasek) {
								string broj;
								ime >> broj;
								string nebitno11, nebitno22, nebitno33;
								ime >> nebitno11; ime >> nebitno22; ime >> nebitno33;
								int iii = stoi(s.size, nullptr, 16);
								OffsetSekcije os(deo1, iii, j);
								offsetisek.push_back(os);
								s.size = saberi(broj, s.size); //hex sabiranje vraca se string
								//napravi offset za OffsetSek za i-ti fajl
								break;
							}
							else {
								break;
							}


						}
					}
					//seek
					ulaznifajlovi[j].clear();
					ulaznifajlovi[j].seekg(0);
				}
				hextabelasekcija.dodajSekciju(s);

			}





		}//kraj while unutrasnjeg
	}//kraj while spoljasnjeg


	//postavljanje pocetnih adresa

	//-3 vrednost koju cuvamo na nivou sekcije

	string maxadresa = "-1";
	list <Sekcija> ::iterator iter;
	for (iter = hextabelasekcija.tabela.begin(); iter != hextabelasekcija.tabela.end(); ++iter) {
		string adresa = postojiUPlace((*iter).ime);
		if (adresa != "-1") {
			(*iter).pocetnaAdresa = adresa;
			(*iter).krajnjaAdresa = oduzmi(saberi(adresa, (*iter).size), "1");
			int a = stoi(adresa, nullptr, 16);
			OffsetSekcije os((*iter).ime, a);
			offsetisekZASVE.push_back(os);
			int offi = stoi(adresa, nullptr, 16);

			Simbol s((*iter).ime, (*iter).ime, offi, true);
			hextabelasimbola.dodajSimbol(s);
			string uporedi = (*iter).krajnjaAdresa;
			if (uporediVece(saberi(uporedi,"1"), maxadresa)) { //uporedi vece od max
				maxadresa = saberi(uporedi, "1");
			}
		}
	}
	if (maxadresa == "-1")
		maxadresa = "0";
	for (iter = hextabelasekcija.tabela.begin(); iter != hextabelasekcija.tabela.end(); ++iter) {
		if ((*iter).pocetnaAdresa == "#") {
			(*iter).pocetnaAdresa = maxadresa;
			(*iter).krajnjaAdresa = oduzmi(saberi(maxadresa, (*iter).size), "1");
			int a = stoi(maxadresa, nullptr, 16);
			OffsetSekcije os((*iter).ime, a);
			offsetisekZASVE.push_back(os);
			int offi = stoi(maxadresa, nullptr, 16);

			Simbol s((*iter).ime, (*iter).ime, offi, true);
			hextabelasimbola.dodajSimbol(s);
			string uporedi = (*iter).krajnjaAdresa;
			maxadresa = saberi(uporedi, "1");
		}
	}
	

	TabelaSekcija pomocnahextabela;
	
	for (int iii = 0; iii < hextabelasekcija.tabela.size(); iii++) {
		Sekcija s("","");
		s.pocetnaAdresa = "FFFF";
		for (iter = hextabelasekcija.tabela.begin(); iter != hextabelasekcija.tabela.end(); ++iter) {
			if (uporediManje((*iter).pocetnaAdresa, s.pocetnaAdresa))
				s = (*iter);
		}

		pomocnahextabela.tabela.push_back(s);

		for (iter = hextabelasekcija.tabela.begin(); iter != hextabelasekcija.tabela.end(); ++iter) {
			if ((*iter).ime == s.ime) {
				(*iter).pocetnaAdresa = "FFFF";
			}
		}
	}

	hextabelasekcija = pomocnahextabela;



	
	//tabela simbola obrada
	for (int i = 0; i < ulaznifajlVelicina; i++) {
		// za svih i fajlova radi isto
		TabelaSimbola pomocna;
		RelTabela pomocnarel;
		list<StariRB> staritabelasimb;
		
		
		while (getline(ulaznifajlovi[i], linija)) {
			
			if (linija == "RelokacionaTabela") {
				break;
			}
		
			istringstream ime(linija);
			string deo;

			while (ime >> deo) {
				string  offset, sek, lokal, rb;
				ime >> offset; ime >> sek; ime >> lokal; ime >> rb;
				int i = stoi(offset);
				bool l;
				if (lokal == "local") l = true;
				else l = false;
				int rbb = stoi(rb);
				Simbol s(deo, sek, i, l,rbb);
			
				pomocna.dodajSimbol(s);
				break;
			}

		}
		//rel zapisi dodavanje
		while (getline(ulaznifajlovi[i], linija)) {

			if (linija == "Kodovi") {
				break;
			}

			istringstream ime(linija);
			string deo;
			
			while (ime >> deo) {
				string  offset, tip, rb, add;
				ime >> offset;  ime >> tip; ime >> rb; ime >> add;
				int iii = stoi(offset);
				int j = stoi(rb);
				int k = stoi(add);
				RelZapis r(deo, iii, tip, j, k,i);
				pomocnarel.dodajZapis(r);
				break;
			}

		}
	//	cout << pomocna<<endl;
		//cout << pomocnarel<<endl;  

		//iziteriraj kroz sve i promeni i dodaj u svi rel zapisi
		list<Simbol> ::iterator iter2;
		for (iter2 = pomocna.tabela.begin(); iter2 != pomocna.tabela.end(); ++iter2) {
			if ((*iter2).getLabela() == (*iter2).getSekcija()) { //ne radi za equ && (*iter2).getLabela()!=equ
				Simbol sim = hextabelasimbola.dohvSimbol((*iter2).getLabela());
				StariRB ss((*iter2).getLabela(), sim.getRB(), (*iter2).getRB());
				staritabelasimb.push_back(ss);
			}
			else if ((*iter2).getLocal() == false && (*iter2).getSekcija()!="UND") {// globalni dodaj u tabelu simb hex
				int napravioffset = (*iter2).getOffset();
				napravioffset += postojiOffsetSekIII((*iter2).getSekcija(),i);
				napravioffset += postojiOffsetSekZASVE((*iter2).getSekcija());
				Simbol sim((*iter2).getLabela(), (*iter2).getSekcija(), napravioffset, false);
				hextabelasimbola.dodajSimbol(sim);
				StariRB ss((*iter2).getLabela(), sim.getRB(), (*iter2).getRB());
				staritabelasimb.push_back(ss);
			}
			else if((*iter2).getSekcija() == "UND"){
			 //dodaj u listu nedefinisanih
				StariRB ss((*iter2).getLabela(), -5, (*iter2).getRB());
				staritabelasimb.push_back(ss);
				undefinedlabel.push_back((*iter2).getLabela());
			}

		}

		//prodji kroz rel zapise prepravi rb i offset i dodaj u svirelzapisi
		list<RelZapis> ::iterator iter3;
		for (iter3 = pomocnarel.tabela.begin(); iter3 != pomocnarel.tabela.end(); ++iter3) {
			int stari = (*iter3).getRB();
			int novi = -1;
			
			//promeni offset
				int napravioffset = (*iter3).getOffset();
				napravioffset += postojiOffsetSekIII((*iter3).getSekcija(), i); //a mi smo dodali samo pomeraj unutar sek
				(*iter3).setOffset(napravioffset);

				list<StariRB>::iterator itt;
				for (itt = staritabelasimb.begin(); itt != staritabelasimb.end(); ++itt) {
					if ((*itt).stari == stari) {
						novi = (*itt).novi;
						if (novi == -5) {
							PrepravkaRelZapisa r((*iter3).getSekcija(),(*iter3).getOffset(), (*itt).labela);
							prepravke.push_back(r);
						}
						break;
					}

				}

				(*iter3).setRB(novi);

			svirelzapisi.dodajZapis((*iter3));
		}

		staritabelasimb.clear(); //svakako nestane pa se napravi nova
	}
	///prepravi red brojeve u rel zapisima
	list<PrepravkaRelZapisa> ::iterator iter4;
	for (iter4 = prepravke.begin(); iter4 != prepravke.end(); ++iter4) {
		int rb = hextabelasimbola.dohvatiRB((*iter4).globalnalabela);
		list<RelZapis> ::iterator iter3;
		for (iter3 = svirelzapisi.tabela.begin(); iter3 != svirelzapisi.tabela.end(); ++iter3)
		{
			if ((*iter3).getSekcija() == (*iter4).sekcija && (*iter3).getOffset() == (*iter4).offset) {
				(*iter3).setRB(rb);
			}
		}
	}
	
	//prodji kroz undefinedlabel i ako je prazna ok je ako nije,  nije se sve razresilo

	//OTKOMENTARISATI ZA PRAVI PRIMER
	
         list<string> ::iterator iter5;
	   for (iter5 = undefinedlabel.begin(); iter5 != undefinedlabel.end(); ++iter5) {
		   if (hextabelasimbola.postojiLabela((*iter5))==false) {
			cout << "Postojanje nerazresenog simbola " << (*iter5); exit(1);
		   }
	   }
	
	//ucitaj sve kodove spoji 
	for (int i = 0; i < ulaznifajlVelicina; i++) {
		
		while (getline(ulaznifajlovi[i], linija)) {
            istringstream ime(linija);
			string deo;
			ime >> deo;
			if (postojiKodSekcije(deo)==false) { //ako ne postoji napravi
				KodSekcije k(deo);
				string masinski;
				getline(ulaznifajlovi[i], masinski);
				k.dodajuKod(masinski);
				kodovi.push_back(k);
			}
			else {
				KodSekcije& k = dohvKodSekcije(deo);
				string masinski;
				getline(ulaznifajlovi[i], masinski);
				k.dodajuKod(masinski);
			}
		}
	}

	//u tabeli simbola se vec nalaze vrednosti gde ih je linker smestio !!BITNO!!
	//prepravi kodove na osnovu rel zapisa
	list<RelZapis> ::iterator iter6;
	for (iter6 = svirelzapisi.tabela.begin(); iter6 != svirelzapisi.tabela.end(); ++iter6)
	{
	int prepravkaKod = 0;
		int rb = (*iter6).getRB();
			Simbol& sim=hextabelasimbola.dohvSimbRBPo(rb);
		if (sim.getLocal() == false) { //globalni
			prepravkaKod = sim.getOffset(); //simbol iz tabele simbola
			if ((*iter6).getAddend() == -2) {
				prepravkaKod -= 2;
				int trenutnaAdresa = 0;
				trenutnaAdresa = (*iter6).getOffset();
				trenutnaAdresa += postojiOffsetSekIII((*iter6).getSekcija(), (*iter6).i);
				trenutnaAdresa += postojiOffsetSekZASVE((*iter6).getSekcija());
				prepravkaKod  -= trenutnaAdresa;
			}
			
		}
		else {
			prepravkaKod =postojiOffsetSekZASVE(sim.getSekcija());
			prepravkaKod += postojiOffsetSekIII(sim.getSekcija(), (*iter6).i);  ///dodatooooo  radi za neki primer
			if ((*iter6).getAddend() == -2) {
				prepravkaKod -= 2;
				int trenutnaAdresa = 0;
				trenutnaAdresa = (*iter6).getOffset();
				//trenutnaAdresa += postojiOffsetSekIII((*iter6).getSekcija(), (*iter6).i); 
				trenutnaAdresa += postojiOffsetSekZASVE((*iter6).getSekcija());
				prepravkaKod -= trenutnaAdresa;
			}
		}
		//prepravi- ali tako sto ces ono sto se tamo nalazi sabrati sa prepravkaKoda
		int lok = (*iter6).getOffset();
		string prepravisek = (*iter6).getSekcija();
		//////prekopiraj iz linkable////////////////////////////////////////////////////////
		string kodic = dohvKodKodaSekcije(prepravisek);
		
		string s8, s9;
		if ((*iter6).getTipRealokacije() == "R_X86_64noninst_16") {
			s8 = kodic.substr(lok * 2, 2);
			s9 = kodic.substr(lok * 2 + 2, 2);
		}
		else {
			s9 = kodic.substr(lok * 2, 2);
			s8 = kodic.substr(lok * 2 + 2, 2);
		}

		

		int dodaj = stoi(s9 + s8, nullptr, 16); //sta se vec tamo nalazi

		string s5;
		char hex_string[20];

		sprintf(hex_string, "%X", prepravkaKod + dodaj); //convert number to hex
		s5 = hex_string;
		for (int i = s5.size(); i < 4; i++) {
			s5 = "0" + s5;
		}
		if ((*iter6).getTipRealokacije() == "R_X86_64noninst_16")
		{
			kodic[lok * 2] = s5[2];
			kodic[lok * 2 + 1] = s5[3];
			kodic[lok * 2 + 2] = s5[0];
			kodic[lok * 2 + 3] = s5[1];
		}
		else {
			if (s5.size() == 8) {
				kodic[lok * 2] = s5[4];
				kodic[lok * 2 + 1] = s5[5];
				kodic[lok * 2 + 2] = s5[6];
				kodic[lok * 2 + 3] = s5[7];
			}
			else {
				kodic[lok * 2] = s5[0];
				kodic[lok * 2 + 1] = s5[1];
				kodic[lok * 2 + 2] = s5[2];
				kodic[lok * 2 + 3] = s5[3];
			}
		}
		setujKodSekcije(prepravisek, kodic);
		/////////////////////////////////////////////////////////////////////////////////////
	}

	//lepo ispisi sve
	cout << hextabelasimbola<<endl;
	cout << hextabelasekcija<<endl;
	cout << svirelzapisi;
	
	preuredikodoveHEX();
	ispisiKodoveHEX();
	
	//ispis kodova na formatiran nacin  ADRESA-SADRZAJ
	// 0000: 00 01 02 03 ...07
	//0008: 
}



//////////hex
void Linker::hex(list<string> ofajlovi, list<string> place, string smesti)
{
	int i = ofajlovi.size();
	ulaznifajlVelicina = i;
	ulaznifajlovi = new ifstream[i];

	list <string> ::iterator iter;
	
	int j = 0;
	for ( iter = ofajlovi.begin(); j < i; j++, ++iter) {
		ulaznifajlovi[j].open((*iter));
		if (ulaznifajlovi[j].is_open() == false) {
			
			cout << "Neuspesno otvoreni fajlovi";
			exit(1);
		}

	}

	for (iter = place.begin(); iter != place.end(); ++iter) {
		int index = -1, index1=-1; // za =, za @

		string linija = (*iter);
		for (int i = 0; i < linija.length(); i++)
		{
			if (linija.at(i) == '=') {
				index = i;
				break;
			}
		}
		int vel1 = linija.size() - index;
		string novalinija = linija.substr(index, vel1);
		for (int i = 0; i < novalinija.length(); i++)
		{
			if (novalinija.at(i) == '@') {
				index1 = i;
				break;
			}
		}

		int vel2 = novalinija.size() - index1;
		string ime = novalinija.substr(0+1,index1-1);
		string adresa = novalinija.substr(index1+1,vel2-1);
		Place p(ime, adresa);
		this->place.push_back(p);
		
	}

	  obradiHEX();






	for (int j = 0; j < i; j++) {
		ulaznifajlovi[j].close();
	}

	ofstream izlazni;
	izlazni.open(smesti);
	if (izlazni.is_open() == false) {
		cout << "Neuspesno otvoreni fajlovi";
		exit(1);
	}
	ispisiKodoveHEXPravilno( &izlazni);


	izlazni.close();
}


