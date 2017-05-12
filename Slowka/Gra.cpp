#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>

#include "Gra.h"

using namespace std;

char litery[] = { 'a', 'b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','r','s','t','u','w','y','z' };

char LosujLitere()
{

	int index = rand() % 23;
	return litery[index];
}

void WymienLitery() 
{

}

void WyswietlLiczbePunktow() 
{

}

//Funkcja zwraca liczbê wyst¹pieñ danej litery w tekscie
int ZliczLitery(string tekst, char litera) 
{
	int count = 0;

	for (int i = 0; i < tekst.size(); i++)
		if (tekst[i] == litera) count++;

	return count;
}

//Funkcja sprawdza czy mo¿na utworzyæ wpisane s³owo z posiadanych liter
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

string KomputerUkladaSlowo(string wylosowaneLitery)
{
	ifstream file("slowa.txt");
	if (file.is_open()) {
		for (int i = 0; i < 2791036; ++i)
		{
			string temp;
			file >> temp;
			if (CzyMoznaUtworzycSlowo(temp, wylosowaneLitery))
				return temp;
		}
	}
}



