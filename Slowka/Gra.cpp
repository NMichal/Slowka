#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>

#include "Gra.h"

using namespace std;


////list<char> litery;
//char litery[] = { 'a', 'a' , 'a', 'a', 'a' , 'a', 'a', 'a' , 'a',
//				'¹',
//				'b','b',
//				'c', 'c', 'c',
//				'æ',
//				'd', 'd', 'd',
//				'e', 'e' , 'e', 'e', 'e' , 'e', 'e',
//				'ê',
//				'f',
//				'g','g',
//				'h','h',
//				'i' , 'i', 'i', 'i' , 'i', 'i', 'i' , 'i',
//				'j', 'j',
//				'k', 'k', 'k',
//				'l', 'l', 'l',
//				'³', '³',
//				'm', 'm', 'm',
//				'n', 'n', 'n' , 'n', 'n',
//				'ñ',
//				'o', 'o', 'o' , 'o', 'o', 'o',
//				'ó',
//				'p', 'p', 'p',
//				'r', 'r', 'r', 'r',
//				's', 's', 's', 's',
//				'œ',
//				't', 't', 't',
//				'u', 'u',
//				'w', 'w', 'w', 'w',
//				'y', 'y', 'y', 'y',
//				'z', 'z', 'z', 'z', 'z',
//				'¿',
//				'Ÿ' };
//list<char> listaLiter(litery, litery + 98);





//int LiczbaPozostalychLiter()
//{
//	int rozmiar = listaLiter.size(); //sizeof(litery)/sizeof(litery[0]); //litery.size();
//	return rozmiar;
//}

char litery[] = { 'a', 'b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','r','s','t','u','w','y','z' };

char LosujLitere()
{

	int index = rand() % 23;
	return litery[index];

}

void WyswietlLiczbePunktow() 
{

}



