#include "TabelaSekcija.h"





bool TabelaSekcija::postojiSekcija(string ime)
{
	list <Sekcija> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if (ime == (*iter).ime) { return true; }
	}

	return false;
}

Sekcija& TabelaSekcija::dohvSek(string deo)
{
	list <Sekcija> ::iterator iter;
	for (iter = tabela.begin(); iter != tabela.end(); ++iter) { //bilo je ++iter
		if (deo == (*iter).ime) { return (*iter); }
	}


}

ostream& operator<<(ostream& it, TabelaSekcija t)
{
	list <Sekcija> ::iterator iter;
	for (iter = t.tabela.begin(); iter != t.tabela.end(); ++iter) { //bilo je ++iter
		it << (*iter) << "\n"; //ne iterira dobro
	}

	return it;
}

void TabelaSekcija::dodajSekciju(Sekcija s)
{
	tabela.push_back(s);
}

