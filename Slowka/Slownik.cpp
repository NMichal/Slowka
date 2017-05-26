#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Slownik.h"
#include "Gra.h"

using namespace std;



string Slownik::listaSlow[214251];
void Slownik::ZaczytajSlownik()
{
	ifstream file("slowa.txt");
	if (file.is_open())
	{
		for (int i = 0; i < 214252; ++i)
		{
			file >> Slownik::listaSlow[i];
		}
	}
}


//Funkcja sprawdza czy podane slowo istnieje w slowniku
bool Slownik::SprawdzSlowo(string slowo)
{
	for each (string wyraz in Slownik::listaSlow)
	{
		if (slowo == wyraz)
			return true;
	}
	return false;
}



//bool SprawdzSlowo2(string slowo)
//{
//	ifstream file("slowa.txt");
//	if (file.is_open()) {
//		/*for (int i = 0; i < 2791036; ++i)*/
//		for (int i = 0; i < 214252; ++i)
//		{
//			string temp;
//			file >> temp;
//			//-->>Debug test>>--
//			if (i == 214250)
//				int a = 2;
//			//-->>End debug test>>--
//			if (slowo == temp)  // Wyraz ma inne kodowanie - i zamiast polskich liter s¹ krzaki ;(
//				return true;
//		}
//	}
//	return false;
//}

string  Slownik::KomputerUkladaSlowo(string wylosowaneLitery)
{
	int punkty = 0;
	string slowo = "";
	for (int i = 0; i < 214251; ++i)
	{
		string temp;
		temp = Slownik::listaSlow[i];
		if (CzyMoznaUtworzycSlowo(temp, wylosowaneLitery))
		{
			int pkt = PunktujSlowo(temp);
			if (pkt > punkty)
			{
				punkty = pkt;
				slowo = temp;
			}
		}
	}
	return slowo;
}