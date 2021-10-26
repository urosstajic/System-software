#pragma once
#include <iostream>
#include <string>
using namespace std;
class OffsetSekcije
{
public:
	string ime;
	int offset; //decimalno
	
	int itielemnt; //kada se koristi u hex pa kaze za koji fajl
	OffsetSekcije(string i);
	OffsetSekcije(string i, int o);
	OffsetSekcije(string i, int o, int ii);
	
   
};

