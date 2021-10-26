#include "RelTabela.h"

RelTabela::RelTabela()
{
	
}

void RelTabela::dodajZapis(RelZapis s)
{
	tabela.push_back(s);
}

RelTabela::~RelTabela()
{
}

ostream& operator<<(ostream& it,  RelTabela t)
{
	
		it << "RelokacionaTabela" << endl;
		list <RelZapis> ::iterator iter;
		for (iter = t.tabela.begin(); iter != t.tabela.end(); ++iter) { //bilo je ++iter
			it << (*iter) << "\n"; //ne iterira dobro
		}
	
	return it;
}
