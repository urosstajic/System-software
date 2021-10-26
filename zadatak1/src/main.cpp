#include <iostream>
#include <fstream>
#include "Asembler.h"
#include "Linker.h"
#include <list>
using namespace std;

int main(int argc, char* argv[]) {

	if (argc < 4) {
		cout << "Nema dovoljno argumenata za pokretanje programa";
		return 0;
	}



	string ulaz; 
	string izlaz;
	list<string> ofajlovi;
	list<string> place;
	string smesti;
	string ime = argv[1];
	if (ime == "asembler") {

		for (int i = 2; i < argc; i++) {
			string s = argv[i];
			if (s.find("-o") != string::npos) {
				izlaz = argv[++i];
			}
			else  {
				ulaz = argv[i];
			}

		}

		Asembler a;
		a.asembliraj(ulaz, izlaz);

	}
	if (ime == "linker") {
		Linker l;
		string ss = argv[2];
		if (ss == "-linkable") {
			for (int i = 3; i < argc; i++) {
				string s = argv[i];
				if (s.find("-o") != string::npos) {
					smesti = argv[++i];
				}
				else if (s.find("-place") == string::npos) {
					ofajlovi.push_back(s);
				}
				
			}
				l.linkable(ofajlovi,smesti);
		}
		else if (ss == "-hex") {

			for (int i = 3; i < argc; i++) {
				string s = argv[i];
				if (s.find("-o") != string::npos) {
					smesti = argv[++i];
				}
				else if (s.find("-place") == string::npos) {
					ofajlovi.push_back(s);
				}
				else { //objekti fajlovi
					place.push_back(s);
				}

			}
			l.hex(ofajlovi, place, smesti);

		}
	}

	return 0;
}