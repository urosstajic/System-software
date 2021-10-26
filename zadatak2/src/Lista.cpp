#include "Lista.h"
Lista::Lista(list<Sekcija>l) {
	this->koje_sekcije = l;
}

int Lista::nadjiPocetnuAdresu(string naziv) {
	//cout << "lista";
	int vel = 0;
	list<Sekcija>::iterator i;
	for (i = this->koje_sekcije.begin(); i != koje_sekcije.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			vel += (*i).getVelicina();
		}
	}
	//cout << vel << "Ovo je vel";
	return vel;
}

int Lista::nadjiAdrZaLinkable(string naziv) {
	int vrati = 0;
	list<Sekcija>::iterator i;
	for (i = koje_sekcije.begin(); i != koje_sekcije.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			vrati += (*i).getVelicina();
		}
	}
	return vrati;
}

int Lista::nadjiAdrZaHex(string naziv) {
	int vrati = 0;
	list<Sekcija>::iterator i;
	for (i = koje_sekcije.begin(); i != koje_sekcije.end(); i++) {
		if ((*i).getNaziv() == naziv) {
			vrati += (*i).getVelicina();
			return vrati;
		}
		else {
			
			continue;
		}
	}
	return vrati;
}