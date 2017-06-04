#include <string> 
#include <list>

using namespace std;

#ifndef GRA_H
#define GRA_H

class Gra {
public:
	static char LosujLitere();
	static bool CzyMoznaUtworzycSlowo(string slowo, string litery);
	static int PunktujSlowo(string slowo);
	static string LiteryDoWyswietlenia(list<char> litery);
};
#endif // GRA_H
