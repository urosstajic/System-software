#pragma once
#include <iostream>
#include <string>
#include "RelZapis.h"
#include <list>


class RelTabela
{ 
	
	
public:
	
	list<RelZapis> tabela;
	RelTabela();
	void dodajZapis(RelZapis s);
	//bool proveriLabelu(string label); //vraca true ako moze da se doda u tabelu simbola


	friend ostream& operator<<(ostream& it,RelTabela t);
	~RelTabela();


};

