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
//				'�',
//				'b','b',
//				'c', 'c', 'c',
//				'�',
//				'd', 'd', 'd',
//				'e', 'e' , 'e', 'e', 'e' , 'e', 'e',
//				'�',
//				'f',
//				'g','g',
//				'h','h',
//				'i' , 'i', 'i', 'i' , 'i', 'i', 'i' , 'i',
//				'j', 'j',
//				'k', 'k', 'k',
//				'l', 'l', 'l',
//				'�', '�',
//				'm', 'm', 'm',
//				'n', 'n', 'n' , 'n', 'n',
//				'�',
//				'o', 'o', 'o' , 'o', 'o', 'o',
//				'�',
//				'p', 'p', 'p',
//				'r', 'r', 'r', 'r',
//				's', 's', 's', 's',
//				'�',
//				't', 't', 't',
//				'u', 'u',
//				'w', 'w', 'w', 'w',
//				'y', 'y', 'y', 'y',
//				'z', 'z', 'z', 'z', 'z',
//				'�',
//				'�' };
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

//Funkcja zwraca liczb� wyst�pie� danej litery w tekscie
int ZliczLitery(string tekst, char litera) 
{
	int count = 0;

	for (int i = 0; i < tekst.size(); i++)
		if (tekst[i] == litera) count++;

	return count;
}

//Funkcja sprawdza czy mo�na utworzy� wpisane s�owo z posiadanych liter
bool CzyMoznaUtworzycSlowo(string slowo, string litery) 
{
	for (int i = 0; i < slowo.size(); i++)
	{
		int test = ZliczLitery(slowo, slowo[i]);
		int test2 = ZliczLitery(litery, slowo[i]);
		if (ZliczLitery(slowo, slowo[i]) > ZliczLitery(litery, slowo[i]))
			return false;
	}
	return true;
}



