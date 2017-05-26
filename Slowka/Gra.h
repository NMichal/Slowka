#include <string> 
#include <list>

using namespace std;

#ifndef GRA_H
#define GRA_H

char LosujLitere();
bool CzyMoznaUtworzycSlowo(string slowo, string litery);
int PunktujSlowo(string slowo);
string LiteryDoWyswietlenia(list<char> litery);

#endif // GRA_H
