#pragma once
#include <iostream>
#include <string>
#include "RelZapis.h"
using namespace std;
class PrepravkaRelZapisa
{
public:
	string sekcija;
	int offset;
	string globalnalabela;
	PrepravkaRelZapisa(string s,int o, string l);
};

