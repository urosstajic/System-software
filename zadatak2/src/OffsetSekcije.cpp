#include "OffsetSekcije.h"

OffsetSekcije::OffsetSekcije(string i)
{
	ime = i;
}

OffsetSekcije::OffsetSekcije(string i, int o)
{
	ime = i;
	offset = o;
}

OffsetSekcije::OffsetSekcije(string i, int o, int ii)
{
	ime = i;
	offset = o;
	itielemnt = ii;
}

