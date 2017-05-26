#include <string>
using namespace std;

#ifndef SLOWNIK_H
#define SLOWNIK_H

class Slownik
{
public:	
	void ZaczytajSlownik();
	bool SprawdzSlowo(string slowo);
	bool SprawdzSlowo2(string slowo);
	static string listaSlow[];//[214251]; 
	string KomputerUkladaSlowo(string wylosowaneLitery);



};
#endif // !SLOWNIK_H
