#pragma once
#include<iostream>
#include<string>
#include <list>
using namespace std;
class KodSekcije{
private: 
	string imeSek;
	

public:
	string kod="";
	int indikator; //-4 za vrednost kada kod nije dodat- za linkable popravljeno
    KodSekcije(string ime); 
	KodSekcije(string ime,int i);
	string getImeSek() const;
    void setImeSek(string ime);

	void dodajuKod(string s);
	friend ostream& operator<<(ostream& it, KodSekcije t);
	~KodSekcije();
};

