#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Slownik.h"

using namespace std;



/*string listaSlow[2791036];
void ZaczytajSlownik()
{
	ifstream file("slowa.txt");
	if (file.is_open())
	{
		for (int i = 0; i < 2791036; ++i)
		{
			file >> listaSlow[i];
		}
	}
}


//Funkcja sprawdza czy podane slowo istnieje w slowniku
bool SprawdzSlowo(string slowo)
{
	for each (string wyraz in listaSlow)
	{
		if (slowo == wyraz)  // Wyraz ma inne kodowanie - i zamiast polskich liter s¹ krzaki ;(
			return true;
	}
	return false;
}*/



bool SprawdzSlowo2(string slowo)
{
	ifstream file("slowa.txt");
	if (file.is_open()) {
		for (int i = 0; i < 2791036; ++i)
		{
			string temp;
			file >> temp;
			if (slowo == temp)  // Wyraz ma inne kodowanie - i zamiast polskich liter s¹ krzaki ;(
				return true;
		}
	}
	return false;
}