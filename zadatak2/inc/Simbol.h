#pragma once
#include<iostream>
#include <string>
using namespace std;
class Simbol
{
private:
	string labela;
	string sekcija;
	int offset; //-1 == ? 
	bool local; //false-global, true-local
	int rb=posrb++; //redni broj
	static int posrb;

public: 
	Simbol(string labela, string sekcija, int offset, bool local);
	Simbol(string labela, string sekcija, int offset, bool local, int rrr);
	void setLocal(bool local);
	void setOffset(int offset);
	void setSekcija(string sek);
	void setLabela(string labela);
	bool getLocal() const;
	int getOffset() const;
	string getSekcija()const;
	string getLabela()const;
	int getRB()const;
	friend ostream& operator<<(ostream& it, const Simbol& s);
	~Simbol();


};

