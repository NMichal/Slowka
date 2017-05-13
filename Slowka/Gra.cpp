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

void WymienLitery(list<char> litery) 
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


int PunktujSlowo(string slowo)
{
	int punkty = 0;
	for (int i = 0; i < slowo.size(); i++)
	{
		int count = ZliczLitery(slowo, slowo[i]);

		if (slowo[i] == 'a' || slowo[i] == 'e' || slowo[i] == 'i' || slowo[i] == 'n' || slowo[i] == 'o' || slowo[i] == 'r' || slowo[i] == 's' || slowo[i] == 'w' || slowo[i] == 'z')
			punkty += count * 1;

		if (slowo[i] == 'c' || slowo[i] == 'd' || slowo[i] == 'k' || slowo[i] == 'l' || slowo[i] == 'm' || slowo[i] == 'p' || slowo[i] == 't' || slowo[i] == 'y')
			punkty += count * 2;

		if (slowo[i] == 'b' || slowo[i] == 'g' || slowo[i] == 'h' || slowo[i] == 'j' || slowo[i] == 'u')
			punkty += count * 3;

		if (slowo[i] == 'f')
			punkty += count * 5;
	}

	return punkty;
}


