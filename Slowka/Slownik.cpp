#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Slownik.h"

using namespace std;



string Slownik::listaSlow[214251];
void Slownik::ZaczytajSlownik()
{
	ifstream file("slowa.txt");
	if (file.is_open())
	{
		for (int i = 0; i < 214252; ++i)
		{
						//-->>Debug test>>--
						if (i == 214250)
							int a = 2;
						//-->>End debug test>>--
			file >> Slownik::listaSlow[i];
		}
	}
}


//Funkcja sprawdza czy podane slowo istnieje w slowniku
bool Slownik::SprawdzSlowo(string slowo)
{
	for each (string wyraz in Slownik::listaSlow)
	{
		if (slowo == wyraz)  //nie dzia³a ostatnie s³owo "zyzania" nie wiadomo czemu .
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