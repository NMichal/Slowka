#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <future>

#include "Gra.h"
#include "Slownik.h"

using namespace std;


char litery[] = { 'a', 'b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','r','s','t','u','w','y','z' };

char Gra::LosujLitere()
{

	int index = rand() % 23;
	return litery[index];
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
bool Gra::CzyMoznaUtworzycSlowo(string slowo, string litery)
{
	for (int i = 0; i < slowo.size(); i++)
	{
		if (ZliczLitery(slowo, slowo[i]) > ZliczLitery(litery, slowo[i]))
			return false;
	}
	return true;
}

int Gra::PunktujSlowo(string slowo)
{
	int punkty = 0;
	for (int i = 0; i < slowo.size(); i++)
	{
		if (slowo[i] == 'a' || slowo[i] == 'e' || slowo[i] == 'i' || slowo[i] == 'n' || slowo[i] == 'o' || slowo[i] == 'r' || slowo[i] == 's' || slowo[i] == 'w' || slowo[i] == 'z')
			punkty += 1;

		if (slowo[i] == 'c' || slowo[i] == 'd' || slowo[i] == 'k' || slowo[i] == 'l' || slowo[i] == 'm' || slowo[i] == 'p' || slowo[i] == 't' || slowo[i] == 'y')
			punkty += 2;

		if (slowo[i] == 'b' || slowo[i] == 'g' || slowo[i] == 'h' || slowo[i] == 'j' || slowo[i] == 'u')
			punkty += 3;

		if (slowo[i] == 'f')
			punkty += 5;
	}

	return punkty;
}


string Gra::LiteryDoWyswietlenia(list<char> litery)
{
	string strLitery = "";
	for (auto v : litery)
	{
		strLitery += v;
		strLitery += " ";
	}
	return strLitery;
}


