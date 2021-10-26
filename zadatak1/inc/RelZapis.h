#pragma once
#include <iostream>
#include <string>
using namespace std;
class RelZapis{
private:
	string sekcija;
	int offset;
	string tipRealokacije;
	int rb;
	int addend; //za PC_32
public:

	bool instrukcija=true; //automatski true , a postavljamo samo kad nije inst=> word
	int i; //iz kog je fajla
	RelZapis(string s, int o,string t,int r, int a, bool b);
	RelZapis(string s, int o, string t, int r, int a, int ii, bool b);
	RelZapis(string s, int o, string t, int r, int a);
	RelZapis(string s, int o, string t, int r, int a, int ii);
	int getOffset() const;
	string getSekcija()const;
	string getTipRealokacije()const;
	int getRB()const;
	int getAddend()const;

	void setRB(int r);
	void setAddend(int local);
	void setOffset(int offset);
	void setSekcija(string sek);
	void setTipRealokacije(string t);

	friend ostream& operator<<(ostream& it, const RelZapis& s);
	~RelZapis();


};

