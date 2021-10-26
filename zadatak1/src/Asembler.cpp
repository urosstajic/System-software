#define _CRT_SECURE_NO_WARNINGS
#include "Asembler.h"
#include <fstream>
#include <sstream>

using namespace std;
void Asembler::prvi_prolaz(ifstream& ulazfajl) {

	//regex
	regex neki_registar("^((r[0-7])|psw|sp|pc)$");
	regex dec_broj("(^[0-9]+)$");
	regex o_broj("^(0[0-9]+)$");
	regex heksa_broj("^(0x[0-9A-Fa-f]+)$");
	regex indirektno("^\\[r[0-7]\\]$");
	regex dolar_dec("^\\$[0-9]+$");
	regex dolar_okt("^\\$0[0-9]+$");
	regex dolar_heksa("^\\$0x[0-9A-Fa-f]+$");
	regex neki_tekst("^[a-zA-Z][a-zA-Z0-9_]*$");
	regex posto_tekst("^\\%[a-zA-Z][a-zA-Z0-9_]*$");
	regex dolar_tekst("^\\$[a-zA-Z][a-zA-Z0-9_]*$");
	regex zvezdica_tekst("^\\*[0-9]+$");
	//kraj regex

	string red;
	int allocation_counter = 0;
	int curr_section=-3;
	string curr_section_naziv = "";
	int br_linije = 1;
	while (getline(ulazfajl, red)) {
		int nadji_tarabu = red.find_first_of('#');
		if (nadji_tarabu != string::npos) {
			string h = red.substr(0, nadji_tarabu);
			red = h;
		}

		istringstream pomoc(red);

		size_t pom = red.find(':');
		//size_t pom2 = red.find('.');

		string koja_instr;
		pomoc >> koja_instr;

		
		if (pom == string::npos) {
			//nije pronadjen je : sto znaci da nije tipa msg:
			if (koja_instr==".section") {//pronadjena je .section
				string kojasekcija;
				pomoc >> kojasekcija;
				//ovo izmeniti, da ne bude ovako kako ja dajem random brojeve
				//treba i da se doda novo u tabeli sekcija, ali moram da vidim za size i te stvari kako ide
				if (curr_section !=-3) {
					Sekcija& s = tabela_sekcija.nadjiSekcijuZaPromenuVelicine(curr_section);
					s.postaviVelicinu(allocation_counter);
				}
				if (tabela_sekcija.da_li_postoji_sekcija(kojasekcija)) {//postoji sekcija
					if (kojasekcija == "") {
						cout << "Nije dobro uneta direktiva .section na liniji" << br_linije;
						exit(1);
					}
					
					//treba da dodam nesto ako je heksa vrednost, moram videti sta
					allocation_counter = 0;
					curr_section = tabela_sekcija.nadjiSekciju(kojasekcija).getRbr();
					curr_section_naziv = kojasekcija;
					if (!tabela_simb.da_li_postoji_simbol(kojasekcija)) {
						
						tabela_simb.dodajSimb(Simb(kojasekcija, curr_section, allocation_counter, "local"));
						tabela_simb.dohvatiSimbolZaMenjanje(kojasekcija).postaviNazivSekcije(curr_section_naziv);
					}
					else {
						cout << "Sekcija " << kojasekcija << " je visestruko definisana na liniji "<<br_linije;
						exit(1);
					}
				}
				else {//ne postoji sekcija
					if (kojasekcija == "") {
						cout << "Nije dobro uneta direktiva .section na liniji" << br_linije;
						exit(1);
					}
					allocation_counter = 0;
					Sekcija s = Sekcija(kojasekcija);
					tabela_sekcija.dodajSekciju(s);
					curr_section = s.getRbr();
					curr_section_naziv = kojasekcija;
					if (!tabela_simb.da_li_postoji_simbol(kojasekcija)) {

						tabela_simb.dodajSimb(Simb(kojasekcija, curr_section, allocation_counter, "local"));
						tabela_simb.dohvatiSimbolZaMenjanje(kojasekcija).postaviNazivSekcije(curr_section_naziv);
					}
					else {
						cout << "Sekcija " << kojasekcija << " je visestruko definisana na liniji "<<br_linije;
						exit(1);
					}
				}
				
				
			}
			else {
				if (koja_instr==".global") {
					br_linije++;
					continue;
				}
				else if (koja_instr==".extern") {
					string labela;
					pomoc >> labela;
					vector<string> s;
					if (labela[labela.length()-1]==',') {
						while (labela != "") {
							string novi;
							string tren_bez_menjanja = labela;
							if (labela[labela.length() - 1] == ',') {
								labela = labela.substr(0, labela.length() - 1);
							}
							if (!tabela_simb.da_li_postoji_simbol(labela)) {
								Simb s = Simb(labela, -1, -1, "global");
								tabela_simb.dodajSimb(s);
								tabela_simb.dohvatiSimbolZaMenjanje(labela).postaviNazivSekcije("UND");
								pomoc >> novi;
								if (novi=="") {
									if (tren_bez_menjanja[tren_bez_menjanja.length() - 1] == ',') {
										cout << "Simbol nije u odgovarajucem formatu na liniji " << br_linije;
										exit(1);
									}
								}
								labela = novi;
							}
							else {
								cout << "Simbol je visestruko definisan na liniji " << br_linije;
								exit(1);
							}
							
							
						}
					}
					else {
						if (labela.find(',') == string::npos) {
							if (!tabela_simb.da_li_postoji_simbol(labela)) {
								Simb s = Simb(labela, -1, -1, "global");
								tabela_simb.dodajSimb(s);
								tabela_simb.dohvatiSimbolZaMenjanje(labela).postaviNazivSekcije("UND");
							}
							else {
								cout << "Simbol je visestruko definisan na liniji " << br_linije;
								exit(1);
							}
						}
						else {

							//ovo ne radi jer ima razmaka
							while (labela.find(',') != string::npos || labela != "") {
								if (labela.find(',') == string::npos && labela != "") {
									s.push_back(labela);
									labela = "";
								}
								else {
									if (labela.find(',') != string::npos) {
										int duzina = labela.length();
										int pozicija = labela.find(",");
										string token = labela.substr(0, pozicija);
										s.push_back(token);
										labela = labela.substr(pozicija + 1, duzina);
									}
								}

							}
							for (auto i = s.begin(); i < s.end(); i++) {
								if (!this->tabela_simb.da_li_postoji_simbol(*i)) {
									Simb s = Simb(*i, -1, -1, "global");
									tabela_simb.dodajSimb(s);
									tabela_simb.dohvatiSimbolZaMenjanje(labela).postaviNazivSekcije("UND");
								}
								else {
									cout << "Simbol je visestruko definisan na liniji " << br_linije;
									exit(1);
								}
							}
						}
					}
				}
				else if (koja_instr==".word") {//prvo uradi relokacione zapise pa onda ovo
					string simbol;
					pomoc >> simbol;
					vector<string> s;
					if (simbol[simbol.length() - 1] == ',') {
						while (simbol != "") {
							string novi;
							string tren_bez_menjanja = simbol;
							if (simbol[simbol.length() - 1] == ',') {
								simbol = simbol.substr(0, simbol.length() - 1);
							}
							s.push_back(simbol);
							pomoc >> novi;
							if (novi == "") {
								if (tren_bez_menjanja[tren_bez_menjanja.length() - 1] == ',') {
									cout << "Simbol nije u odgovarajucem formatu na liniji " << br_linije;
									exit(1);
								}
							}
							simbol = novi;
						}
					}
					else {
						while (simbol.find(',') != string::npos || simbol != "") {
							if (simbol.find(',') == string::npos && simbol != "") {
								s.push_back(simbol);
								simbol = "";
							}
							else {
								if (simbol.find(',') != string::npos) {
									int duzina = simbol.length();
									int pozicija = simbol.find(",");
									string token = simbol.substr(0, pozicija);
									s.push_back(token);
									simbol = simbol.substr(pozicija + 1, duzina);
									//cout << simbol << endl;
								}
							}

						}
					}
					//cout << "Velicina onog je " << s.size();
					allocation_counter += s.size() * 2;
					
				}
				else if (koja_instr==".skip") {//ubaciti proveru da li je instrukcija lepo napisana
					string vrednost;
					pomoc >> vrednost;
					int vred_int=-1;
					if (vrednost[0] == '0' && vrednost[1] == 'x') {
						string novi = vrednost.substr(2, vrednost.length());
						char* c = const_cast<char*>(novi.c_str());
						vred_int = (int)strtol(c, nullptr, 16);
					}
					else if (regex_match(vrednost, o_broj)) {
						char* c = const_cast<char*>(vrednost.c_str());
						vred_int = (int)strtol(c, nullptr, 8);
					}
					else {
						vred_int = stoi(vrednost);
					}

					allocation_counter += vred_int;
					//cout << allocation_counter << "\n";
				}
				else if (koja_instr==".equ") {//sta mi radi equ
					string simbol, vrednost;
					pomoc >> simbol;
					simbol = simbol.substr(0, simbol.length()-1);
					pomoc >> vrednost;

					int vred_int = -1;
					if (vrednost.at(0) == '0' && vrednost.at(1) == 'x') {
						if (!regex_match(vrednost, heksa_broj)) {
							cout << "Nije unet odgovarajuci literal na liniji " << br_linije;
							exit(1);
						}
						string novi = vrednost.substr(2, vrednost.length());
						char* c = const_cast<char*>(novi.c_str());
						vred_int = (int)strtol(c, NULL, 16);
					}
					else if (vrednost[0] == '0' && vrednost.length() != 1 && vrednost[1]!='x') {
						if (!regex_match(vrednost, o_broj)) {
							cout << "Nije unet odgovarajuci literal na liniji " << br_linije;
							exit(1);
						}
						char* c = const_cast<char*>(vrednost.c_str());
						vred_int = (int)strtol(c, NULL, 8);
					}
					else {
						if (!regex_match(vrednost, dec_broj)) {
							cout << "Nije unet odgovarajuci literal na liniji " << br_linije;
							exit(1);
						}
						vred_int = stoi(vrednost);
					}

					if (!tabela_simb.da_li_postoji_simbol(simbol)) {
						tabela_simb.dodajSimb(Simb(simbol, -2, vred_int, "local"));
						tabela_simb.dohvatiSimbolZaMenjanje(simbol).postaviNazivSekcije("APS");
					}
					else {
						cout << "Simbol je visestruko definisan na liniji " << br_linije;
						exit(1);
					}
				}
				else if (koja_instr==".end") {//da li uopste nesto treba da pise u end?
				if (curr_section != -3) {
					Sekcija& s = tabela_sekcija.nadjiSekcijuZaPromenuVelicine(curr_section);
					s.postaviVelicinu(allocation_counter);
				}
				}
				else {//instrukcije kao push i to
					if (koja_instr == "halt" || koja_instr == "ret" || koja_instr == "iret") {
						string provera;
						pomoc >> provera;
						if (provera == "") {
							//sve ok
							allocation_counter++;
						}
						else {
							cout << "Nije korektno napisana jednobajtna instrukcija na liniji " << br_linije;
							exit(1);
						}
					}
					else if (koja_instr == "int") {
						string provera;
						pomoc >> provera;
						if (provera != "") {
							string opet_provera;
							pomoc >> opet_provera;
							if (opet_provera != "") {
								cout << "Nije korektno napisana instrukcija int na liniji " << br_linije;
								exit(1);
							}
							if (!regex_match(provera, neki_registar)) {
								cout << "Nije korektno napisan operand int na liniji " << br_linije;
								exit(1);
							}
							allocation_counter += 2;
						}
						else {
							cout << "Nije korektno napisana instrukcija int na liniji " << br_linije;
							exit(1);
						}
					}
					else if (koja_instr == "call") {
						string provera;
						pomoc >> provera;
						if (provera == "") {
							cout << "Nije korektno napisana instrukcija call na liniji " << br_linije;
							exit(1);
						}
						else {
							if (provera.find('[') == string::npos) {
								string opet_provera;
								pomoc >> opet_provera;
								if (opet_provera != "") {
									cout << "Nije korektno napisana instrukcija call na liniji " << br_linije;
									exit(1);
								}
							}
							
							//treba da ubacim provere za one regexe
							if (provera.find('*') != string::npos) {
								if (provera[0] != '*') {
									cout << "Nije korektno napisana instrukcija call na liniji " << br_linije;
									exit(1);
								}
								string novi = provera.substr(1, provera.size());
								provera = novi;
								if (regex_match(provera, neki_registar) || regex_match(provera, indirektno)) {
									allocation_counter += 3;
								}
								else if (regex_match(provera, dec_broj) || regex_match(provera, o_broj) || regex_match(provera, heksa_broj) || regex_match(provera, neki_tekst)) {
									allocation_counter += 5;
								}
								else {
									//ovde jos nesto treba ali nzm sta
									string plus, simbol;
									pomoc >> plus;
									pomoc >> simbol;
									if (provera.find('[') != string::npos) {
										provera = provera.substr(1, provera.length());
									}
									if (!regex_match(provera, neki_registar)) {
										cout << "Nije korektno napisana instrukcija call na liniji " << br_linije;
										exit(1);
									}
									if (plus != "+") {
										cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
										exit(1);
									}
									if (simbol[simbol.length() - 1] != ']') {
										cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
										exit(1);
									}
									simbol = simbol.substr(0, simbol.length() - 1);
									if (regex_match(simbol, dec_broj) || regex_match(simbol, o_broj) || regex_match(simbol, heksa_broj) || regex_match(simbol, neki_tekst)) {
										allocation_counter += 5;
									}
									else {
										cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
										exit(1);
									}
								}
							}
							else {//i ovde fali
								if (regex_match(provera, dec_broj) || regex_match(provera, o_broj) || regex_match(provera, heksa_broj) || regex_match(provera, posto_tekst) || regex_match(provera, neki_tekst)) {
									allocation_counter += 5;
								}
								else {
									cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
									exit(1);
								}
								
							}
						}
					}
					else if (koja_instr == "add" || koja_instr == "sub" || koja_instr == "div" || koja_instr == "mul" || koja_instr == "xchg" || koja_instr == "and" || koja_instr == "xor" || koja_instr == "or" || koja_instr == "test" || koja_instr == "shr" || koja_instr == "shl" || koja_instr=="cmp") {
						string regD, regS, nova_provera;
						pomoc >> regD;
						pomoc >> regS;
						pomoc >> nova_provera;
						if (nova_provera != "") {
							cout << "Nije dobar format instrukcije na liniji " << br_linije;
							exit(1);
						}
						if (regD[regD.length() - 1] != ',') {
							cout << "Nije dobar format instrukcije na liniji " << br_linije;
							exit(1);
						}
						regD = regD.substr(0, regD.size() - 1);
						//proveri jel su u dobrom formatu ova dva
						if (!regex_match(regD, neki_registar) || !regex_match(regS, neki_registar)) {
							cout << "Nije dobar format operanada na liniji " << br_linije;
							exit(1);
						}
						allocation_counter += 2;
						//da li da proveravam tipa ako je add nesto , nesto
					}
					else if (koja_instr == "not") {
						string regD, nova_provera;
						pomoc >> regD;
						pomoc >> nova_provera;
						if (nova_provera != "") {
							cout << "Nije dobar format instrukcije not na liniji " << br_linije;
							exit(1);
						}
						if (!regex_match(regD, neki_registar)) {
							cout << "Nije dobar format operanda na liniji " << br_linije;
							exit(1);
						}
						allocation_counter += 2;
					}
					else if (koja_instr == "push" || koja_instr == "pop") {
						string regD, nova_provera;
						pomoc >> regD;
						pomoc >> nova_provera;
						if (nova_provera != "") {
							cout << "Nije dobar format instrukcije na liniji " << br_linije;
							exit(1);
						}
						if (!regex_match(regD, neki_registar)) {
							cout << "Nije dobar format operanda na liniji " << br_linije;
							exit(1);
						}
						allocation_counter += 3;
					}
					else if (koja_instr == "jmp" || koja_instr == "jne" || koja_instr == "jgt" || koja_instr == "jeq") {
						string operand;
						pomoc >> operand;
						if (operand == "") {
							cout << "Nije korektno napisana instrukcija call na liniji " << br_linije;
							exit(1);
						}
						else {
							if (operand.find('[') == string::npos) {
								string opet_provera;
								pomoc >> opet_provera;
								if (opet_provera != "") {
									cout << "Nije korektno napisana instrukcija na liniji " << br_linije;
									exit(1);
								}
							}
							if (operand.find('*') != string::npos) {
								string novi = operand.substr(1, operand.size());
								operand = novi;
								if (regex_match(operand, neki_registar) || regex_match(operand, indirektno)) {
									allocation_counter += 3;
								}
								else if (regex_match(operand, dec_broj) || regex_match(operand, o_broj) || regex_match(operand, heksa_broj) || regex_match(operand, neki_tekst)) {
									allocation_counter += 5;
								}
								else {
									//ovde jos nesto treba ali nzm sta
									if (operand.find('[') != string::npos) {
										operand = operand.substr(1, operand.length());
									}
									//string novi = operand.substr(1, operand.size());
									//operand = novi;
									if (!regex_match(operand, neki_registar)) {
										cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
										exit(1);
									}
									string plus, simbol;
									pomoc >> plus;
									pomoc >> simbol;
									if (plus != "+") {
										cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
										exit(1);
									}
									if (simbol[simbol.length() - 1] != ']') {
										cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
										exit(1);
									}
									simbol = simbol.substr(0, simbol.length() - 1);
									if (regex_match(simbol, dec_broj) || regex_match(simbol, o_broj) || regex_match(simbol, heksa_broj) || regex_match(simbol, neki_tekst)) {
										allocation_counter += 5;
									}
									else {
										cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
										exit(1);
									}
								}
							}
							else {
								if (regex_match(operand, dec_broj) || regex_match(operand, o_broj) || regex_match(operand, heksa_broj) || regex_match(operand, posto_tekst) || regex_match(operand, neki_tekst)) {
									allocation_counter += 5;
								}
								else {
									cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
									exit(1);
								}
								
							}
						}
					}
					else if (koja_instr == "ldr" || koja_instr == "str") {//treba da se izmeni smamo da pitam prvo u kom obliku moze biti [ + ]
						string regD, operand, opet_provera;
						pomoc >> regD;
						pomoc >> operand;
						pomoc >> opet_provera;
						if (opet_provera != "") {
							cout << "Nije korektno napisana instrukcija na liniji " << br_linije;
							exit(1);
						}
						if (operand != "" && regD != "") {
							if (regD[regD.length() - 1] != ',') {
								cout << "Nije korektno napisana instrukcija na liniji " << br_linije;
								exit(1);
							}
							regD = regD.substr(0, regD.size()-1);
							if (!regex_match(regD, neki_registar)) {
								cout << "Nije korektno napisana instrukcija na liniji " << br_linije;
								exit(1);
							}
							if (regex_match(operand, neki_registar) || regex_match(operand, indirektno)) {
								allocation_counter += 3;
							}
							else if (regex_match(operand, dec_broj) || regex_match(operand, o_broj) || regex_match(operand, heksa_broj)|| regex_match(operand, neki_tekst) || regex_match(operand, dolar_dec) || regex_match(operand, dolar_heksa) || regex_match(operand, dolar_okt) || regex_match(operand, dolar_tekst) || regex_match(operand, posto_tekst)) {
								allocation_counter += 5;
							}
							else {
								string plus, simbol;
								pomoc >> plus;
								pomoc >> simbol;
								if (regD.find('[') != string::npos) {
									regD = regD.substr(1, regD.length());
								}
								if (!regex_match(regD, neki_registar)) {
									cout << "Nije korektno napisan format operanda na liniji " << br_linije;
									exit(1);
								}
								if (plus != "+") {
									cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
									exit(1);
								}
								if (simbol[simbol.length() - 1] != ']') {
									cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
									exit(1);
								}
								simbol = simbol.substr(0, simbol.length() - 1);
								if (regex_match(simbol, dec_broj) || regex_match(simbol, o_broj) || regex_match(simbol, heksa_broj) || regex_match(simbol, neki_tekst)) {
									allocation_counter += 5;
								}
								else {
									cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
									exit(1);
								}
							}
						}
						else {
							cout << "Nije korektno napisana instrukcija na liniji " << br_linije;
							exit(1);
						}
					}
				}
			}
		}
		else {
			//pronadjene su dve tacke
			if (koja_instr[koja_instr.length() - 1] != ':') {	
				cout << "Nije korektno napisan operand instrukcije na liniji " << br_linije;
				exit(1);
			}
			koja_instr = koja_instr.substr(0, koja_instr.size() - 1);
			if (!tabela_simb.da_li_postoji_simbol(koja_instr)) {
				Simb s = Simb(koja_instr, curr_section, allocation_counter, "local");
				s.postaviNazivSekcije(curr_section_naziv);
				tabela_simb.dodajSimb(s);
				//tabela_simb.dohvatiSimbolZaMenjanje(koja_instr).postaviNazivSekcije(curr_section_naziv);
			}
			else {
				cout << "Simbol je visestruko definisan na liniji " << br_linije;
				exit(1);
			}
		}
		br_linije++;
	}
}

void Asembler::drugi_prolaz(ifstream& ulazfajl, string ime_izlaznog) {
	//cout << "Drugi prolaz" << endl;
	//regex
	regex neki_registar("^((r[0-7])|pc|sp)$");
	regex dec_broj("(^[0-9]+)$");
	regex o_broj("^(0[0-9]+)$");
	regex heksa_broj("^(0x[0-9A-Fa-f]+)$");
	regex indirektno("^\\[r[0-7]\\]$");
	regex dolar_dec("^\\$[0-9]+$");
	regex dolar_okt("^\\$0[0-9]+$");
	regex dolar_heksa("^\\$0x[0-9A-Fa-f]+$");
	regex neki_tekst("^[a-zA-Z][a-zA-Z0-9_]*$");
	regex posto_tekst("^\\%[a-zA-Z][a-zA-Z0-9_]*$");
	regex dolar_tekst("^\\$[a-zA-Z][a-zA-Z0-9_]*$");
	regex zvezdica_tekst("^\\*[0-9]+$");
	//kraj regex

	string red;
	int allocation_counter = 0;
	int curr_section = -3;
	int br_linije = 1;
	int kraj = 0;
	while (getline(ulazfajl, red)) {
		int nadji_tarabu = red.find_first_of('#');
		if (nadji_tarabu != string::npos) {
			string h = red.substr(0, nadji_tarabu);
			red = h;
		}
		istringstream pomoc(red);
		
		if (red == "") {
			continue;
		}
		if (red[red.length() - 1] == ':') {
			continue;
		}
		string koja_instr;
		pomoc >> koja_instr;
		//cout << "ovo je instrukcija " << koja_instr;
		if (koja_instr == ".end") {
			kraj = 1;
			break;
		}
		else if (koja_instr == ".global") {
			//cout << "u global";
			string labela;
			pomoc >> labela;
			vector<string> s;
			if (labela[labela.length() - 1] == ',') {
				while (labela != "") {
					string novi;
					if (labela[labela.length() - 1] == ',') {
						labela = labela.substr(0, labela.length() - 1);
					}
					if (!tabela_simb.da_li_postoji_simbol(labela)) {
						
						cout << "Ne postoji simbol u tabeli simbola";
						exit(1);
					}
					else {
						tabela_simb.simbolGlobalan(labela);
						//tabela_simb.dohvatiSimbolZaMenjanje(labela).postaviNazivSekcije("UND");
						pomoc >> novi;
						labela = novi;
					}
				}
			}
			else {//
				if (labela.find(',') == string::npos) {
					if (!tabela_simb.da_li_postoji_simbol(labela)) {
						cout << "Nije definisan simbol u tabeli simbola";
						exit(1);
					}
					else {
						tabela_simb.simbolGlobalan(labela);
						//tabela_simb.dohvatiSimbolZaMenjanje(labela).postaviNazivSekcije("UND");
					}
				}
				else {
					//ovo ne radi jer ima razmaka
					while (labela.find(',') != string::npos || labela != "") {
						if (labela.find(',') == string::npos && labela != "") {
							s.push_back(labela);
							labela = "";
						}
						else {
							if (labela.find(',') != string::npos) {
								int duzina = labela.length();
								int pozicija = labela.find(",");
								string token = labela.substr(0, pozicija);
								s.push_back(token);
								labela = labela.substr(pozicija + 1, duzina);
							}
						}

					}
					for (auto i = s.begin(); i < s.end(); i++) {
						if (!this->tabela_simb.da_li_postoji_simbol(*i)) {
							cout << "Ne postoji simbol u tabeli simbola";
							exit(1);
						}
						else {
							tabela_simb.simbolGlobalan(*i);
						}
					}
				}
			}
		}
		else if (koja_instr == ".extern") {
			continue;
		}
		else if (koja_instr == ".skip") {
			//cout << "Uslooo";
			string vrednost;
			pomoc >> vrednost;
			int vred_int = -1;
			if (vrednost[0] == '0' && vrednost[1] == 'x') {
				string novi = vrednost.substr(2, vrednost.length());
				char* c = const_cast<char*>(novi.c_str());
				vred_int = (int)strtol(c, nullptr, 16);
			}
			else if (regex_match(vrednost, o_broj)) {
				char* c = const_cast<char*>(vrednost.c_str());
				vred_int = (int)strtol(c, nullptr, 8);
			}
			else {
				vred_int = stoi(vrednost);
			}

			allocation_counter += vred_int;
			string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
			if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
				SadrzajSekcije s = SadrzajSekcije(imesek);
				this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
			}
			//ovaj deo sada lepo istestirati, ne znam da li ce negde da fali & i to
			SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);
			
			for (int i = 0; i < vred_int; i++) {
				tek.dodajKod("00");
			}//cout << "blaa" << tek;
		}
		else if (koja_instr == ".word") {//moracu malo vise da skapiram drugi prolaz, da bih ovo odradio
			string labela;
			pomoc >> labela;
			string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
			if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
				SadrzajSekcije s = SadrzajSekcije(imesek);
				this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
			}
			//string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
			SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);
			vector<string> s;
			if (labela[labela.length() - 1] == ',') {
				while (labela != "") {
					string novi;
					if (labela[labela.length() - 1] == ',') {
						labela = labela.substr(0, labela.length() - 1);
					}

					if (regex_match(labela, dec_broj) || regex_match(labela, o_broj) || regex_match(labela, heksa_broj)) {
						allocation_counter += 2;
						int vred_int;
						if (labela[0] == '0' && labela[1] == 'x') {
							string novi = labela.substr(2, labela.length());
							char* c = const_cast<char*>(novi.c_str());
							vred_int = (int)strtol(c, nullptr, 16);
						}
						else if (regex_match(labela, o_broj)) {
							char* c = const_cast<char*>(labela .c_str());
							vred_int = (int)strtol(c, nullptr, 8);
						}
						else {
							vred_int = stoi(labela);
						}
						string heksa = this->decToHexa(vred_int);
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
						tek.dodajKod(krajnje);
					}
					else if (regex_match(labela, neki_tekst)) {
						//cout << "Uslo ovde";
						if (!tabela_simb.da_li_postoji_simbol(labela)) {
							cout << "Ne postoji simbol u tabeli simbola";
							exit(1);
						}
						if (tabela_simb.dohvatiSimbol(labela).dohvatiLocGlob() == "local") {
							Simb tren_simb = tabela_simb.dohvatiSimbol(labela);
							string ime_sek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
							RelokacioniZapis z(curr_section, allocation_counter, "R_X86_64noninst_16", tabela_simb.dohvatiSimbol(labela).dohvatiSekciju(), 0);//ovde ima jos jedno polje false
							z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(labela).dohvatiSekciju()).getNaziv());//da li je ovo greska
							z.postaviImeSekcije(ime_sek);
							tab_rel_zapisa.dodajRelokacioniZapis(z);

							int ofs = tabela_simb.dohvatiSimbol(labela).dohvatiOfset();
							//cout << "Ovo je ofset" + ofs;
							string heksa = decToHexa(ofs);
							string nesto = heksa;
							string krajnje = "";
							if (nesto.length() < 4) {
								int nes = 4 - nesto.length();
								for (int i = 0; i < nes; i++) {
									krajnje += "0";
								}
								krajnje += nesto;
							}
							else {
								krajnje = nesto;
							}
							tek.dodajKod(krajnje.substr(2, 4) + krajnje.substr(0, 2));
						}
						else {
							tek.dodajKod("0000");
							RelokacioniZapis z(curr_section, allocation_counter, "R_X86_64noninst_16", tabela_simb.dohvatiSimbol(labela).dohvatiRbr(), 0);
							z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
							z.postaviImeSimbola(labela);
							tab_rel_zapisa.dodajRelokacioniZapis(z);
						}
						allocation_counter += 2;
					}

					pomoc >> novi;
					labela = novi;
				}
			}
			else {
				if (regex_match(labela, dec_broj) || regex_match(labela, o_broj) || regex_match(labela, heksa_broj)) {
					allocation_counter += 2;
					int vred_int;
					if (labela[0] == '0' && labela[1] == 'x') {
						string novi = labela.substr(2, labela.length());
						char* c = const_cast<char*>(novi.c_str());
						vred_int = (int)strtol(c, nullptr, 16);
					}
					else if (regex_match(labela, o_broj)) {
						char* c = const_cast<char*>(labela.c_str());
						vred_int = (int)strtol(c, nullptr, 8);
					}
					else {
						vred_int = stoi(labela);
					}
					string heksa = this->decToHexa(vred_int);
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
					tek.dodajKod(krajnje);
				}
				else if (regex_match(labela, neki_tekst)) {
					//cout << "Ovo je labela " + labela;
					if (!tabela_simb.da_li_postoji_simbol(labela)) {
						cout << "Ne postoji simbol u tabeli simbola";
						exit(1);
					}
					if (tabela_simb.dohvatiSimbol(labela).dohvatiLocGlob() == "local") {
						Simb tren_simb = tabela_simb.dohvatiSimbol(labela);
						string ime_sek = tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(labela).dohvatiSekciju()).getNaziv();
						RelokacioniZapis z(curr_section, allocation_counter, "R_X86_64noninst_16", tabela_simb.dohvatiSimbol(labela).dohvatiSekciju(), 0);//ovde ima jos jedno polje false
						z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
						z.postaviImeSimbola(ime_sek);
						tab_rel_zapisa.dodajRelokacioniZapis(z);
						//Simb prov = tabela_simb.dohvatiSimbol(labela);
						int ofs = tabela_simb.dohvatiSimbol(labela).dohvatiOfset();
						//cout << "Ovo je ofset " + ofs<<endl;
						string heksa = decToHexa(ofs);
						string nesto = heksa;
						string krajnje = "";
						if (nesto.length() < 4) {
							int nes = 4 - nesto.length();
							for (int i = 0; i < nes; i++) {
								krajnje += "0";
							}
							krajnje += nesto;
						}
						else {
							krajnje = nesto;
						}
						tek.dodajKod(krajnje.substr(2, 4) + krajnje.substr(0, 2));
						allocation_counter += 2;
					}
					else {
						//cout << "Ovo je za global";
						tek.dodajKod("0000");
						RelokacioniZapis z(curr_section, allocation_counter, "R_X86_64noninst_16", tabela_simb.dohvatiSimbol(labela).dohvatiRbr(), 0);
						z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
						z.postaviImeSimbola(labela);
						tab_rel_zapisa.dodajRelokacioniZapis(z);
						allocation_counter += 2;
					}
				}
			}

		}
		else if (koja_instr == ".section") {
			string koja_sekcija;
			pomoc >> koja_sekcija;
			allocation_counter = 0;
			//cout << koja_sekcija;
			curr_section = tabela_sekcija.nadjiSekciju(koja_sekcija).getRbr();
			//cout << "Curr section je " << curr_section<<endl;
		}
		else if (koja_instr == ".equ") {
			continue;
		}
		else {//instrukcije kao push, pop i to
			string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
			if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
				SadrzajSekcije s = SadrzajSekcije(imesek);
				this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
			}
			if (koja_instr == "halt" || koja_instr == "ret" || koja_instr == "iret") {
				//string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
				SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);
				if (koja_instr == "halt")tek.dodajKod("00");
				if (koja_instr == "ret") tek.dodajKod("40");
				if (koja_instr == "iret") tek.dodajKod("20");
				allocation_counter++;
			}
			else if (koja_instr == "int") {
				string regD;
				pomoc >> regD;
				/*string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
				if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
					SadrzajSekcije s = SadrzajSekcije(imesek);
					this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
				}*/
				SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);
				tek.dodajKod("10");
				if (regD == "psw") {
					tek.dodajKod("8");
				}
				else if (regD == "pc") {
					tek.dodajKod("6");
				}
				else if (regD == "sp") {
					tek.dodajKod("7");
				}
				else {
					tek.dodajKod(regD.substr(1, 1));
				}
				tek.dodajKod("F");
				allocation_counter += 2;
			}
			else if (koja_instr == "add" || koja_instr == "sub" || koja_instr == "div" || koja_instr == "mul" || koja_instr == "xchg" || koja_instr == "and" || koja_instr == "xor" || koja_instr == "or" || koja_instr == "test" || koja_instr == "shr" || koja_instr == "shl" || koja_instr=="cmp") {
				/*string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
				if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
					SadrzajSekcije s = SadrzajSekcije(imesek);
					this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
				}*/
				SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);

				if (koja_instr == "add") tek.dodajKod("70");
				if (koja_instr == "sub") tek.dodajKod("71");
				if (koja_instr == "mul") tek.dodajKod("72");
				if (koja_instr == "div") tek.dodajKod("73");
				if (koja_instr == "cmp") tek.dodajKod("74");
				if (koja_instr == "and") tek.dodajKod("81");
				if (koja_instr == "or") tek.dodajKod("82");
				if (koja_instr == "xor") tek.dodajKod("83");
				if (koja_instr == "test") tek.dodajKod("84");
				if (koja_instr == "shl") tek.dodajKod("90");
				if (koja_instr == "shr") tek.dodajKod("91");
				if (koja_instr == "xchg") tek.dodajKod("60");

				string regD, regS;
				pomoc >> regD;
				pomoc >> regS;
				regD = regD.substr(0, regD.size() - 1);
				string s="";
				if (regD == "pc") {
					tek.dodajKod("6");
				}
				else if (regD == "sp") {
					tek.dodajKod("7");
				}
				else {
					s += regD.substr(1, 1);
				}
				if (regS == "pc") {
					tek.dodajKod("6");
				}
				else if (regS == "sp") {
					tek.dodajKod("7");
				}
				else {
					s += regS.substr(1, 1);
				}
				//string s = regD.substr(1, 1) + regS.substr(1, 1);
				tek.dodajKod(s);

				allocation_counter += 2;
			}
			else if (koja_instr == "not") {
				/*string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
				if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
					SadrzajSekcije s = SadrzajSekcije(imesek);
					this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
				}*/
				SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);

				tek.dodajKod("80");

				string regD;
				pomoc >> regD;
				string s = "";
				if (regD == "pc") {
					tek.dodajKod("66");
				}
				else if (regD == "sp") {
					tek.dodajKod("77");
				}
				else {
					s = regD.substr(1, 1) + regD.substr(1, 1);
				}
				
				tek.dodajKod(s);

				allocation_counter += 2;

			}
			else if (koja_instr == "push" || koja_instr == "pop") {
				string regD;
				pomoc >> regD;

				/*string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
				if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
					SadrzajSekcije s = SadrzajSekcije(imesek);
					this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
				}*/
				SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);
				string s = "";
				if (regD == "psw") {
					tek.dodajKod("8");
				}
				else if (regD == "pc") {
					tek.dodajKod("6");
				}
				else if (regD == "sp") {
					tek.dodajKod("7");
				}
				else {
					s += regD.substr(1, 1);
				}

				if (koja_instr == "push") {
					tek.dodajKod("B0");
					tek.dodajKod("6" + s);//sta znaci ova 6 ovde
					tek.dodajKod("12"); 
				}
				if (koja_instr == "pop") {
					tek.dodajKod("A0");
					tek.dodajKod(s + "6");//ispravi
					tek.dodajKod("42");
				}

				allocation_counter += 3;
			}
			else if (koja_instr == "ldr" || koja_instr == "str") {
				string regD, operand;
				pomoc >> regD;
				pomoc >> operand;
				/*string imesek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
				if (!this->lista_sadrz_sekcije.da_li_postoji_sadrzajSekcije(imesek)) {
					SadrzajSekcije s = SadrzajSekcije(imesek);
					this->lista_sadrz_sekcije.dodajSadrzajSekcije(s);
				}*/
				SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);
				regD = regD.substr(0, regD.length() - 1);

				if (koja_instr == "ldr") tek.dodajKod("A0");
				if (koja_instr == "str") tek.dodajKod("B0");

				if (regex_match(operand, neki_registar)) {
					allocation_counter += 3;
					string s = "";
					if (regD == "pc") {
						tek.dodajKod("6");
					}
					else if (regD == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += regD.substr(1, 1);
					}
					if (operand == "pc") {
						tek.dodajKod("6");
					}
					else if (operand == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += operand.substr(1, 1);
					}
					//string s = regD.substr(1, 1) + operand.substr(1, 1);
					tek.dodajKod(s);
					tek.dodajKod("01");
				}
				else if (regex_match(operand, indirektno)) {
					allocation_counter += 3;
					operand = operand.substr(1, operand.length());
					string s = "";
					if (regD == "pc") {
						tek.dodajKod("6");
					}
					else if (regD == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += regD.substr(1, 1);
					}
					if (operand == "pc") {
						tek.dodajKod("6");
					}
					else if (operand == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += operand.substr(1, 1);
					}
					//string s = regD.substr(1, 1) + operand.substr(1, 1);
					tek.dodajKod(s);
					tek.dodajKod("02");

				}
				else if (regex_match(operand, dec_broj) || regex_match(operand, o_broj) || regex_match(operand, heksa_broj)) {
					int vred_int;
					allocation_counter += 5;
					string s="";
					if (regD == "pc") {
						tek.dodajKod("6");
					}
					else if (regD == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += regD.substr(1, 1);
					}
					s += "9";//odakle ova 9
					tek.dodajKod(s);
					tek.dodajKod("04");//sto 04
					if (operand[0] == '0' && operand[1] == 'x') {
						string novi = operand.substr(2, operand.length());
						char* c = const_cast<char*>(novi.c_str());
						vred_int = (int)strtol(c, nullptr, 16);
					}
					else if (regex_match(operand, o_broj)) {
						char* c = const_cast<char*>(operand.c_str());
						vred_int = (int)strtol(c, nullptr, 8);
					}
					else {
						vred_int = stoi(operand);
					}
					string heksa = this->decToHexa(vred_int);
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
					tek.dodajKod(krajnje);
				}
				else if (regex_match(operand, neki_tekst)) {
					allocation_counter += 5; string s = "";
					if (regD == "pc") {
						tek.dodajKod("6");
					}
					else if (regD == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += regD.substr(1, 1);
					}
					s += "9";//odakle ova 9
					//string s = regD.substr(1, 1) + "9"; //sta ovo znaci
					tek.dodajKod(s);
					tek.dodajKod("04");
					if (!tabela_simb.da_li_postoji_simbol(operand)) {
						cout << "Ne postoji simbol u tabeli simbola";
						exit(1);
					}
					Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
					if (tren_simb.dohvatiSekciju() == -1) {
						tek.dodajKod("0000");
						RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
						z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
						z.postaviImeSimbola(operand);
						tab_rel_zapisa.dodajRelokacioniZapis(z);

					}
					else if (tren_simb.dohvatiSekciju() == -2) {
						//tu mi nije jasno sta treba da radim
						int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
						string heksa = this->decToHexa(ofs);
						string nesto = heksa;
						string krajnje = "";
						if (nesto.length() < 4) {
							int nes = 4 - nesto.length();
							for (int i = 0; i < nes; i++) {
								krajnje += "0";
							}
							krajnje += nesto;
						}
						else {
							krajnje = nesto;
						}
						tek.dodajKod(krajnje);
					}
					else {
						if (tabela_simb.dohvatiSimbol(operand).dohvatiLocGlob() == "local") {
							//Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
							if (tren_simb.dohvatiSekciju() != -2) {
								string ime_sek = tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(operand).dohvatiSekciju()).getNaziv();
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(ime_sek);
								tab_rel_zapisa.dodajRelokacioniZapis(z);

								int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string heksa = this->decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {//ako je equ
								int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string heksa = this->decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
						}
						else {
							tek.dodajKod("0000");
							RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
							z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
							z.postaviImeSimbola(operand);
							tab_rel_zapisa.dodajRelokacioniZapis(z);
						}
					}
				}
				else if (regex_match(operand, posto_tekst)) {
					int vred_int;
					allocation_counter += 5;
					operand = operand.substr(1, operand.length());
					string s = "";
					if (regD == "pc") {
						tek.dodajKod("6");
					}
					else if (regD == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += regD.substr(1, 1);
					}
					s += "7";//odakle ova 9
					//s = regD.substr(1, 1) + "7";
					tek.dodajKod(s);
					tek.dodajKod("05");
					if (!tabela_simb.da_li_postoji_simbol(operand)) {
						cout << "Ne postoji simbol u tabeli simbola";
						exit(1);
					}
					Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
					if (tren_simb.dohvatiSekciju() == -2) {
						//ne znam sta
						int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
						string heksa = decToHexa(ofs);
						string nesto = heksa;
						string krajnje = "";
						if (nesto.length() < 4) {
							int nes = 4 - nesto.length();
							for (int i = 0; i < nes; i++) {
								krajnje += "0";
							}
							krajnje += nesto;
						}
						else {
							krajnje = nesto;
						}
						tek.dodajKod(krajnje);
					}
					else {
						if (tabela_simb.dohvatiSimbol(operand).dohvatiLocGlob() == "local") {
							Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
							if (tren_simb.dohvatiSekciju() == curr_section) {
								int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								int mesto = pozicija - allocation_counter;
								string heksa = decToHexaZaNegativne(mesto);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {
								int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								int pc = allocation_counter - 2;
								int pomeraj = pozicija - pc;
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), -2);
								//RelokacioniZapis z(curr_section, pozicija - allocation_counter , "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), -2);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(operand).dohvatiSekciju()).getNaziv());
								tab_rel_zapisa.dodajRelokacioniZapis(z);

								int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string heksa = decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
						}
						else {
							if (tren_simb.dohvatiSekciju() == curr_section) {
								int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								int mesto = pozicija - allocation_counter;
								string heksa = decToHexaZaNegativne(mesto);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {
								tek.dodajKod("0000");
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), -2);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(operand);
								tab_rel_zapisa.dodajRelokacioniZapis(z);
							}
							
						}
					}
				}
				else if (regex_match(operand, dolar_tekst)) {
					int vred_int;
					allocation_counter += 5;
					operand = operand.substr(1, operand.length());
					string s = "";
					if (regD == "pc") {
						tek.dodajKod("6");
					}
					else if (regD == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += regD.substr(1, 1);
					}
					s += "9";//odakle ova 9
					//s = regD.substr(1, 1) + "9";
					tek.dodajKod(s);
					tek.dodajKod("00");
					if (!tabela_simb.da_li_postoji_simbol(operand)) {
						cout << "Ne postoji simbol u tabeli simbola";
						exit(1);
					}
					Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
					if (tren_simb.dohvatiSekciju() == -2) {
						//ne znam sta
						int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
						string heksa = decToHexa(ofs);
						string nesto = heksa;
						string krajnje = "";
						if (nesto.length() < 4) {
							int nes = 4 - nesto.length();
							for (int i = 0; i < nes; i++) {
								krajnje += "0";
							}
							krajnje += nesto;
						}
						else {
							krajnje = nesto;
						}
						tek.dodajKod(krajnje);
					}
					else if (tren_simb.dohvatiSekciju() == -1) {
						tek.dodajKod("0000");
						int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
						RelokacioniZapis z(curr_section, allocation_counter-2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
						z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
						z.postaviImeSimbola(operand);
						tab_rel_zapisa.dodajRelokacioniZapis(z);
					}
					else {
						if (tabela_simb.dohvatiSimbol(operand).dohvatiLocGlob() == "local") {
							Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
							if (tren_simb.dohvatiSekciju() != -2) {
								int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string ime_sek = tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv();
								RelokacioniZapis z(curr_section, allocation_counter - 2 , "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(operand).dohvatiSekciju()).getNaziv());
								tab_rel_zapisa.dodajRelokacioniZapis(z);

								int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string heksa = decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {//ako je equ
								int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string heksa = decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
						}
						else {
							tek.dodajKod("0000");
							RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
							z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
							z.postaviImeSimbola(operand);
							tab_rel_zapisa.dodajRelokacioniZapis(z);
						}
					}
				}
				else if (regex_match(operand, dolar_dec) || regex_match(operand, dolar_okt) || regex_match(operand, dolar_heksa)) {
					int vred_int;
					operand = operand.substr(1, operand.length());
					//cout << "    " + regD;
					allocation_counter += 5;
					string s = "";
					if (regD == "pc") {
						tek.dodajKod("6");
					}
					else if (regD == "sp") {
						tek.dodajKod("7");
					}
					else {
						s += regD.substr(1, 1);
					}
					s += "9";//odakle ova 9
					//s = regD.substr(1, 1) + "9";
					tek.dodajKod(s);
					tek.dodajKod("00");
					if (operand[0] == '0' && operand[1] == 'x') {
						string novi = operand.substr(2, operand.length());
						char* c = const_cast<char*>(novi.c_str());
						vred_int = (int)strtol(c, nullptr, 16);
					}
					else if (regex_match(operand, o_broj)) {
						char* c = const_cast<char*>(operand.c_str());
						vred_int = (int)strtol(c, nullptr, 8);
					}
					else {
						vred_int = stoi(operand);
					}
					//cout << "ovo je vredn int " + vred_int<<endl;
					string heksa = this->decToHexa(vred_int);
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
					tek.dodajKod(krajnje);
				}
				else {
					if (operand.find('[') != string::npos) {
						operand = operand.substr(1, operand.length());
					}
					string plus, simbol;
					pomoc >> plus;
					pomoc >> simbol;
					simbol = simbol.substr(0, simbol.length() - 1);
					int vred_int;
					if (regex_match(simbol, dec_broj) || regex_match(simbol, o_broj) || regex_match(simbol, heksa_broj)) {
						allocation_counter += 5;
						string s = "";
						if (operand == "pc") {
							s += "6";
						}
						else if (operand == "sp") {
							s += "7";
						}
						else {
							s += operand.substr(1, 1);
						}
						//string r = simbol.substr(1, 1);//mislim da ovo ne treba ovako
						tek.dodajKod("F" + s + "03");//ovo se kod andree razlikuje
						if (operand[0] == '0' && operand[1] == 'x') {
							string novi = operand.substr(2, operand.length());
							char* c = const_cast<char*>(novi.c_str());
							vred_int = (int)strtol(c, nullptr, 16);
						}
						else if (regex_match(operand, o_broj)) {
							char* c = const_cast<char*>(operand.c_str());
							vred_int = (int)strtol(c, nullptr, 8);
						}
						else {
							vred_int = stoi(operand);
						}
						string heksa = this->decToHexa(vred_int);
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
						tek.dodajKod(krajnje);
					}
					else if (regex_match(simbol, neki_tekst)) {
						tek.dodajKod("F903");
						allocation_counter += 5;
						if (!tabela_simb.da_li_postoji_simbol(simbol)) {
							cout << "Ne postoji simbol u tabeli simbola";
							exit(1);
						}
						Simb tren_simb = tabela_simb.dohvatiSimbol(simbol);
						if (tren_simb.dohvatiSekciju() == -1) {
							tek.dodajKod("0000");
							RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(simbol).dohvatiRbr(), 0);
							z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
							z.postaviImeSimbola(simbol);
							tab_rel_zapisa.dodajRelokacioniZapis(z);

						}
						else if (tren_simb.dohvatiSekciju() == -2) {
							int ofs = tabela_simb.dohvatiSimbol(simbol).dohvatiOfset();
							string heksa = decToHexa(ofs);
							string nesto = heksa;
							string krajnje = "";
							if (nesto.length() < 4) {
								int nes = 4 - nesto.length();
								for (int i = 0; i < nes; i++) {
									krajnje += "0";
								}
								krajnje += nesto;
							}
							else {
								krajnje = nesto;
							}
							tek.dodajKod(krajnje);
						}
						else {
							//ovde ima jos nesto ali nije mi jasno sta
							if (tabela_simb.dohvatiSimbol(simbol).dohvatiLocGlob() == "local") {
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(simbol).dohvatiSekciju(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(simbol).dohvatiSekciju()).getNaziv());
								tab_rel_zapisa.dodajRelokacioniZapis(z);

								int ofs = tabela_simb.dohvatiSimbol(simbol).dohvatiOfset();
								string heksa = decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {
								tek.dodajKod("0000");
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(simbol).dohvatiRbr(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(simbol);
								tab_rel_zapisa.dodajRelokacioniZapis(z);
							}
						}
					}					
				}
			}
			else if (koja_instr == "jmp" || koja_instr == "jne" || koja_instr == "jgt" || koja_instr == "jeq" || koja_instr=="call") {
				
				SadrzajSekcije& tek = this->lista_sadrz_sekcije.dohvatiSekciju(imesek);

				if (koja_instr == "call")tek.dodajKod("30");
				if (koja_instr == "jmp") tek.dodajKod("50");
				if (koja_instr == "jeq") tek.dodajKod("51");
				if (koja_instr == "jne") tek.dodajKod("52");
				if (koja_instr == "jgt") tek.dodajKod("53");

				string operand;
				pomoc >> operand;

				if (operand.find('*') != string::npos) {
					/*if (operand.find('[') != string::npos) {
						operand = operand.substr(1, operand.length());
					}*/
					string novi = operand.substr(1, operand.size());//obrisi zvezdicu
					operand = novi;
					int vred_int;
					if (regex_match(operand, neki_registar) || regex_match(operand, indirektno)) {
						allocation_counter += 3;
						if (regex_match(operand, neki_registar)) {
							string s = "";
							if (operand == "pc") {
								s += "6";
							}
							else if (operand == "sp") {
								s += "7";
							}
							else {
								s += operand.substr(1, 1);
							}
							//string r = operand.substr(1, 1);
							tek.dodajKod("F");
							tek.dodajKod(s);
							tek.dodajKod("01");
						}
						else {//proveri ovo
							operand = operand.substr(1, operand.length());
							string s = "";
							if (operand == "pc") {
								s += "6";
							}
							else if (operand == "sp") {
								s += "7";
							}
							else {
								s += operand.substr(1, 1);
							}
							//string r = operand.substr(1, 1);
							tek.dodajKod("F");
							tek.dodajKod(s);
							tek.dodajKod("02");
						}
					}
					else if (regex_match(operand, dec_broj) || regex_match(operand, o_broj) || regex_match(operand, heksa_broj)) {
						allocation_counter += 5;
						tek.dodajKod("F904");
						if (operand[0] == '0' && operand[1] == 'x') {
							string novi = operand.substr(2, operand.length());
							char* c = const_cast<char*>(novi.c_str());
							vred_int = (int)strtol(c, nullptr, 16);
						}
						else if (regex_match(operand, o_broj)) {
							char* c = const_cast<char*>(operand.c_str());
							vred_int = (int)strtol(c, nullptr, 8);
						}
						else {
							vred_int = stoi(operand);
						}
						string heksa = this->decToHexa(vred_int);
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
						tek.dodajKod(krajnje);
					}
					else if (regex_match(operand, neki_tekst)) {
						tek.dodajKod("F904");//proveri
						allocation_counter += 5;
						if (!tabela_simb.da_li_postoji_simbol(operand)){
							cout << "Ne postoji simbol u tabeli simbola";
							exit(1);
						}
						Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
						if (tren_simb.dohvatiSekciju() == -1) {
							tek.dodajKod("0000");
							RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
							z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
							z.postaviImeSimbola(operand);
							tab_rel_zapisa.dodajRelokacioniZapis(z);

						}
						else if (tren_simb.dohvatiSekciju() == -2) {
							//tu mi nije jasno sta treba da radim
							int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
							string heksa = decToHexa(ofs);
							string nesto = heksa;
							string krajnje = "";
							if (nesto.length() < 4) {
								int nes = 4 - nesto.length();
								for (int i = 0; i < nes; i++) {
									krajnje += "0";
								}
								krajnje += nesto;
							}
							else {
								krajnje = nesto;
							}
							tek.dodajKod(krajnje);
						}
						else {
							//ovde ima jos nesto ali nije mi jasno sta
							if (tabela_simb.dohvatiSimbol(operand).dohvatiLocGlob()=="local") {
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(operand).dohvatiSekciju()).getNaziv());
								tab_rel_zapisa.dodajRelokacioniZapis(z);

								int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string heksa = decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {
								tek.dodajKod("0000");
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(operand);
								tab_rel_zapisa.dodajRelokacioniZapis(z);
							}
						}
					}
					else {
						if (operand.find('[') != string::npos) {
							operand = operand.substr(1, operand.length());
						}
						string plus, simbol;
						pomoc >> plus;
						pomoc >> simbol;
						simbol = simbol.substr(0, simbol.length() - 1);
						if (regex_match(simbol, dec_broj) || regex_match(simbol, o_broj) || regex_match(simbol, heksa_broj)) {
							allocation_counter += 5;
							string s = "";
							if (operand == "pc") {
								s += "6";
							}
							else if (operand == "sp") {
								s += "7";
							}
							else {
								s += operand.substr(1, 1);
							}
							//string r = simbol.substr(1, 1);//mislim da ovo ne treba ovako
							tek.dodajKod("F" + s + "03");
							if (operand[0] == '0' && operand[1] == 'x') {
								string novi = operand.substr(2, operand.length());
								char* c = const_cast<char*>(novi.c_str());
								vred_int = (int)strtol(c, nullptr, 16);
							}
							else if (regex_match(operand, o_broj)) {
								char* c = const_cast<char*>(operand.c_str());
								vred_int = (int)strtol(c, nullptr, 8);
							}
							else {
								vred_int = stoi(operand);
							}
							string heksa = this->decToHexa(vred_int);
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
							tek.dodajKod(krajnje);
						}
						else if (regex_match(simbol, neki_tekst)) {
							allocation_counter += 5;
							string s = "";
							if (operand == "pc") {
								s += "6";
							}
							else if (operand == "sp") {
								s += "7";
							}
							else {
								s += operand.substr(1, 1);
							}
							tek.dodajKod("F" + s + "03");
							//tek.dodajKod("F903");
							if (!tabela_simb.da_li_postoji_simbol(simbol)) {
								cout << "Ne postoji simbol u tabeli simbola";
								exit(1);
							}
							Simb tren_simb = tabela_simb.dohvatiSimbol(simbol);
							if (tren_simb.dohvatiSekciju() == -1) {
								tek.dodajKod("0000");
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(simbol).dohvatiRbr(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(simbol);
								tab_rel_zapisa.dodajRelokacioniZapis(z);

							}
							else if (tren_simb.dohvatiSekciju() == -2) {
								int ofs = tabela_simb.dohvatiSimbol(simbol).dohvatiOfset();
								string heksa = decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {
								if (tabela_simb.dohvatiSimbol(simbol).dohvatiLocGlob() == "local") {
									RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(simbol).dohvatiSekciju(), 0);
									z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
									z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(simbol).dohvatiSekciju()).getNaziv());
									tab_rel_zapisa.dodajRelokacioniZapis(z);

									int ofs = tabela_simb.dohvatiSimbol(simbol).dohvatiOfset();
									string heksa = decToHexa(ofs);
									string nesto = heksa;
									string krajnje = "";
									if (nesto.length() < 4) {
										int nes = 4 - nesto.length();
										for (int i = 0; i < nes; i++) {
											krajnje += "0";
										}
										krajnje += nesto;
									}
									else {
										krajnje = nesto;
									}
									tek.dodajKod(krajnje);
								}
								else {
									tek.dodajKod("0000");
									RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(simbol).dohvatiRbr(), 0);
									z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
									z.postaviImeSimbola(simbol);
									tab_rel_zapisa.dodajRelokacioniZapis(z);
								}
							}
						}						
					}
				}
				else {//nema zvezdice
				int vred_int;
					if (regex_match(operand, dec_broj) || regex_match(operand, o_broj) || regex_match(operand, heksa_broj)) {
						allocation_counter += 5;
						tek.dodajKod("F900");
						if (operand[0] == '0' && operand[1] == 'x') {
							string novi = operand.substr(2, operand.length());
							char* c = const_cast<char*>(novi.c_str());
							vred_int = (int)strtol(c, nullptr, 16);
						}
						else if (regex_match(operand, o_broj)) {
							char* c = const_cast<char*>(operand.c_str());
							vred_int = (int)strtol(c, nullptr, 8);
						}
						else {
							vred_int = stoi(operand);
						}
						string heksa = this->decToHexa(vred_int);
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
						tek.dodajKod(krajnje);

					}
					else if (regex_match(operand, neki_tekst)) {
						tek.dodajKod("F900");
						allocation_counter += 5;
						if (!tabela_simb.da_li_postoji_simbol(operand)) {
							cout << "Ne postoji simbol u tabeli simbola";
							exit(1);
						}
						Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
						if (tren_simb.dohvatiSekciju() == -1) {
							tek.dodajKod("0000");
							RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
							z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
							z.postaviImeSimbola(operand);
							tab_rel_zapisa.dodajRelokacioniZapis(z);

						}
						else if (tren_simb.dohvatiSekciju() == -2) {
							int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
							string heksa = decToHexa(ofs);
							string nesto = heksa;
							string krajnje = "";
							if (nesto.length() < 4) {
								int nes = 4 - nesto.length();
								for (int i = 0; i < nes; i++) {
									krajnje += "0";
								}
								krajnje += nesto;
							}
							else {
								krajnje = nesto;
							}
							tek.dodajKod(krajnje);
						}
						else {
							if (tabela_simb.dohvatiSimbol(operand).dohvatiLocGlob() == "local") {
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(operand).dohvatiSekciju()).getNaziv());
								tab_rel_zapisa.dodajRelokacioniZapis(z);

								int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
								string heksa = decToHexa(ofs);
								string nesto = heksa;
								string krajnje = "";
								if (nesto.length() < 4) {
									int nes = 4 - nesto.length();
									for (int i = 0; i < nes; i++) {
										krajnje += "0";
									}
									krajnje += nesto;
								}
								else {
									krajnje = nesto;
								}
								tek.dodajKod(krajnje);
							}
							else {
								tek.dodajKod("0000");
								RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), 0);
								z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
								z.postaviImeSimbola(operand);
								tab_rel_zapisa.dodajRelokacioniZapis(z);
							}
						}
					}
					else if (regex_match(operand, posto_tekst)) {
						tek.dodajKod("F705");
						allocation_counter += 5;
						operand = operand.substr(1, operand.length());
						if (!tabela_simb.da_li_postoji_simbol(operand)) {
							cout << "Ne postoji simbol u tabeli simbola";
							exit(1);
						}
						Simb tren_simb = tabela_simb.dohvatiSimbol(operand);
						if (tren_simb.dohvatiSekciju() == -2) {
							int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
							string heksa = decToHexa(ofs);
							string nesto = heksa;
							string krajnje = "";
							if (nesto.length() < 4) {
								int nes = 4 - nesto.length();
								for (int i = 0; i < nes; i++) {
									krajnje += "0";
								}
								krajnje += nesto;
							}
							else {
								krajnje = nesto;
							}
							tek.dodajKod(krajnje);
						}
						else {
							if (tabela_simb.dohvatiSimbol(operand).dohvatiLocGlob() == "local") {
								if (tren_simb.dohvatiSekciju() == curr_section) {
									int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
									int mesto = pozicija - allocation_counter;
									string heksa = decToHexaZaNegativne(mesto);
									string nesto = heksa;
									string krajnje = "";
									if (nesto.length() < 4) {
										int nes = 4 - nesto.length();
										for (int i = 0; i < nes; i++) {
											krajnje += "0";
										}
										krajnje += nesto;
									}
									else {
										krajnje = nesto;
									}
									tek.dodajKod(krajnje);
								}
								else {
									int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
									int pc = allocation_counter - 2;
									int pomeraj = pozicija - pc;
									RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), -2);
									//RelokacioniZapis z(curr_section, pozicija - allocation_counter , "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiSekciju(), -2);
									z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
									z.postaviImeSimbola(tabela_sekcija.nadjiSekcijuPoRBr(tabela_simb.dohvatiSimbol(operand).dohvatiSekciju()).getNaziv());
									tab_rel_zapisa.dodajRelokacioniZapis(z);

									int ofs = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
									string heksa = decToHexa(ofs);
									string nesto = heksa;
									string krajnje = "";
									if (nesto.length() < 4) {
										int nes = 4 - nesto.length();
										for (int i = 0; i < nes; i++) {
											krajnje += "0";
										}
										krajnje += nesto;
									}
									else {
										krajnje = nesto;
									}
									tek.dodajKod(krajnje);
								}
								
							}
							else {
								if (tren_simb.dohvatiSekciju() == curr_section) {
									int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
									int mesto = pozicija - allocation_counter;
									string heksa = decToHexaZaNegativne(mesto);
									string nesto = heksa;
									string krajnje = "";
									if (nesto.length() < 4) {
										int nes = 4 - nesto.length();
										for (int i = 0; i < nes; i++) {
											krajnje += "0";
										}
										krajnje += nesto;
									}
									else {
										krajnje = nesto;
									}
									tek.dodajKod(krajnje);
								}
								else {
									tek.dodajKod("0000");
									RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), -2);
									z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
									z.postaviImeSimbola(operand);
									tab_rel_zapisa.dodajRelokacioniZapis(z);
								}
								
								/*if (tabela_simb.dohvatiSimbol(operand).dohvatiSekciju() == -1) {
									RelokacioniZapis z(curr_section, allocation_counter - 2, "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), -2);
									z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
									z.postaviImeSimbola(operand);
									tab_rel_zapisa.dodajRelokacioniZapis(z);
								}
								else {
									int pozicija = tabela_simb.dohvatiSimbol(operand).dohvatiOfset();
									RelokacioniZapis z(curr_section, pozicija - allocation_counter, "R_X86_64_PC16", tabela_simb.dohvatiSimbol(operand).dohvatiRbr(), -2);
									z.postaviImeSekcije(tabela_sekcija.nadjiSekcijuPoRBr(curr_section).getNaziv());
									z.postaviImeSimbola(operand);
									tab_rel_zapisa.dodajRelokacioniZapis(z);
								}*/
							}
						}
					}

				}
			}
		}
	}
	//cout << "Tabels simb nakon 2."<<endl;
	ofstream izlazni;
	izlazni.open(ime_izlaznog);
	izlazni << tabela_sekcija << endl;
	izlazni << tabela_simb;
	izlazni << endl;
	izlazni << tab_rel_zapisa << endl << endl;
	//cout << endl << "Ovo je lista sadrzaja sekcije" << endl;
	//cout << this->lista_sadrz_sekcije;
	lista_sadrz_sekcije.ispisiSveSadrzaje(izlazni);
	izlazni.close();
}

string Asembler::decToHexa(int n)
{
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
	string pom="";
	for (int j = i - 1; j >= 0; j--) {
		pom += hexaDeciNum[j];
	}

	return pom;	
}

string Asembler::decToHexaZaNegativne(int br) {
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