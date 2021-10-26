#include "TabelaSimbola.h"
#include <stdlib.h>
TabelaSimbola::TabelaSimbola()
{
}




Simbol& TabelaSimbola::dohvSimbRBPo(int rb)
{
	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getRB() == rb) {
			return (*iter);
		}
	}

	
}

bool TabelaSimbola::postojiLabela(string labela)
{
	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela()==labela) { return true; }
	}
	return false;
}

void TabelaSimbola::dodajSimbol(Simbol s)
{
	//ako vec ne postoji taj vec je obezbedjeno u proveri labelu
	tabela.push_back(s);
}

bool TabelaSimbola::proveriLabelu(string label)
{
	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == label) { return true; }
	}

	return false;
}

void TabelaSimbola::postaviGlobal(string lab)
{
	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == lab) {
			(*iter).setLocal(false);
			break;
		}
	}
}

int TabelaSimbola::proveriGlobalnost(string ucitan)
{
	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == ucitan) {
			if ((*iter).getLocal())
				return 0;
			else return 1;
			break;
			
		}
	}
	return 0;
}

int TabelaSimbola::dohvatiRB(string ime)
{

	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == ime) {
			return (*iter).getRB();
			
			break;

		}
	}
	return -2;
}

int TabelaSimbola::dohvatiRBSekcije(string ime)
{
	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == ime) {
			string sek=(*iter).getSekcija();
			return this->dohvatiRB(sek);
			break;

		}
	}
	return 0;
}

int TabelaSimbola::dohvatiOffset(string ucitan)
{

	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == ucitan) {
		
			return  (*iter).getOffset();
			

		}
	}
	return 0;
}

string TabelaSimbola::dohvatiSekciju(string ucitan)
{

	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == ucitan) {

			return  (*iter).getSekcija();


		}
	}
	return "";
}



int TabelaSimbola::dohvatiOffsetRB(int rbb)
{

	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getRB()==rbb) {

			return  (*iter).getOffset();


		}
	}
	return 0;
}





Simbol& TabelaSimbola::dohvSimbol(string ime)
{
	
	list <Simbol> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if ((*iter).getLabela() == ime) {
			return (*iter);

		}
	}

}


ostream& operator<<(ostream& it, TabelaSimbola t)
{
	list <Simbol> ::iterator iter;
	for (iter = t.tabela.begin(); iter != t.tabela.end(); ++iter) { //bilo je ++iter
	      it << (*iter)<<"\n"; //ne iterira dobro
	}
	
	return it;
}

TabelaSimbola::~TabelaSimbola()
{
	//tabela.~list();
}



