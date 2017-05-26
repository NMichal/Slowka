#include <string> 
#include <list>

using namespace std;

#ifndef GRA_H
#define GRA_H



//int LiczbaPozostalychLiter();

char LosujLitere();
bool CzyMoznaUtworzycSlowo(string slowo, string litery);
string KomputerUkladaSlowo(string wylosowaneLitery);
void WymienLitery();
int PunktujSlowo(string slowo);
string LiteryDoWyswietlenia(list<char> litery);



#endif // GRA_H
