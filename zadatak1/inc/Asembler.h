#pragma once
#include<iostream>
#include <fstream>
#include <string>
#include "TabelaSimbola.h"
#include <regex>
#include <list>
#include <iterator>
#include "KodSekcije.h"
#include "RelTabela.h"
#include "TabelaSekcija.h"
using namespace std;


class Asembler
{
private:
	int locationCounter = 0;
	TabelaSimbola tabelasimbola;
public:
	TabelaSekcija tabelasekcija;
	list <KodSekcije> kodovi;
	RelTabela relokacionatabela; 
	KodSekcije& getSekPoImenu(string ime);
	
	bool postojiEqu = false;
	bool first = false;
	int OffsetEqu = 0;
	void dodajuKodove(KodSekcije s);
	void ispisiRelTabelu(ofstream& i);
	void ispisiKodove(ofstream& i);
	int linijakoda=0;

	regex reglabela{ "^([a-zA-Z][a-zA-Z0-9_]*):$" }; 

	regex reginstrukcija{ "^(halt|int|iret|call|ret|jmp|jeq|jne|jgt|push|pop|xchg|add|sub|mul|div|cmp|not|and|or|xor|test|shl|shr|ldr|str)$" }; //radi

	regex regREG{ "^((r[0-7])|psw)$" }; 
	regex regREGIND1{"^\\[r[0-7]\\]$"}; 

	regex regLiteral{ "(^[0-9]+)$" };
	regex regLiteral1{"^(0[0-9]+)$"};
	regex regLiteral2{ "^(0x[0-9A-Fa-f]+)$" };
	

	regex operandPodaci51{"^\\$[0-9]+$"};
	regex operandPodaci511{ "^\\$0[0-9]+$" };
	regex operandPodaci512{ "^\\$0x[0-9A-Fa-f]+$" };
	

	regex operandPodaci52{ "^\\$[a-zA-Z][a-zA-Z0-9_]*$" };
	regex operandPodaci53{ "^[a-zA-Z][a-zA-Z0-9_]*$" };
	regex operandPodaci54{ "^\\%[a-zA-Z][a-zA-Z0-9_]*$" };
	regex zvezdaLiteral{"^\\*[0-9]+$"};


	string worObradaLiterala(string literal);
	
	
	
	Asembler();
	void asembliraj(string ulaz, string izlaz);
	void prviProlaz(ifstream& ulaz, ofstream& izlaz); //ispisuje tabelu simbola
	void drugiProlaz(ifstream& ulaz, ofstream& izlaz); //ispisuje rel zapise i kod
	~Asembler();
};
