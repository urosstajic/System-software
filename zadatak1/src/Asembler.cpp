#include "Asembler.h"
#include <sstream>

//prviprolaz  77-744
//drugi prolaz 744-do kraja


//g++ -o executable *.cpp 
// ./executable asembler -o izlaz.o ulaz.s

//ldr str 1167-1561 za format add r1, r2
// jmp od 2637-2961
string Asembler::worObradaLiterala(string ubacen1) {

	int n = stoi(ubacen1, nullptr, 0); //broj koji zelimo
	string s5;
	char hex_string[20];

	sprintf(hex_string, "%X", n); //convert number to hex
	s5 = hex_string;
	for (int i = s5.size(); i < 4; i++) {
		s5 = "0" + s5;
	}

	return s5;
}

KodSekcije& Asembler::getSekPoImenu(string ime)
{

	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		if ((*iter).getImeSek()==ime) {
			return *iter;
		}
	}
}



void Asembler::dodajuKodove(KodSekcije s)
{
	kodovi.push_back(s);
}


void Asembler::ispisiRelTabelu(ofstream& i)
{
	i << relokacionatabela;
	
}

void Asembler::ispisiKodove(ofstream& i)
{
	i << "Kodovi" << endl;
	list <KodSekcije> ::iterator iter;
	for (iter = kodovi.begin(); iter != kodovi.end(); ++iter) { //bilo je ++iter
		i << (*iter) << endl;
	}
}

Asembler::Asembler()
{
	
}


void Asembler::prviProlaz(ifstream& ulaz, ofstream& izlaz)
{
	string tekucaSekcija=" ";
	string linija;//line.find(".end") != string::npos - znaci da je taj string .end
	string labela;

	bool end = false;
	bool labelaflag=false;
	linijakoda = 0;
	while (getline(ulaz, linija) && end==false) 
	{
		linijakoda++;
		labelaflag = false;

		if (linija.find("#") != string::npos) { //da odsecemo komentar
			int index = -1;
			for (int i = 0; i < linija.length(); i++)
			{
				if (linija.at(i) == '#') {
					index = i;
					break;
				}
			}

			string novalinija = linija.substr(0, index);
			linija = novalinija;
		}

		istringstream ime(linija);
		string deo;
		while (ime >> deo) {
			

			 if (deo == ".end") {
				 string ucitan;
				 ime >> ucitan;
				 if (ucitan != "") { cout << " END Greska linija koda "<<linijakoda; exit(1); }
				end = true;
				break;
			}

			
	   else if (deo.find(":") != string::npos) {
				 if (labelaflag == true) { cout << "Labela " + deo << "nije na pocetku linije u kodu"<<linijakoda << endl; exit(1); }
				 labelaflag = true;
				if (regex_match(deo,reglabela)==false) {
					cout << "Nije odgovarajuci format labele";
					exit(1);
				}
				deo.pop_back(); //da se izvuku :
				if (tekucaSekcija == " ") {
					cout << "Labela"+deo+" nadevena van sekcije";
					exit(1);
				}
				if (!tabelasimbola.proveriLabelu(deo)) {
					Simbol s(deo, tekucaSekcija, locationCounter, true);
					tabelasimbola.dodajSimbol(s);
				}
				else {
					cout << "Visestruka definicija simbola" + deo; exit(1);
				}
				continue;
			}

	   else if(deo==".section"){
				string ucitan,kraj;
				ime >> ucitan; //ime sekcije
				if (ucitan == "") { cout << "Nekorektno uneta direktiva za sekciju u liniji koda:"<<linijakoda; exit(1); }
				if (tekucaSekcija != " ") {
					string s5;
					char hex_string[20];

					sprintf(hex_string, "%X", locationCounter); //convert number to hex
					s5 = hex_string;
					
					string s6;
					char hex_stringg[20];

					sprintf(hex_stringg, "%X", locationCounter-1); //convert number to hex
					s6 = hex_stringg;
					

					Sekcija s(tekucaSekcija, s5, tabelasimbola.dohvatiRB(tekucaSekcija),"0",s6);
					tabelasekcija.dodajSekciju(s);
				}
				locationCounter = 0;
				tekucaSekcija = ucitan;
				if (!tabelasimbola.proveriLabelu(tekucaSekcija)) {
					Simbol s(tekucaSekcija, tekucaSekcija, locationCounter, true);
					tabelasimbola.dodajSimbol(s);
				}
				else {
					cout << "Visestruka definicija " + tekucaSekcija; exit(1);
				}
				

				ime >> kraj; //ime sekcije
				if (kraj != "") { cout << "Nekorektno uneta direktiva za sekciju//previse parametara  " << linijakoda; exit(1); }
				else {
					break;// jer je jedna linija obradjena
				}
			}

	   else if (deo==".extern") {   
				string ucitan,ucitan1;
				ime >> ucitan;

				while (ucitan.find(",") != string::npos) {
					int index = -1;
					for (int i = 0; i < ucitan.length(); i++)
					{
						if (ucitan.at(i) == ',') {
							index = i;
							break;
						}

					}
					if (index == ucitan.size() - 1) {    //fora kad je extern a, b
						ucitan.pop_back(); //skini zarez

						ime >> ucitan1;
						if (ucitan1 == "") { cout << "Ne sme zarez biti poslednji"; exit(1); } //zarez pa se nista ne ucitava
						if (tabelasimbola.proveriLabelu(ucitan) == true) { cout << "Visestruka definicija simbola linija koda" << linijakoda; exit(1); }
						Simbol s(ucitan, "UND", -1, false);
						tabelasimbola.dodajSimbol(s);
						ucitan = ucitan1;
						continue;
					}
					else {
					string simbol = ucitan.substr(0, index);
					int velicina = ucitan.size() - index - 1;
					string novalinija = ucitan.substr(index + 1, velicina);
					ucitan = novalinija;
					if (tabelasimbola.proveriLabelu(simbol) == true) { cout << "Visestruka definicija simbola" << linijakoda; exit(1); }
					Simbol s(simbol, "UND", -1, false);
					tabelasimbola.dodajSimbol(s);
				 }
				}

				if (tabelasimbola.proveriLabelu(ucitan) == true) { cout << "Visestruka definicija simbola" << linijakoda; exit(1); }
				Simbol s(ucitan, "UND", -1, false);
				tabelasimbola.dodajSimbol(s);
			}

	   else if (deo==".global") {
				 break;
			 }

	   else if (deo==".equ") { 
				 if (first == false) {
					 KodSekcije equ("equ");
					 this->dodajuKodove(equ);
					 first = true;
				 }
				 postojiEqu = true;
				string ucitan,ubacen1,ucitan3,kraj;
	           ime >> ucitan;
			   KodSekcije& k = this->getSekPoImenu("equ");

			   if (ucitan.find(",") != string::npos) {
				   int index = -1;
				   for (int i = 0; i < ucitan.length(); i++)
				   {
					   if (ucitan.at(i) == ',') {
						   index = i;
						   break;
					   }
				   }
				   if (index == ucitan.size() - 1) { //format add r1, r2
					   ucitan.pop_back();
					   ime >> ubacen1;
					   if (regex_match(ubacen1, regLiteral) == false && regex_match(ubacen1, regLiteral1) == false && regex_match(ubacen1, regLiteral2) == false ) {  // && regex_match(ucitan, regLiteral1) == false
						   cout << "Nije uneta odgovarajuc literal za  " + deo<<" Linija koda :"<<linijakoda; exit(1);
					   }
					   int n = stoi(ubacen1, nullptr, 0);
					 //broj koji zelimo
					   string s5;
					   char hex_string[20];

					   sprintf(hex_string, "%X", n); //convert number to hex
					   s5 = hex_string;
					   for (int i = s5.size(); i < 4; i++) {
						   s5 = "0" + s5;
					   }
					   if (tabelasimbola.proveriLabelu(ucitan) == true) { cout << "Visestruka definicija simbola" << " Linija koda :" << linijakoda; exit(1); }
					   Simbol s(ucitan, "equ", OffsetEqu, true);
					   tabelasimbola.dodajSimbol(s);
					   OffsetEqu += 4;
					   k.dodajuKod(s5);
					   

				   }
				   else { //add r1,r2
					   string prvi, drugi;
					   prvi = ucitan.substr(0, index);
					   int velicina = ucitan.size() - index - 1;
					   drugi = ucitan.substr(index + 1, velicina);
					   if (regex_match(drugi, regLiteral) == false && regex_match(drugi, regLiteral1) == false && regex_match(drugi, regLiteral2) == false ) {  // && regex_match(ucitan, regLiteral1) == false
						   cout << "Nije uneta odgovarajuc literal za  " + deo << " Linija koda :" << linijakoda; exit(1);
					   }

					   int n = stoi(drugi, nullptr, 0);
					   //broj koji zelimo
					   string s5;
					   char hex_string[20];

					   sprintf(hex_string, "%X", n); //convert number to hex
					   s5 = hex_string;
					   for (int i = s5.size(); i < 4; i++) {
						   s5 = "0" + s5;
					   }
					   if (tabelasimbola.proveriLabelu(prvi) == true) { cout << "Visestruka definicija simbola" << " Linija koda :" << linijakoda; exit(1); }
					   Simbol s(prvi, "equ", OffsetEqu, true);
					   tabelasimbola.dodajSimbol(s);
					   OffsetEqu += 4;
					   k.dodajuKod(s5);
				   }
			   }

			   else { //ucitan je samo npr r1 
				   string saZarezom, ucitan2;
				   ime >> saZarezom;

				   if (saZarezom.find(",") == string::npos) { cout << "POGRESNO UNETA " + deo + " nema zareza " << " Linija koda :" << linijakoda; exit(1); }
				   if (saZarezom == ",") { //add r1 , r2
					   ime >> ucitan2;
					   if (regex_match(ucitan2, regLiteral) == false && regex_match(ucitan2, regLiteral1) == false && regex_match(ucitan2, regLiteral2) == false ) {  // && regex_match(ucitan, regLiteral1) == false
						   cout << "Nije uneta odgovarajuc literal za  " + deo << " Linija koda :" << linijakoda; exit(1);
					   }
					   int n = stoi(ucitan2, nullptr, 0);
					   //broj koji zelimo
					   string s5;
					   char hex_string[20];

					   sprintf(hex_string, "%X", n); //convert number to hex
					   s5 = hex_string;
					   for (int i = s5.size(); i < 4; i++) {
						   s5 = "0" + s5;
					   }
					   if (tabelasimbola.proveriLabelu(ucitan) == true) { cout << "Visestruka definicija simbola" << " Linija koda :" << linijakoda; exit(1); }
					   Simbol s(ucitan, "equ", OffsetEqu, true);
					   tabelasimbola.dodajSimbol(s);
					   OffsetEqu += 4;
					   k.dodajuKod(s5);
				   }
				   else { //add r1 ,r2
					   string bezZareza;
					   int velicina = saZarezom.size() - 1;
					   bezZareza = saZarezom.substr(1, velicina);
					   if (regex_match(bezZareza, regLiteral) == false && regex_match(bezZareza, regLiteral1) == false && regex_match(bezZareza, regLiteral2) == false ) {  // && regex_match(ucitan, regLiteral1) == false
						   cout << "Nije uneta odgovarajuc literal za  " + deo << " Linija koda :" << linijakoda; exit(1);
					   }
					   int n = stoi(bezZareza, nullptr, 0);
					   //broj koji zelimo
					   string s5;
					   char hex_string[20];

					   sprintf(hex_string, "%X", n); //convert number to hex
					   s5 = hex_string;
					   for (int i = s5.size(); i < 4; i++) {
						   s5 = "0" + s5;
					   }
					   if (tabelasimbola.proveriLabelu(ucitan) == true) { cout << "Visestruka definicija simbola" << " Linija koda :" << linijakoda; exit(1); }
					   Simbol s(ucitan, "equ", OffsetEqu, true);
					   tabelasimbola.dodajSimbol(s);
					   OffsetEqu += 4;
					   k.dodajuKod(s5);
				   }

			   }
			   ime >> kraj;
			   if (kraj != "") { cout << "Pogresan format equ instr- previse parametara" << " Linija koda :" << linijakoda << endl; exit(1); }
			   break;

			}

	   else if (deo==".skip") { 
	         string ucitan;
			 int broj;
	         ime>> ucitan; 
			
		      if (regex_match(ucitan, regLiteral) == false && regex_match(ucitan, regLiteral1) == false && regex_match(ucitan, regLiteral2) == false ) {  // && regex_match(ucitan, regLiteral1) == false
				 cout << "Nije uneta odgovarajuc literal za  " + deo << " Linija koda :" << linijakoda; exit(1);
			 }
			 broj = stoi(ucitan, nullptr, 0);
			 locationCounter+=broj;
            }

	   else if (deo==".word") {
	   string  ucitan,ucitan1;
	   ime >> ucitan;
	   int brojac=1;



	   while (ucitan.find(",") != string::npos) {
		   int index = -1;
		   for (int i = 0; i < ucitan.length(); i++)
		   {
			   if (ucitan.at(i) == ',') {
				   index = i;
				   break;
			   }

		   }
		   if (index == ucitan.size() - 1) {    //fora kad je extern a, b
			   ucitan.pop_back(); //skini zarez

			   ime >> ucitan1;
			   if (ucitan1 == "") { cout << "Ne sme zarez biti poslednji" << " Linija koda :" << linijakoda; exit(1); } //zarez pa se nista ne ucitava
			   brojac++;
			   ucitan = ucitan1;
			   continue;
		   }
		   else {
			   string simbol = ucitan.substr(0, index);
			   int velicina = ucitan.size() - index - 1;
			   string novalinija = ucitan.substr(index + 1, velicina);
			   ucitan = novalinija;
			   brojac++;
		   }
	   }
	 
	   locationCounter += 2 * brojac;
          }
			//------------------------------------------INSTRUKCIJE-------------------------------------------------------------------//
	   else	if (regex_match(deo, reginstrukcija)) {
				if (tekucaSekcija == " ") {
					cout << "Instrukcija se ne nazali u okviru sekcije" << " Linija koda :" << linijakoda; exit(1);
				}
				if (deo == "halt" || deo=="iret" || deo=="ret") {
					locationCounter++;
					string ubacen;
					ime >> ubacen;
					if (ubacen == "") { break; } //stavljeno da ne ide kroz donje if-ove
					else { cout << "Nekorektno navodjenje jednobajtne instrukcije" << " Linija koda :" << linijakoda;  exit(1); }
				}
				else	if (deo == "int") {
					string ubacen;
					ime >> ubacen;
					if (regex_match(ubacen, regREG)==false) {
						cout << "Nekorektan poziv int instrukcije" << " Linija koda :" << linijakoda;  exit(1);
					}
					locationCounter+=2;
					string novi;
					ime >> novi;
					if (novi == "") { break; } //stavljeno da ne ide kroz donje if-ove
					else { cout << "Nekorektno navodjenje OVE INS " + deo + " instrukcije jer ucita " + novi << " Linija koda :" << linijakoda;  exit(1); }
	             }
				else	if (deo == "xchg" || deo == "add" || deo == "sub" || deo == "mul" || deo == "div"
					|| deo == "cmp" || deo == "not" || deo == "and" || deo == "or" || deo == "xor" || deo == "test" || deo == "shl" || deo == "shr") {
					string ucitan,zarez,ubacen1;
					ime >> ucitan;

					if (ucitan.find(",") != string::npos) {
						int index = -1;
						for (int i = 0; i < ucitan.length(); i++)
						{
							if (ucitan.at(i) == ',') {
								index = i;
								break;
							}
	                    }
						if (index == ucitan.size()-1) { //format add r1, r2
							ucitan.pop_back(); 
							ime >> ubacen1;
							if (regex_match(ucitan, regREG) == false && regex_match(ubacen1, regREG) == false) {
								cout << "Nekorektan poziv -pogresni registri  " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
							}

						}
						else { //add r1,r2
							string prvi, drugi;
							prvi = ucitan.substr(0, index);
							int velicina = ucitan.size() - index - 1;
							drugi = ucitan.substr(index + 1, velicina); 
									if (regex_match(prvi, regREG) == false || regex_match(drugi, regREG)==false) {
										cout << "Nekorektan poziv "+deo+" instrukcije" << " Linija koda :" << linijakoda;  exit(1);
									}
						      }
					}
						
					else { //ucitan je samo npr r1 
						string saZarezom,ucitan2;
						ime >> saZarezom;
						if (saZarezom.find(",") == string::npos) { cout << "POGRESNO UNETA " + deo+" nema zareza " << " Linija koda :" << linijakoda; exit(1); }
						if (saZarezom == ",") { //add r1 , r2
							ime >> ucitan2;
							if (regex_match(ucitan, regREG) == false || regex_match(ucitan2, regREG) == false) {
								cout << "Nekorektan poziv " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
							}
						}
						else { //add r1 ,r2
							string bezZareza;
							int velicina = saZarezom.size()-1;
							bezZareza = saZarezom.substr(1, velicina);
							if (regex_match(ucitan, regREG) == false || regex_match(bezZareza, regREG) == false) {
								cout << "Nekorektan poziv " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
							}
						}

					}
					
					locationCounter += 2;
					string novi;
					ime >> novi;
					if (novi == "") { break; } //stavljeno da ne ide kroz donje if-ove
					else { cout << "Nekorektno navodjenje OVE INS "+deo+" instrukcije jer ucita "+novi << " Linija koda :" << linijakoda;  exit(1); }
				}

				else if (deo == "push" || deo == "pop") { 

					string ubacen;
					ime >> ubacen;
					if (regex_match(ubacen, regREG) == false) {
						cout << "Nekorektan poziv " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
					}
					locationCounter += 3;
				}
				///////////////////////////////////////////////////////////////////////////////
				else if (deo == "ldr" || deo == "str") { 
					string ucitan, ubacen1, kraj,zarez;
					ime >> ucitan;


					if (ucitan.find(",") != string::npos) {
						int index = -1;
						for (int i = 0; i < ucitan.length(); i++)
						{
							if (ucitan.at(i) == ',') {
								index = i;
								break;
							}
						}
						if (index == ucitan.size() - 1) { //format add r1, r2
							ucitan.pop_back();
							ime >> ubacen1;
								if (regex_match(ucitan, regREG) == false) {
						cout << "Nekorektan poziv " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
					}
					
					if (regex_match(ubacen1, regREG) || regex_match(ubacen1, regREGIND1)) {
						locationCounter += 3;
					}
					else  if (regex_match(ubacen1, operandPodaci51) || regex_match(ubacen1, operandPodaci511) || regex_match(ubacen1, operandPodaci512)
	                              || regex_match(ubacen1, operandPodaci52) || regex_match(ubacen1, operandPodaci53)
	                            ||  regex_match(ubacen1, regLiteral) ||  regex_match(ubacen1, regLiteral1)
	                            ||   regex_match(ubacen1, regLiteral2) 
	                             || regex_match(ubacen1, operandPodaci54)) {
						locationCounter += 5;
					}
					else {
						string plus,jos;
						ime >> plus;
						if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
						ime >> jos;
						jos.pop_back();
						if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
							|| regex_match(jos, regLiteral2)  || regex_match(jos, operandPodaci53)) {
							locationCounter += 5;
						}
						else { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
					}
					

						}
						else { //add r1,r2
							string prvi, drugi;
							prvi = ucitan.substr(0, index);
							int velicina = ucitan.size() - index - 1;
							drugi = ucitan.substr(index + 1, velicina);
							if (regex_match(prvi, regREG) == false) {
								cout << "Nekorektan poziv " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
							}
							if (regex_match(drugi, regREG) || regex_match(drugi, regREGIND1)) {
								locationCounter += 3;
							}
							else  if (regex_match(drugi, operandPodaci51) || regex_match(drugi, operandPodaci511) || regex_match(drugi, operandPodaci512)
								|| regex_match(drugi, operandPodaci52) || regex_match(drugi, operandPodaci53)
								|| regex_match(drugi, regLiteral) || regex_match(drugi, regLiteral1)
								|| regex_match(drugi, regLiteral2)
								|| regex_match(drugi, operandPodaci54)) {
								locationCounter += 5;
							}
							else {
								string plus, jos;
								ime >> plus;
								if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
								ime >> jos;
								jos.pop_back();
								if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
									|| regex_match(jos, regLiteral2) || regex_match(jos, operandPodaci53)) {
									locationCounter += 5;
								}
								else { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
							}

						}
					}

					else { //ucitan je samo npr r1 
						string saZarezom, ucitan2;
						ime >> saZarezom;
						if (saZarezom.find(",") == string::npos) { cout << "POGRESNO UNETA " + deo + " nema zareza " << " Linija koda :" << linijakoda; exit(1); }
						if (saZarezom == ",") { //add r1 , r2
							ime >> ucitan2;
							if (regex_match(ucitan, regREG) == false) {
								cout << "Nekorektan poziv " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
							}
							
							if (regex_match(ucitan2, regREG) || regex_match(ucitan2, regREGIND1)) {
								locationCounter += 3;
							}
							else  if (regex_match(ucitan2, operandPodaci51) || regex_match(ucitan2, operandPodaci511) || regex_match(ucitan2, operandPodaci512)
								|| regex_match(ucitan2, operandPodaci52) || regex_match(ucitan2, operandPodaci53)
								|| regex_match(ucitan2, regLiteral) || regex_match(ucitan2, regLiteral1)
								|| regex_match(ucitan2, regLiteral2)
								|| regex_match(ucitan2, operandPodaci54)) {
								locationCounter += 5;
							}
							else {
								string plus, jos;
								ime >> plus;
								if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
								ime >> jos;
								jos.pop_back();
								if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
									|| regex_match(jos, regLiteral2) || regex_match(jos, operandPodaci53)) {
									locationCounter += 5;
								}
								else { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
							}

						}
						else { //add r1 ,r2
							string bezZareza;
							int velicina = saZarezom.size() - 1;
							bezZareza = saZarezom.substr(1, velicina);
							if (regex_match(ucitan, regREG) == false) {
								cout << "Nekorektan poziv " + deo + " instrukcije" << " Linija koda :" << linijakoda;  exit(1);
							}
							
							if (regex_match(bezZareza, regREG) || regex_match(bezZareza, regREGIND1)) {
								locationCounter += 3;
							}
							else  if (regex_match(bezZareza, operandPodaci51) || regex_match(bezZareza, operandPodaci511) || regex_match(bezZareza, operandPodaci512)
								|| regex_match(bezZareza, operandPodaci52) || regex_match(bezZareza, operandPodaci53)
								|| regex_match(bezZareza, regLiteral) || regex_match(bezZareza, regLiteral1)
								|| regex_match(bezZareza, regLiteral2)
								|| regex_match(bezZareza, operandPodaci54)) {
								locationCounter += 5;
							}
							else {
								string plus, jos;
								ime >> plus;
								if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
								ime >> jos;
								jos.pop_back();
								if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
									|| regex_match(jos, regLiteral2) || regex_match(jos, operandPodaci53)) {
									locationCounter += 5;
								}
								else { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
							}

						}

					}
				
					ime >> kraj; //smemo ovako posto ce kom biti vec odsecen
					if (kraj != "") { cout << "Nepravilan format " + deo + " instrukcije" << " Linija koda :" << linijakoda; exit(1); }

				}


				else if (deo == "call" || deo == "jmp" || deo == "jeq" || deo == "jne" || deo == "jgt") {
					string ucitan,ubacen1, kraj;
					ime >> ubacen1;
					if (ubacen1.find("*") != string::npos) { //ako ima zvezdicu
						
						int vel = ubacen1.size() - 1;
						ucitan = ubacen1.substr(1, vel);
						ubacen1= ucitan;
						if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1) || regex_match(ubacen1, regLiteral2)
							|| regex_match(ubacen1, operandPodaci53)) {
							locationCounter += 5;
						}
						else if (regex_match(ubacen1, regREG) || regex_match(ubacen1, regREGIND1)) {
							locationCounter += 3;
						}
						else {
							string plus, jos;
							ime >> plus;
							if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
							ime >> jos;
							jos.pop_back();
							if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
								|| regex_match(jos, regLiteral2) || regex_match(jos, operandPodaci53)) {
								locationCounter += 5;
							}
							else { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand" << " Linija koda :" << linijakoda; exit(1); }
						}//plus,jos
					}

					else {
						
						if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1) || regex_match(ubacen1, regLiteral2)
							|| regex_match(ubacen1, operandPodaci53) || regex_match(ubacen1, operandPodaci54)) {
							locationCounter += 5;
						}
					}
					

					ime >> kraj; //smemo ovako posto ce kom biti vec odsecen
					if (kraj != "") { cout << "Nepravilan format " + deo + " instrukcije" << " Linija koda :" << linijakoda; exit(1); }

				}



			}//kraj za instrukcije

	  




	   else { //nista sto ponudjeni asembler moze da obradi
	   cout << "nista sto ponudjeni asembler moze da obradi" << " Linija koda :" << linijakoda;  exit(1);

         }


		}//kraj unutrasnjeg while-a
	//	linijakoda++;
	}//kraj while-a
	string s5;
	char hex_string[20];

	sprintf(hex_string, "%X", locationCounter); //convert number to hex
	s5 = hex_string;
	
	string s6;
	char hex_stringg[20];

	sprintf(hex_stringg, "%X", locationCounter - 1); //convert number to hex
	s6 = hex_stringg;
	


	Sekcija s(tekucaSekcija, s5, tabelasimbola.dohvatiRB(tekucaSekcija), "0", s6);
	
	tabelasekcija.dodajSekciju(s);

	if (postojiEqu==true) {

		string s5;
		char hex_string[20];

		sprintf(hex_string, "%X", OffsetEqu); //convert number to hex
		s5 = hex_string;
		
		string s6;
		char hex_stringg[20];

		sprintf(hex_stringg, "%X", OffsetEqu - 1); //convert number to hex
		s6 = hex_stringg;
		

		Sekcija sek("equ", s5, -1, "0", s6);
		tabelasekcija.dodajSekciju(sek);
	}

	
	izlaz<< "TabelaSekcija" << endl << tabelasekcija;
	
	    
}

void Asembler::drugiProlaz(ifstream& ulaz, ofstream& izlaz)
{
	ulaz.clear();
	ulaz.seekg(0); //pocni da citas iz pocetka
	locationCounter = 0;
	string tekucaSekcija = " ";
	string linija;  //line.find(".end") != string::npos - znaci da je taj string .end
	string labela;

	bool end = false;
	while (getline(ulaz, linija) && end == false)
	{

		if (linija.find("#") != string::npos) { //da odsecemo komentar
			int index = -1;
			for (int i = 0; i < linija.length(); i++)
			{
				if (linija.at(i) == '#') {
					index = i;
					break;
				}
			}

			string novalinija = linija.substr(0, index);
		
			linija = novalinija;
		}

		istringstream ime(linija);
		string deo;
		while (ime >> deo) {

			if (deo == ".end") {
				end = true;
				break;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//ispraviti za global a , b , c -sa tim razmacima 
			if (deo == ".global") { //ne radi
			   
				string ucitan,ucitan1;
				ime >> ucitan;



				while (ucitan.find(",") != string::npos) {
					int index = -1;
					for (int i = 0; i < ucitan.length(); i++)
					{
						if (ucitan.at(i) == ',') {
							index = i;
							break;
						}

					}
					if (index == ucitan.size() - 1) {    //fora kad je extern a, b
						ucitan.pop_back(); //skini zarez

						ime >> ucitan1;
						//if (ucitan1 != "") { cout << "Ne sme zarez biti poslednji"; exit(1); } //zarez pa se nista ne ucitava
						if (tabelasimbola.proveriLabelu(ucitan) == false)
						{ cout << "Ne postoji u tabeli simbola-nije nigde definisana u fajlu "+ucitan; exit(1); }
						tabelasimbola.postaviGlobal(ucitan);
						ucitan = ucitan1;
						continue;
					}
					else {
						string simbol = ucitan.substr(0, index);
						int velicina = ucitan.size() - index - 1;
						string novalinija = ucitan.substr(index + 1, velicina);
						ucitan = novalinija;
						if (tabelasimbola.proveriLabelu(simbol) == false) { cout << "Ne postoji u tabeli simbola-nije nigde definisana u fajlu "+simbol+simbol
							; exit(1); }
						tabelasimbola.postaviGlobal(simbol);
	                   }
				}
				if (tabelasimbola.proveriLabelu(ucitan) == false) { cout << "Ne postoji u tabeli simbola-nije nigde definisana u fajlu "+ucitan; exit(1); }
				tabelasimbola.postaviGlobal(ucitan);

			}


			else if (deo == ".extern" ) {
				break;
			}

			else if (deo.find(":") != string::npos) {
				
				continue;
			}
			else if (deo == ".equ") {
				break;

			}

			else if (deo==".section") {
				string ucitan;
				ime >> ucitan; //ime sekcije
			    locationCounter = 0;
				tekucaSekcija = ucitan;
			  //pravi odmah svoju tabelu rel i kodsekcije
				
				KodSekcije k(tekucaSekcija);
				dodajuKodove(k);

			}
			else if (deo.find(".skip") != string::npos) {  
				string ucitan;
				int broj;
				ime >> ucitan; 
			    broj = stoi(ucitan, nullptr, 0);
				KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
				for (int i = 0; i < broj; i++)
				{
					k.dodajuKod("00");
				}

				locationCounter += broj;
			}

			else if (deo.find(".word") != string::npos) {
				string  ucitan, ucitan1;
				ime >> ucitan;
				//int brojac = 1;
				bool sam = false; //ako ni ne ulazi u while
				KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
				RelTabela& t =relokacionatabela;

				while (ucitan.find(",") != string::npos) {
					int index = -1;
				

					for (int i = 0; i < ucitan.length(); i++)
					{
						if (ucitan.at(i) == ',') {
							index = i;
							break;
						}

					}
					if (index == ucitan.size() - 1) {    //fora kad je extern a, b
						ucitan.pop_back(); //skini zarez

						ime >> ucitan1;
						if (ucitan1 == "") { cout << "Ne sme zarez biti poslednji"; exit(1); } //zarez pa se nista ne ucitava
						//////////////////////////
						if (regex_match(ucitan, regLiteral) || regex_match(ucitan, regLiteral1) || regex_match(ucitan, regLiteral2)) {
							string s = worObradaLiterala(ucitan);
							string p = s.substr(0, 2);
							string d = s.substr(2, 2);
							k.dodajuKod(d + p);

							locationCounter += 2;
						}

						if (regex_match(ucitan, operandPodaci53)) { //ako je simbol
							if (!tabelasimbola.proveriLabelu(ucitan)) {
								Simbol s(ucitan, "UND", -1, false);
								tabelasimbola.dodajSimbol(s);
							}
							int i = tabelasimbola.proveriGlobalnost(ucitan); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter, "R_X86_64noninst_16", tabelasimbola.dohvatiRBSekcije(ucitan), 0, false);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(ucitan);

								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}
								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(d + p);

								locationCounter += 2;
							}
							else {
								RelZapis z(tekucaSekcija, locationCounter, "R_X86_64noninst_16", tabelasimbola.dohvatiRB(ucitan), 0, false);
								t.dodajZapis(z);
								k.dodajuKod("0000");
								locationCounter += 2;
							}
						}



						/////////////////////////////
					
						ucitan = ucitan1;
						continue;
					}
					else {
						string simbol = ucitan.substr(0, index);
						
						if (regex_match(simbol, regLiteral) || regex_match(simbol, regLiteral1) || regex_match(simbol, regLiteral2) ) {
							string s =worObradaLiterala(ucitan);
							string p = s.substr(0, 2);
							string d = s.substr(2,2);
							k.dodajuKod(d+p);
						}


						if (regex_match(simbol, operandPodaci53)) { //ako je simbol
							if (tabelasimbola.proveriLabelu(simbol)==false) {
							//dodaje ga u tabelu simbola
								Simbol s(simbol,"UND",-1,false);
								tabelasimbola.dodajSimbol(s);
							}
							int i = tabelasimbola.proveriGlobalnost(simbol); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter, "R_X86_64noninst_16", tabelasimbola.dohvatiRBSekcije(simbol), 0, false);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(simbol);

								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(d + p);
							}
							else {
								RelZapis z(tekucaSekcija, locationCounter, "R_X86_64noninst_16", tabelasimbola.dohvatiRB(ucitan), 0,false);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}
						}
						
						
						int velicina = ucitan.size() - index - 1;
						string novalinija = ucitan.substr(index + 1, velicina);
						ucitan = novalinija;
						locationCounter+=2;
					}
				}

				if (regex_match(ucitan, regLiteral) || regex_match(ucitan, regLiteral1) || regex_match(ucitan, regLiteral2)) {
					string s = worObradaLiterala(ucitan);
					string p = s.substr(0, 2);
					string d = s.substr(2, 2);
					k.dodajuKod(d + p);
					
					locationCounter += 2;
				}

				if (regex_match(ucitan, operandPodaci53)) { //ako je simbol
					if (!tabelasimbola.proveriLabelu(ucitan)) { 
						Simbol s(ucitan, "UND", -1, false);
						tabelasimbola.dodajSimbol(s);
					}
					int i = tabelasimbola.proveriGlobalnost(ucitan); //0-lok, 1-glob
					if (i == 0) {
						RelZapis z(tekucaSekcija, locationCounter, "R_X86_64noninst_16", tabelasimbola.dohvatiRBSekcije(ucitan), 0, false);
						t.dodajZapis(z); //lokalno
						int o = tabelasimbola.dohvatiOffset(ucitan);

						string s5;
						char hex_string[20];

						sprintf(hex_string, "%X", o); //convert number to hex
						s5 = hex_string;
						for (int i = s5.size(); i < 4; i++) {
							s5 = "0" + s5;
						}
						string p = s5.substr(0, 2);
						string d = s5.substr(2, 2);
						k.dodajuKod(d + p);
						
						locationCounter += 2;
					}
					else {
						RelZapis z(tekucaSekcija, locationCounter, "R_X86_64noninst_16", tabelasimbola.dohvatiRB(ucitan), 0, false);
						t.dodajZapis(z);
						k.dodajuKod("0000");
						locationCounter += 2;
					}
				}

				//locationCounter += 2 * brojac;
			}

			//--------------------------------------------------INSTRUKCIJE-II prolaz----------------------------------//


		
			else	if (regex_match(deo, reginstrukcija)) {
			
			if (deo == "halt" || deo == "iret" || deo == "ret") {
				locationCounter++;
				KodSekcije& k=this->getSekPoImenu(tekucaSekcija);
				if (deo == "halt")k.dodajuKod("00");
				else if (deo == "iret") k.dodajuKod("20");
				else if (deo == "ret") k.dodajuKod("40");
			}


			else	if (deo == "int") {
				string ubacen;
				ime >> ubacen;
				KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
				k.dodajuKod("10");
				
				string s = ubacen.substr(1,1)+"F" ;
				k.dodajuKod(s);
				locationCounter += 2;
				
			}

			else if (deo == "add" || deo == "sub" || deo == "mul" || deo == "div" || deo == "cmp" ||
				deo == "xchg" || deo == "not" || deo == "and" || deo == "or" || deo == "xor"
				|| deo == "test" || deo == "shl" || deo == "shr") {
				KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
				if (deo == "add")k.dodajuKod("70");
				else if (deo == "sub") k.dodajuKod("71");
				else if (deo == "mul") k.dodajuKod("72");
				else if (deo == "div") k.dodajuKod("73");
				else if (deo == "cmp") k.dodajuKod("74");
				else if (deo == "not") k.dodajuKod("80");
				else if (deo == "and") k.dodajuKod("81");
				else if (deo == "or") k.dodajuKod("82");
				else if (deo == "xor") k.dodajuKod("83");
				else if (deo == "test") k.dodajuKod("84");
				else if (deo == "shl") k.dodajuKod("90");
				else if (deo == "shr") k.dodajuKod("91");
				else if (deo == "xchg") k.dodajuKod("60");
				


				string ucitan, zarez, ubacen1;
				ime >> ucitan;

				if (ucitan.find(",") != string::npos) {
					int index = -1;
					for (int i = 0; i < ucitan.length(); i++)
					{
						if (ucitan.at(i) == ',') {
							index = i;
							break;
						}
					}
					if (index == ucitan.size() - 1) { //format add r1, r2 //ucitan-r1
						ucitan.pop_back();
						ime >> ubacen1; //r2
						string s = ucitan.substr(1, 1) + ubacen1.substr(1,1);
						k.dodajuKod(s);


					}
					else { //add r1,r2
						string prvi, drugi;
						prvi = ucitan.substr(0, index);
						int velicina = ucitan.size() - index - 1;
						drugi = ucitan.substr(index + 1, velicina);
						string s = prvi.substr(1, 1) + drugi.substr(1, 1);
						k.dodajuKod(s);
					}
				}

				else { //ucitan je samo npr r1 
					string saZarezom, ucitan2;
					ime >> saZarezom;
					if (saZarezom.find(",") == string::npos) { cout << "POGRESNO UNETA " + deo + " nema zareza "; exit(1); }
					if (saZarezom == ",") { //add r1 , r2  ucitan-r1
						ime >> ucitan2; //r2
						string s = ucitan.substr(1, 1) + ucitan2.substr(1, 1);
						k.dodajuKod(s);
					}
					else { //add r1 ,r2
						string bezZareza;
						int velicina = saZarezom.size() - 1;
						bezZareza = saZarezom.substr(1, velicina);
						string s = ucitan.substr(1, 1) + bezZareza.substr(1, 1);
						k.dodajuKod(s);
					}

				}

				locationCounter += 2;


			}

			

			else if (deo == "push" || deo == "pop") {

				string ubacen;
				ime >> ubacen;
				KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
				if (deo == "push") { k.dodajuKod("B0"); k.dodajuKod("6" + ubacen.substr(1, 1)); k.dodajuKod("12"); } //reg ind
				else if (deo == "pop") { k.dodajuKod("A0"); k.dodajuKod( ubacen.substr(1, 1) + "6" );  k.dodajuKod("42");
				}

				locationCounter += 3;
			}
			///////////////////////////////////////////////////////////////////////////////
			else if (deo == "ldr" || deo == "str") {
				string ucitan, ubacen1, kraj, zarez;
				ime >> ucitan;

				KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
				if (deo == "ldr")k.dodajuKod("A0");
				else if (deo == "str") k.dodajuKod("B0");

				if (ucitan.find(",") != string::npos) {
					int index = -1;
					for (int i = 0; i < ucitan.length(); i++)
					{
						if (ucitan.at(i) == ',') {
							index = i;
							break;
						}
					}
					if (index == ucitan.size() - 1) { //format add r1, r2
						ucitan.pop_back();
						ime >> ubacen1;

						if (regex_match(ubacen1, regREG)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(1, 1);
							k.dodajuKod(s);
							string s1 = "01"; //reg dir
							k.dodajuKod(s1);           

						}
						else if (regex_match(ubacen1, regREGIND1)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(2, 1);
							k.dodajuKod(s);
							string s1 = "02"; //red ind bez pomeraja
							k.dodajuKod(s1);
						}
						else if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1)
							|| regex_match(ubacen1, regLiteral2)) {
							locationCounter += 5;
							string s;

							s = ucitan.substr(1, 1) + "9"; //sta je source- 9 kao registar koji ne postoji


							k.dodajuKod(s);
							string s1 = "04";
							k.dodajuKod(s1);


							int n = stoi(ubacen1, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p+d);
						}
						else if (regex_match(ubacen1, operandPodaci53)) {

							locationCounter += 5;
							string s = ucitan.substr(1, 1) + "9"; //reg
							k.dodajuKod(s);
							k.dodajuKod("04");
							string kodic;

							Simbol simb = tabelasimbola.dohvSimbol(ubacen1);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p+d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(ubacen1)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen1 << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(ubacen1); //0-lok, 1-glob
								if (i == 0) {
									int o;
									if (tabelasimbola.dohvatiSekciju(ubacen1) != "equ") {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubacen1), 0);
										t.dodajZapis(z); //lokalno
										o = tabelasimbola.dohvatiOffset(ubacen1);
										string s5;
										char hex_string[20];

										sprintf(hex_string, "%X", o); //convert number to hex
										s5 = hex_string;
										for (int i = s5.size(); i < 4; i++) {
											s5 = "0" + s5;
										}
										string p = s5.substr(0, 2);
										string d = s5.substr(2, 2);
										k.dodajuKod(p + d);

									}
									else {   //za equ
										int lokacija = simb.getOffset();
										KodSekcije& k1 = getSekPoImenu("equ");
										kodic = k1.kod.substr(lokacija, 4);
										string p = kodic.substr(0, 2);
										string d = kodic.substr(2, 2);
										k.dodajuKod(p + d);
									
									}
									

								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}
							}
						}


						else if (regex_match(ubacen1, operandPodaci52)) { //dolar simb
							locationCounter += 5;
							string s = ucitan.substr(1, 1) + "9"; //reg
							k.dodajuKod(s);
							k.dodajuKod("00");
							string kodic;
							int vel1 = ubacen1.size() - 1;
							string ubaci = ubacen1.substr(1, vel1);
							if (!tabelasimbola.postojiLabela(ubaci)) {
								cout << "Greska- ne postoji u tabeli simbola niti se uvodi kao extern" << endl; exit(1);
							}
							Simbol simb = tabelasimbola.dohvSimbol(ubaci);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p+d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(ubaci)) { cout << "Ne postoji nijedan simbol sa .word" + ubaci << endl; exit(1);
								}
								int i = tabelasimbola.proveriGlobalnost(ubaci); //0-lok, 1-glob
								if (i == 0) {
									int o;
									if (tabelasimbola.dohvatiSekciju(ubaci) != "equ") {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubaci), 0);
										t.dodajZapis(z); //lokalno
										o = tabelasimbola.dohvatiOffset(ubaci);
										string s5;
										char hex_string[20];

										sprintf(hex_string, "%X", o); //convert number to hex
										s5 = hex_string;
										for (int i = s5.size(); i < 4; i++) {
											s5 = "0" + s5;
										}

										string p = s5.substr(0, 2);
										string d = s5.substr(2, 2);
										k.dodajuKod(p + d);
									}
									else {   //za equ
										int lokacija = simb.getOffset();
										KodSekcije& k1 = getSekPoImenu("equ");
										kodic = k1.kod.substr(lokacija, 4);
										string p = kodic.substr(0, 2);
										string d = kodic.substr(2, 2);
										k.dodajuKod(p + d);

									}

								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}



							}

						}
						else if (regex_match(ubacen1, operandPodaci51) || regex_match(ubacen1, operandPodaci511) || regex_match(ubacen1, operandPodaci512)) {
							locationCounter += 5; string s;

							s = ucitan.substr(1, 1) + "9"; //sta je source

							k.dodajuKod(s);
							string s1 = "00";
							k.dodajuKod(s1);
							int vel = ubacen1.size() - 1;
							string novi = ubacen1.substr(1, vel);

							int n = stoi(novi, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p+d);

						}


						else  if (regex_match(ubacen1, operandPodaci54)) { //skini procenat
						locationCounter += 5;
						KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
						RelTabela& t = relokacionatabela;
						int vel = ubacen1.size()-1;
						string ubacen = ubacen1.substr(1, vel);
						string reg = ucitan.substr(1, 1);
						
						k.dodajuKod(reg+"7");
						k.dodajuKod("05");
						string kodic;
						Simbol s = tabelasimbola.dohvSimbol(ubacen);
						if (s.getSekcija() == "equ") { //GRESKA NE SME NIKO DA KAZE %simbol gde je simbol equ
							//RelZapis z(tekucaSekcija, locationCounter-2, "R_X86_64_PC16", s.getRB(), -2);
							//t.dodajZapis(z);
							KodSekcije& k1 = getSekPoImenu("equ");
							kodic = k1.kod.substr(s.getOffset(), 4);
							string p = kodic.substr(0, 2);
							string d = kodic.substr(2, 2);
							k.dodajuKod(p+d);

						}

						else {
							if (!tabelasimbola.proveriLabelu(ubacen)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen << endl; exit(1); }
							int i = tabelasimbola.proveriGlobalnost(ubacen); //0-lok, 1-glob
							if (i == 0) {
								if (tabelasimbola.dohvatiSekciju(ubacen) != "equ") {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRBSekcije(ubacen), -2);
									t.dodajZapis(z); //lokalno
									int o = tabelasimbola.dohvatiOffset(ubacen);

									string s5;
									char hex_string[20];

									sprintf(hex_string, "%X", o); //convert number to hex
									s5 = hex_string;
									for (int i = s5.size(); i < 4; i++) {
										s5 = "0" + s5;
									}

									string p = s5.substr(0, 2);
									string d = s5.substr(2, 2);
									k.dodajuKod(p + d);
								}

								else {   //za equ
									Simbol s = tabelasimbola.dohvSimbol(ubacen);
										int lokacija = s.getOffset();
										KodSekcije& k1 = getSekPoImenu("equ");
										kodic = k1.kod.substr(lokacija, 4);
										string p = kodic.substr(0, 2);
										string d = kodic.substr(2, 2);
										k.dodajuKod(p + d);

									}

							}
							else {
								RelZapis z(tekucaSekcija, locationCounter-2, "R_X86_64_PC16", tabelasimbola.dohvatiRB(ubacen), -2);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}
						}
                     	}


						else { //kad treba vise reci-srediti

						int velicina = ubacen1.size() - 1;
						string reg = ubacen1.substr(1, velicina);
						string plus, jos;
						ime >> plus;

						if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand"; exit(1); }
						ime >> jos;
						jos.pop_back(); // da se skloni ]
						string r2 = reg.substr(1, 1);
						string r1 = ucitan.substr(1, 1);
						k.dodajuKod(r2 + r1);
						k.dodajuKod("03");

						if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
							|| regex_match(jos, regLiteral2)) {
							int n = stoi(jos, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);
							locationCounter += 5;
						}



							else if (regex_match(jos, operandPodaci53)) {
							locationCounter += 5;
								string kodic;
								Simbol simb = tabelasimbola.dohvSimbol(jos);
								RelTabela& t = relokacionatabela;
								if (simb.getSekcija() == "equ") {
									int lokacija = simb.getOffset();
									KodSekcije& k1 = getSekPoImenu("equ");
									kodic = k1.kod.substr(lokacija, 4);
									string p = kodic.substr(0, 2);
									string d = kodic.substr(2, 2);
									k.dodajuKod(p + d);
									
								}
								/*		else if (simb.getSekcija() == tekucaSekcija) {

										}*/
								else if (simb.getSekcija() == "UND") {
									k.dodajuKod("0000");
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
									t.dodajZapis(z);
								

								}
								else { //razlicite su sekcije

									if (!tabelasimbola.proveriLabelu(jos)) { cout << "Ne postoji nijedan simbol sa .word" + jos << endl; exit(1); }
									int i = tabelasimbola.proveriGlobalnost(jos); //0-lok, 1-glob
									if (i == 0) {
										if (tabelasimbola.dohvatiSekciju(jos) != "equ") {
											RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(jos), 0);
											t.dodajZapis(z); //lokalno
											int o = tabelasimbola.dohvatiOffset(jos);
											string s5;
											char hex_string[20];

											sprintf(hex_string, "%X", o); //convert number to hex
											s5 = hex_string;
											for (int i = s5.size(); i < 4; i++) {
												s5 = "0" + s5;
											}

											string p = s5.substr(0, 2);
											string d = s5.substr(2, 2);
											k.dodajuKod(p + d);
										} 
										else {   //za equ
											Simbol simb = tabelasimbola.dohvSimbol(jos);
											int lokacija = simb.getOffset();
											KodSekcije& k1 = getSekPoImenu("equ");
											kodic = k1.kod.substr(lokacija, 4);
											string p = kodic.substr(0, 2);
											string d = kodic.substr(2, 2);
											k.dodajuKod(p + d);

										}

									}
									else {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
										t.dodajZapis(z);
										k.dodajuKod("0000");
									}
								}



							}//kraj if elsa 

							
						}


					}
					else { //add r1,r2
						string prvi, drugi,ubacen1;
						prvi = ucitan.substr(0, index);
						int velicina = ucitan.size() - index - 1;
						drugi = ucitan.substr(index + 1, velicina);
						ubacen1 = drugi;
						ucitan = prvi;

						//////////
						if (regex_match(ubacen1, regREG)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(1, 1);
							k.dodajuKod(s);
							string s1 = "01"; //reg dir
							k.dodajuKod(s1);            //PROVERI DA LI OBRUNTO STR I LDR

						}
						else if (regex_match(ubacen1, regREGIND1)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(2, 1);
							k.dodajuKod(s);
							string s1 = "02"; //red ind bez pomeraja
							k.dodajuKod(s1);
						}
						else if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1)
							|| regex_match(ubacen1, regLiteral2)) {
							locationCounter += 5;
							string s;

							s = ucitan.substr(1, 1) + "9"; //sta je source- 9 kao registar koji ne postoji


							k.dodajuKod(s);
							string s1 = "04";
							k.dodajuKod(s1);


							int n = stoi(ubacen1, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);
						}
						else if (regex_match(ubacen1, operandPodaci53)) {

							locationCounter += 5;
							string s = ucitan.substr(1, 1) + "9"; //reg
							k.dodajuKod(s);
							k.dodajuKod("04");
							string kodic;

							Simbol simb = tabelasimbola.dohvSimbol(ubacen1);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(ubacen1)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen1 << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(ubacen1); //0-lok, 1-glob
								if (i == 0) {
									if (tabelasimbola.dohvatiSekciju(ubacen1) != "equ") {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubacen1), 0);
										t.dodajZapis(z); //lokalno
										int o = tabelasimbola.dohvatiOffset(ubacen1);
										string s5;
										char hex_string[20];

										sprintf(hex_string, "%X", o); //convert number to hex
										s5 = hex_string;
										for (int i = s5.size(); i < 4; i++) {
											s5 = "0" + s5;
										}

										string p = s5.substr(0, 2);
										string d = s5.substr(2, 2);
										k.dodajuKod(p + d);
									}

									else {   //za equ
										Simbol simb = tabelasimbola.dohvSimbol(ubacen1);
										int lokacija = simb.getOffset();
										KodSekcije& k1 = getSekPoImenu("equ");
										kodic = k1.kod.substr(lokacija, 4);
										string p = kodic.substr(0, 2);
										string d = kodic.substr(2, 2);
										k.dodajuKod(p + d);

									}
								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}
							}
						}


						else if (regex_match(ubacen1, operandPodaci52)) { //dolar simb
							locationCounter += 5;
							string s = ucitan.substr(1, 1) + "9"; //reg
							k.dodajuKod(s);
							k.dodajuKod("00");
							string kodic;
							int vel1 = ubacen1.size() - 1;
							string ubaci = ubacen1.substr(1, vel1);
							if (!tabelasimbola.postojiLabela(ubaci)) {
								cout << "Greska- ne postoji u tabeli simbola niti se uvodi kao extern" << endl; exit(1);
							}
							Simbol simb = tabelasimbola.dohvSimbol(ubaci);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(ubaci)) { cout << "Ne postoji nijedan simbol sa .word" + ubaci << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(ubaci); //0-lok, 1-glob
								if (i == 0) {
									if (tabelasimbola.dohvatiSekciju(ubaci) != "equ") {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubaci), 0);
										t.dodajZapis(z); //lokalno
										int o = tabelasimbola.dohvatiOffset(ubaci);
										string s5;
										char hex_string[20];

										sprintf(hex_string, "%X", o); //convert number to hex
										s5 = hex_string;
										for (int i = s5.size(); i < 4; i++) {
											s5 = "0" + s5;
										}

										string p = s5.substr(0, 2);
										string d = s5.substr(2, 2);
										k.dodajuKod(p + d);
									}
									else {   //za equ
										Simbol simb = tabelasimbola.dohvSimbol(ubaci);
										int lokacija = simb.getOffset();
										KodSekcije& k1 = getSekPoImenu("equ");
										kodic = k1.kod.substr(lokacija, 4);
										string p = kodic.substr(0, 2);
										string d = kodic.substr(2, 2);
										k.dodajuKod(p + d);

									}
								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}



							}

						}
						else if (regex_match(ubacen1, operandPodaci51) || regex_match(ubacen1, operandPodaci511) || regex_match(ubacen1, operandPodaci512)) {
							locationCounter += 5; string s;

							s = ucitan.substr(1, 1) + "9"; //sta je source

							k.dodajuKod(s);
							string s1 = "00";
							k.dodajuKod(s1);
							int vel = ubacen1.size() - 1;
							string novi = ubacen1.substr(1, vel);

							int n = stoi(novi, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);

						}


						else  if (regex_match(ubacen1, operandPodaci54)) { //skini procenat
							locationCounter += 5;
							KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
							RelTabela& t = relokacionatabela;
							int vel = ubacen1.size() - 1;
							string ubacen = ubacen1.substr(1, vel);
							string reg = ucitan.substr(1, 1);

							k.dodajuKod(reg + "7");
							k.dodajuKod("05");
							string kodic;
							Simbol s = tabelasimbola.dohvSimbol(ubacen);
							if (s.getSekcija() == "equ") {
								//RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", s.getRB(), -2);
								//t.dodajZapis(z);
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(s.getOffset(), 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);

							}

							else {
								if (!tabelasimbola.proveriLabelu(ubacen)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(ubacen); //0-lok, 1-glob
								if (i == 0) {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRBSekcije(ubacen), -2);
									t.dodajZapis(z); //lokalno
									int o = tabelasimbola.dohvatiOffset(ubacen);

									string s5;
									char hex_string[20];

									sprintf(hex_string, "%X", o); //convert number to hex
									s5 = hex_string;
									for (int i = s5.size(); i < 4; i++) {
										s5 = "0" + s5;
									}

									string p = s5.substr(0, 2);
									string d = s5.substr(2, 2);
									k.dodajuKod(p + d);
								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRB(ubacen), -2);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}
							}
						}


						else { //kad treba vise reci-srediti

							int velicina = ubacen1.size() - 1;
							string reg = ubacen1.substr(1, velicina);
							string plus, jos;
							ime >> plus;

							if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand"; exit(1); }
							ime >> jos;
							jos.pop_back(); // da se skloni ]
							string r2 = reg.substr(1, 1);
							string r1 = ucitan.substr(1, 1);
							k.dodajuKod(r2 + r1);
							k.dodajuKod("03");

							if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
								|| regex_match(jos, regLiteral2)) {
								int n = stoi(jos, nullptr, 0); //broj koji zelimo
								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", n); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}
								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);
								locationCounter += 5;
							}



							else if (regex_match(jos, operandPodaci53)) {
								locationCounter += 5;
								string kodic;
								Simbol simb = tabelasimbola.dohvSimbol(jos);
								RelTabela& t = relokacionatabela;
								if (simb.getSekcija() == "equ") {
									int lokacija = simb.getOffset();
									KodSekcije& k1 = getSekPoImenu("equ");
									kodic = k1.kod.substr(lokacija, 4);
									string p = kodic.substr(0, 2);
									string d = kodic.substr(2, 2);
									k.dodajuKod(p + d);
								}
								
								else if (simb.getSekcija() == "UND") {
									k.dodajuKod("0000");
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
									t.dodajZapis(z);

								}
								else { //razlicite su sekcije

									if (!tabelasimbola.proveriLabelu(jos)) { cout << "Ne postoji nijedan simbol sa .word" + jos << endl; exit(1); }
									int i = tabelasimbola.proveriGlobalnost(jos); //0-lok, 1-glob
									if (i == 0) {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(jos), 0);
										t.dodajZapis(z); //lokalno
										int o = tabelasimbola.dohvatiOffset(jos);
										string s5;
										char hex_string[20];

										sprintf(hex_string, "%X", o); //convert number to hex
										s5 = hex_string;
										for (int i = s5.size(); i < 4; i++) {
											s5 = "0" + s5;
										}

										string p = s5.substr(0, 2);
										string d = s5.substr(2, 2);
										k.dodajuKod(p + d);

									}
									else {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
										t.dodajZapis(z);
										k.dodajuKod("0000");
									}
								}



							}//kraj if elsa 


						}



						///////
					}
				}

				else { //ucitan je samo npr r1 
					string saZarezom, ucitan2,ubacen1;
					ime >> saZarezom;
					
					if (saZarezom == ",") { //add r1 , r2
						ime >> ucitan2;
						if (regex_match(ucitan, regREG) == false) {
							cout << "Nekorektan poziv " + deo + " instrukcije";  exit(1);
						}
						ubacen1 = ucitan2;
						/////////////////////
						if (regex_match(ubacen1, regREG)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(1, 1);
							k.dodajuKod(s);
							string s1 = "01"; //reg dir
							k.dodajuKod(s1);            //PROVERI DA LI OBRUNTO STR I LDR

						}
						else if (regex_match(ubacen1, regREGIND1)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(2, 1);
							k.dodajuKod(s);
							string s1 = "02"; //red ind bez pomeraja
							k.dodajuKod(s1);
						}
						else if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1)
							|| regex_match(ubacen1, regLiteral2)) {
							locationCounter += 5;
							string s;

							s = ucitan.substr(1, 1) + "9"; //sta je source- 9 kao registar koji ne postoji


							k.dodajuKod(s);
							string s1 = "04";
							k.dodajuKod(s1);


							int n = stoi(ubacen1, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);
						}
						else if (regex_match(ubacen1, operandPodaci53)) {

							locationCounter += 5;
							string s = ucitan.substr(1, 1) + "9"; //reg
							k.dodajuKod(s);
							k.dodajuKod("04");
							string kodic;

							Simbol simb = tabelasimbola.dohvSimbol(ubacen1);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(ubacen1)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen1 << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(ubacen1); //0-lok, 1-glob
								if (i == 0) {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubacen1), 0);
									t.dodajZapis(z); //lokalno
									int o = tabelasimbola.dohvatiOffset(ubacen1);
									string s5;
									char hex_string[20];

									sprintf(hex_string, "%X", o); //convert number to hex
									s5 = hex_string;
									for (int i = s5.size(); i < 4; i++) {
										s5 = "0" + s5;
									}

									string p = s5.substr(0, 2);
									string d = s5.substr(2, 2);
									k.dodajuKod(p + d);

								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}
							}
						}


						else if (regex_match(ubacen1, operandPodaci52)) { //dolar simb
							locationCounter += 5;
							string s = ucitan.substr(1, 1) + "9"; //reg
							k.dodajuKod(s);
							k.dodajuKod("00");
							string kodic;
							int vel1 = ubacen1.size() - 1;
							string ubaci = ubacen1.substr(1, vel1); 
							if (!tabelasimbola.postojiLabela(ubaci)) {
								cout << "Greska- ne postoji u tabeli simbola niti se uvodi kao extern" << endl; exit(1);
							}
							Simbol simb = tabelasimbola.dohvSimbol(ubaci);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(ubaci)) { cout << "Ne postoji nijedan simbol sa .word" + ubaci << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(ubaci); //0-lok, 1-glob
								if (i == 0) {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubaci), 0);
									t.dodajZapis(z); //lokalno
									int o = tabelasimbola.dohvatiOffset(ubaci);
									string s5;
									char hex_string[20];

									sprintf(hex_string, "%X", o); //convert number to hex
									s5 = hex_string;
									for (int i = s5.size(); i < 4; i++) {
										s5 = "0" + s5;
									}

									string p = s5.substr(0, 2);
									string d = s5.substr(2, 2);
									k.dodajuKod(p + d);

								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}



							}

						}
						else if (regex_match(ubacen1, operandPodaci51) || regex_match(ubacen1, operandPodaci511) || regex_match(ubacen1, operandPodaci512)) {
							locationCounter += 5; string s;

							s = ucitan.substr(1, 1) + "9"; //sta je source

							k.dodajuKod(s);
							string s1 = "00";
							k.dodajuKod(s1);
							int vel = ubacen1.size() - 1;
							string novi = ubacen1.substr(1, vel);

							int n = stoi(novi, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);

						}


						else  if (regex_match(ubacen1, operandPodaci54)) { //skini procenat
							locationCounter += 5;
							KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
							RelTabela& t = relokacionatabela;
							int vel = ubacen1.size() - 1;
							string ubacen = ubacen1.substr(1, vel);
							string reg = ucitan.substr(1, 1);

							k.dodajuKod(reg + "7");
							k.dodajuKod("05");
							string kodic;
							Simbol s = tabelasimbola.dohvSimbol(ubacen);
							if (s.getSekcija() == "equ") {
							//	RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", s.getRB(), -2);
							//	t.dodajZapis(z);
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(s.getOffset(), 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);

							}

							else {
								if (!tabelasimbola.proveriLabelu(ubacen)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(ubacen); //0-lok, 1-glob
								if (i == 0) {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRBSekcije(ubacen), -2);
									t.dodajZapis(z); //lokalno
									int o = tabelasimbola.dohvatiOffset(ubacen);

									string s5;
									char hex_string[20];

									sprintf(hex_string, "%X", o); //convert number to hex
									s5 = hex_string;
									for (int i = s5.size(); i < 4; i++) {
										s5 = "0" + s5;
									}

									string p = s5.substr(0, 2);
									string d = s5.substr(2, 2);
									k.dodajuKod(p + d);
								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRB(ubacen), -2);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}
							}
						}


						else { //kad treba vise reci-srediti

							int velicina = ubacen1.size() - 1;
							string reg = ubacen1.substr(1, velicina);
							string plus, jos;
							ime >> plus;
							locationCounter += 5;
							if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand"; exit(1); }
							ime >> jos;
							jos.pop_back(); // da se skloni ]
							string r2 = reg.substr(1, 1);
							string r1 = ucitan.substr(1, 1);
							k.dodajuKod(r2 + r1);
							k.dodajuKod("03");

							if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
								|| regex_match(jos, regLiteral2)) {
								int n = stoi(jos, nullptr, 0); //broj koji zelimo
								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", n); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}
								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);
								
							}



							else if (regex_match(jos, operandPodaci53)) {

								string kodic;
								Simbol simb = tabelasimbola.dohvSimbol(jos);
								RelTabela& t = relokacionatabela;
								if (simb.getSekcija() == "equ") {
									int lokacija = simb.getOffset();
									KodSekcije& k1 = getSekPoImenu("equ");
									kodic = k1.kod.substr(lokacija, 4);
									string p = kodic.substr(0, 2);
									string d = kodic.substr(2, 2);
									k.dodajuKod(p + d);
								}
								/*		else if (simb.getSekcija() == tekucaSekcija) {

										}*/
								else if (simb.getSekcija() == "UND") {
									k.dodajuKod("0000");
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
									t.dodajZapis(z);

								}
								else { //razlicite su sekcije

									if (!tabelasimbola.proveriLabelu(jos)) { cout << "Ne postoji nijedan simbol sa .word" + jos << endl; exit(1); }
									int i = tabelasimbola.proveriGlobalnost(jos); //0-lok, 1-glob
									if (i == 0) {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(jos), 0);
										t.dodajZapis(z); //lokalno
										int o = tabelasimbola.dohvatiOffset(jos);
										string s5;
										char hex_string[20];

										sprintf(hex_string, "%X", o); //convert number to hex
										s5 = hex_string;
										for (int i = s5.size(); i < 4; i++) {
											s5 = "0" + s5;
										}

										string p = s5.substr(0, 2);
										string d = s5.substr(2, 2);
										k.dodajuKod(p + d);

									}
									else {
										RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
										t.dodajZapis(z);
										k.dodajuKod("0000");
									}
								}



							}//kraj if elsa 


						}


					////////////////////	*/
					}
					else { //add r1 ,r2
						string bezZareza,ubacen1;
						int velicina = saZarezom.size() - 1;
						bezZareza = saZarezom.substr(1, velicina);
						ubacen1 = bezZareza;
						/////////////////////
						if (regex_match(ubacen1, regREG)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(1, 1);
							k.dodajuKod(s);
							string s1 = "01"; //reg dir
							k.dodajuKod(s1);           

						}
						else if (regex_match(ubacen1, regREGIND1)) {
							locationCounter += 3;
							string s = ucitan.substr(1, 1) + ubacen1.substr(2, 1);
							k.dodajuKod(s);
							string s1 = "02"; //red ind bez pomeraja
							k.dodajuKod(s1);
						}
						else if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1)
							|| regex_match(ubacen1, regLiteral2)) {
							locationCounter += 5;
							string s;

							s = ucitan.substr(1, 1) + "9"; //sta je source- 9 kao registar koji ne postoji


							k.dodajuKod(s);
							string s1 = "04";
							k.dodajuKod(s1);


							int n = stoi(ubacen1, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);
						}
						else if (regex_match(ubacen1, operandPodaci53)) {

							locationCounter += 5;
							string s = ucitan.substr(1, 1) + "9"; //reg
							k.dodajuKod(s);
							k.dodajuKod("04");
							string kodic;

							Simbol simb = tabelasimbola.dohvSimbol(ubacen1);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
							k.dodajuKod("0000");
							RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
							t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(ubacen1)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen1 << endl; exit(1); }
							int i = tabelasimbola.proveriGlobalnost(ubacen1); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubacen1), 0);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(ubacen1);
								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);

							}
							else {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}
							}
						}


						else if (regex_match(ubacen1, operandPodaci52)) { //dolar simb
						locationCounter += 5;
						string s = ucitan.substr(1, 1) + "9"; //reg
						k.dodajuKod(s);
						k.dodajuKod("00");
						string kodic;
						int vel1 = ubacen1.size() - 1;
						string ubaci = ubacen1.substr(1, vel1);
						if (!tabelasimbola.postojiLabela(ubaci)) {
							cout << "Greska- ne postoji u tabeli simbola niti se uvodi kao extern" << endl; exit(1);
						}
						Simbol simb = tabelasimbola.dohvSimbol(ubaci);
						RelTabela& t = relokacionatabela;
						if (simb.getSekcija() == "equ") {
							int lokacija = simb.getOffset();
							KodSekcije& k1 = getSekPoImenu("equ");
							kodic = k1.kod.substr(lokacija, 4);
							string p = kodic.substr(0, 2);
							string d = kodic.substr(2, 2);
							k.dodajuKod(p + d);
						}
						/*		else if (simb.getSekcija() == tekucaSekcija) {

								}*/
						else if (simb.getSekcija() == "UND") {
							k.dodajuKod("0000");
							RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
							t.dodajZapis(z);

						}
						else { //razlicite su sekcije

							if (!tabelasimbola.proveriLabelu(ubaci)) { cout << "Ne postoji nijedan simbol sa .word" + ubaci << endl; exit(1); }
							int i = tabelasimbola.proveriGlobalnost(ubaci); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubaci), 0);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(ubaci);
								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);

							}
							else {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubaci), 0);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}



						    }

						}
						else if (regex_match(ubacen1, operandPodaci51) || regex_match(ubacen1, operandPodaci511) || regex_match(ubacen1, operandPodaci512)) {
						locationCounter += 5; string s;

						s = ucitan.substr(1, 1) + "9"; //sta je source

						k.dodajuKod(s);
						string s1 = "00";
						k.dodajuKod(s1);
						int vel = ubacen1.size() - 1;
						string novi = ubacen1.substr(1, vel);

						int n = stoi(novi, nullptr, 0); //broj koji zelimo
						string s5;
						char hex_string[20];

						sprintf(hex_string, "%X", n); //convert number to hex
						s5 = hex_string;
						for (int i = s5.size(); i < 4; i++) {
							s5 = "0" + s5;
						}
						string p = s5.substr(0, 2);
						string d = s5.substr(2, 2);
						k.dodajuKod(p + d);

						}


						else  if (regex_match(ubacen1, operandPodaci54)) { //skini procenat
						locationCounter += 5;
						KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
						RelTabela& t = relokacionatabela;
						int vel = ubacen1.size() - 1;
						string ubacen = ubacen1.substr(1, vel);
						string reg = ucitan.substr(1, 1);

						k.dodajuKod(reg + "7");
						k.dodajuKod("05");
						string kodic;
						Simbol s = tabelasimbola.dohvSimbol(ubacen);
						if (s.getSekcija() == "equ") {
						//	RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", s.getRB(), -2);
						//	t.dodajZapis(z);
							KodSekcije& k1 = getSekPoImenu("equ");
							kodic = k1.kod.substr(s.getOffset(), 4);
							string p = kodic.substr(0, 2);
							string d = kodic.substr(2, 2);
							k.dodajuKod(p + d);

						}

						else {
							if (!tabelasimbola.proveriLabelu(ubacen)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen << endl; exit(1); }
							int i = tabelasimbola.proveriGlobalnost(ubacen); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRBSekcije(ubacen), -2);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(ubacen);

								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);
							}
							else {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRB(ubacen), -2);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}
						}
						}


						else { //kad treba vise reci-srediti

						int velicina = ubacen1.size() - 1;
						string reg = ubacen1.substr(1, velicina);
						string plus, jos;
						ime >> plus;
						locationCounter += 5;
						if (plus != "+") { cout << "Nepravilan format " + deo + " instrukcije-pogresan oprand"; exit(1); }
						ime >> jos;
						jos.pop_back(); // da se skloni ]
						string r2 = reg.substr(1, 1);
						string r1 = ucitan.substr(1, 1);
						k.dodajuKod(r2 + r1);
						k.dodajuKod("03");

						if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
							|| regex_match(jos, regLiteral2)) {
							int n = stoi(jos, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);
							
						}



						else if (regex_match(jos, operandPodaci53)) {

							string kodic;
							Simbol simb = tabelasimbola.dohvSimbol(jos);
							RelTabela& t = relokacionatabela;
							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);
							}
							/*		else if (simb.getSekcija() == tekucaSekcija) {

									}*/
							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(jos)) { cout << "Ne postoji nijedan simbol sa .word" + jos << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(jos); //0-lok, 1-glob
								if (i == 0) {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(jos), 0);
									t.dodajZapis(z); //lokalno
									int o = tabelasimbola.dohvatiOffset(jos);
									string s5;
									char hex_string[20];

									sprintf(hex_string, "%X", o); //convert number to hex
									s5 = hex_string;
									for (int i = s5.size(); i < 4; i++) {
										s5 = "0" + s5;
									}

									string p = s5.substr(0, 2);
									string d = s5.substr(2, 2);
									k.dodajuKod(p + d);

								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}
							}



						}//kraj if elsa 


						}



					///////////////////////	 */
					}

				}

				

			}


			else if (deo == "call" || deo == "jmp" || deo == "jeq" || deo == "jne" || deo == "jgt") {

			KodSekcije& k = this->getSekPoImenu(tekucaSekcija);
			RelTabela& t = relokacionatabela;

			if (deo == "call")k.dodajuKod("30");
			else if (deo == "jmp") k.dodajuKod("50");
			else if (deo == "jeq") k.dodajuKod("51");
			else if (deo == "jne") k.dodajuKod("52");
			else if (deo == "jgt") k.dodajuKod("53");

				string ubacen1,ucitan;
				ime >> ubacen1;

				/////ostalo sve///////////////////////////
				if (ubacen1.find("*") != string::npos) { //ako ima zvezdicu

					int vel = ubacen1.size() - 1;
					ucitan = ubacen1.substr(1, vel);
					ubacen1 = ucitan;
					if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1) || regex_match(ubacen1, regLiteral2)) {
						locationCounter += 5;
						k.dodajuKod("F904");

						int n = stoi(ubacen1, nullptr, 0); //broj koji zelimo
						string s5;
						char hex_string[20];

						sprintf(hex_string, "%X", n); //convert number to hex
						s5 = hex_string;
						for (int i = s5.size(); i < 4; i++) {
							s5 = "0" + s5;
						}
						string p = s5.substr(0, 2);
						string d = s5.substr(2, 2);
						k.dodajuKod(p + d);


					}
					else if (regex_match(ubacen1, regREG)) {
						locationCounter += 3;
						string r = ubacen1.substr(1, 1);
						k.dodajuKod("F" + r + "01");
					}
					else if (regex_match(ubacen1, operandPodaci53)) {
						locationCounter += 5;
						k.dodajuKod("F904");
						///////
						string kodic;
                      	Simbol simb = tabelasimbola.dohvSimbol(ubacen1);

						if (simb.getSekcija() == "equ") {
							int lokacija = simb.getOffset();
							KodSekcije& k1 = getSekPoImenu("equ");
							kodic = k1.kod.substr(lokacija, 4);
							string p = kodic.substr(0, 2);
							string d = kodic.substr(2, 2);
							k.dodajuKod(p + d);
						}

						else if (simb.getSekcija() == "UND") {
							k.dodajuKod("0000");
							RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
							t.dodajZapis(z);

						}
						else { //razlicite su sekcije

							if (!tabelasimbola.proveriLabelu(ubacen1)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen1 << endl; exit(1); }
							int i = tabelasimbola.proveriGlobalnost(ubacen1); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubacen1), 0);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(ubacen1);

								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);

							}
							else {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}
						}

					}
					
					else if ( regex_match(ubacen1, regREGIND1)) {
						locationCounter += 3;
						string r = ubacen1.substr(2, 1);
						k.dodajuKod("F" + r + "02");
					}
					else {
						string plus, jos;
						ime >> plus;
						ime >> jos;
						jos.pop_back();
						if (regex_match(jos, regLiteral) || regex_match(jos, regLiteral1)
							|| regex_match(jos, regLiteral2))  {
							locationCounter += 5;
							string r = ubacen1.substr(2, 1);
							k.dodajuKod("F"+r+"03");

							int n = stoi(jos, nullptr, 0); //broj koji zelimo
							string s5;
							char hex_string[20];

							sprintf(hex_string, "%X", n); //convert number to hex
							s5 = hex_string;
							for (int i = s5.size(); i < 4; i++) {
								s5 = "0" + s5;
							}
							string p = s5.substr(0, 2);
							string d = s5.substr(2, 2);
							k.dodajuKod(p + d);

						}
						else if ( regex_match(jos, operandPodaci53)) {
							locationCounter += 5;

							///////////////
							string r = ubacen1.substr(2, 1);
							k.dodajuKod("F" + r + "03");
							string kodic;
							Simbol simb = tabelasimbola.dohvSimbol(jos);

							if (simb.getSekcija() == "equ") {
								int lokacija = simb.getOffset();
								KodSekcije& k1 = getSekPoImenu("equ");
								kodic = k1.kod.substr(lokacija, 4);
								string p = kodic.substr(0, 2);
								string d = kodic.substr(2, 2);
								k.dodajuKod(p + d);
							}

							else if (simb.getSekcija() == "UND") {
								k.dodajuKod("0000");
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
								t.dodajZapis(z);

							}
							else { //razlicite su sekcije

								if (!tabelasimbola.proveriLabelu(jos)) { cout << "Ne postoji nijedan simbol sa .word" + jos << endl; exit(1); }
								int i = tabelasimbola.proveriGlobalnost(jos); //0-lok, 1-glob
								if (i == 0) {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(jos), 0);
									t.dodajZapis(z); //lokalno
									int o = tabelasimbola.dohvatiOffset(jos);
									
									string s5;
									char hex_string[20];

									sprintf(hex_string, "%X", o); //convert number to hex
									s5 = hex_string;
									for (int i = s5.size(); i < 4; i++) {
										s5 = "0" + s5;
									}
									
									string p = s5.substr(0, 2);
									string d = s5.substr(2, 2);
									k.dodajuKod(p + d);

								}
								else {
									RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(jos), 0);
									t.dodajZapis(z);
									k.dodajuKod("0000");
								}
							}
							///////////
						}
						
					}//plus,jos
				}

				else { //BEZ ZVEZDICE

					if (regex_match(ubacen1, regLiteral) || regex_match(ubacen1, regLiteral1) || regex_match(ubacen1, regLiteral2)) {
						locationCounter += 5;
						k.dodajuKod("F900");

						int n = stoi(ubacen1, nullptr, 0); //broj koji zelimo
						string s5;
						char hex_string[20];

						sprintf(hex_string, "%X", n); //convert number to hex
						s5 = hex_string;
						for (int i = s5.size(); i < 4; i++) {
							s5 = "0" + s5;
						}
						string p = s5.substr(0, 2);
						string d = s5.substr(2, 2);
						k.dodajuKod(p + d);

					}
					else if ( regex_match(ubacen1, operandPodaci53)) {
						locationCounter += 5;
						k.dodajuKod("F900");
						///////
						string kodic;

						Simbol simb = tabelasimbola.dohvSimbol(ubacen1);
						
						if (simb.getSekcija() == "equ") {
							int lokacija = simb.getOffset();
							KodSekcije& k1 = getSekPoImenu("equ");
							kodic = k1.kod.substr(lokacija, 4);
							string p = kodic.substr(0, 2);
							string d = kodic.substr(2, 2);
							k.dodajuKod(p + d);
						}
						
						else if (simb.getSekcija() == "UND") {
							k.dodajuKod("0000");
							RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
							t.dodajZapis(z);

						}
						else { //razlicite su sekcije

							if (!tabelasimbola.proveriLabelu(ubacen1)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen1 << endl; exit(1); }
							int i = tabelasimbola.proveriGlobalnost(ubacen1); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRBSekcije(ubacen1), 0);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(ubacen1);

								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);

							}
							else {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_16", tabelasimbola.dohvatiRB(ubacen1), 0);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}
						}
					

						///////

					}
					else if (regex_match(ubacen1, operandPodaci54)) {
						locationCounter += 5;
						k.dodajuKod("F705"); //nacin adresiranja proveri
						//////
						int vel = ubacen1.size() - 1;
						string u = ubacen1.substr(1, vel);
						ubacen1 = u;
						string kodic;
						Simbol s = tabelasimbola.dohvSimbol(ubacen1);
						if (s.getSekcija() == "equ") {
						//	RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", s.getRB(), -2);
						//	t.dodajZapis(z);
							KodSekcije& k1 = getSekPoImenu("equ");
							kodic = k1.kod.substr(s.getOffset(), 4);
							string p = kodic.substr(0, 2);
							string d = kodic.substr(2, 2);
							k.dodajuKod(p + d);

						}

						else {
							if (!tabelasimbola.proveriLabelu(ubacen1)) { cout << "Ne postoji nijedan simbol sa .word" + ubacen1 << endl; exit(1); }
							int i = tabelasimbola.proveriGlobalnost(ubacen1); //0-lok, 1-glob
							if (i == 0) {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRBSekcije(ubacen1), -2);
								t.dodajZapis(z); //lokalno
								int o = tabelasimbola.dohvatiOffset(ubacen1);

								string s5;
								char hex_string[20];

								sprintf(hex_string, "%X", o); //convert number to hex
								s5 = hex_string;
								for (int i = s5.size(); i < 4; i++) {
									s5 = "0" + s5;
								}

								string p = s5.substr(0, 2);
								string d = s5.substr(2, 2);
								k.dodajuKod(p + d);
							}
							else {
								RelZapis z(tekucaSekcija, locationCounter - 2, "R_X86_64_PC16", tabelasimbola.dohvatiRB(ubacen1), -2);
								t.dodajZapis(z);
								k.dodajuKod("0000");
							}
						}

						/////



					}
				}


              
				/////////////////////////////////////////////

			}



			}//kraj za instrukcije





			//--------------------------------KRAJ----------------------------------------------------------------------//




		}//kraj unutrasnjeg while
	}//kraj spoljasnjeg while

	izlaz <<"TabelaSimbola"<< endl<<tabelasimbola;
	ispisiRelTabelu(izlaz);
	ispisiKodove(izlaz);
	
	
}



void Asembler::asembliraj(string ulaz, string izlaz)
{
	ifstream ulazni;
	ulazni.open(ulaz);
	if (ulazni.is_open() == false) {
		cout << "Neuspesno otvoreni fajlovi"; 
		exit(1);
	}
	ofstream izlazni;
	izlazni.open(izlaz);
	if (izlazni.is_open() == false) {
		cout << "Neuspesno otvoreni fajlovi";
		exit(1);
	}
	prviProlaz(ulazni,izlazni);
    drugiProlaz(ulazni,izlazni);

	ulazni.close();
	izlazni.close();

}


Asembler::~Asembler()
{
}


