#include <string>
using namespace std;

#ifndef SLOWNIK_H
#define SLOWNIK_H

class Slownik
{
public:	
	void ZaczytajSlownik();
	bool SprawdzSlowo(string slowo);
	static string listaSlow[];
	string KomputerUkladaSlowo(string wylosowaneLitery);
};
#endif // !SLOWNIK_H
