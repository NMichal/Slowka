#include <iostream> 
#include <windows.h>
#include <string> 
#include <commctrl.h>
#include <list>
#include <ctime>

#include "Slownik.h"
#include "Gra.h"

//Zrobiæ na dowoln¹ liczbe graczy
using namespace std;

#define ID_ETYKIETA_LICZBA_LITEREK 10
#define ID_ETYKIETA_PUNKTY_GRACZA 11
#define ID_ETYKIETA_PUNKTY_KOMPUTERA 12
#define ID_RADIO_TURY 13
#define ID_RADIO_PUNKTY 14

//Pozmieniaæ teksty okien 

///mo¿emy sparsowaæ liniê œcie¿ki ¿eby dodaæ coœ ciekawego np. jakieœ cheaty - u³atwienia

//Czytanie ze s³owanika
//Sprawdzanie czy podane s³owo istnieje w s³owniku 
//Zliczanie punktów
//?? mo¿e na pocz¹tku pytanie ile tur - max - do skoñczenia literek
//pass - gdy nie mamy nic (chocia¿ mo¿e lepiej tylko wymieñ literki)
// Jakieœ komunikaty
// Licznik ile pozosta³o literek w "worku"
// lowercase - przy pobieraniu z textboxa (sprawdziæ czy wszystko w s³owniku bêdzie ma³e)

//wylaczyc maksymalizacje
// byæ mo¿e póŸniej dodaæ ID_PRZYCISKOW 

list<char> literyKomputera;
list<char> literyGracza;
int punktyKomputera = 0;
int punktyGracza = 0;
int tura = 1;
int *ptura = &tura;



Slownik slownik;
LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma region

HWND buttonWymienLiterki;
HWND buttonZatwierdz;
HWND buttonPomin;
HWND textBoxWpisaneSlowo;
//HWND staticTextEtykietaLiczbaLiterek;
//HWND staticTextLiczbaLiterek;
HWND staticTextEtykietaTwojeLitery;
HWND staticTextTwojeLitery;
HWND listViewRozgrywka;
HWND staticTextEtykietaPunktyGracza;
HWND staticTextPunktyGracza;
HWND staticTextEtykietaPunktyKomputera;
HWND staticTextPunktyKomputera;
RECT rcl; // list view bo nale¿y co grupy Common Controls

//elementy DW
HWND buttonOK;
HWND textBoxLiczbaTurPunktow;
HWND staticTextTuryPunkty;
HWND radioTury;
HWND radioPunkty;


#pragma endregion Deklaracja kontrolek okna


///Funkcja wstawiajaca do listView wpis
void RozgrywkaInsert(int tura, string osoba, string slowo, int punkty)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;

	string tura_str = to_string(tura);
	LPSTR tura_lpstr = const_cast<char *>(tura_str.c_str());
	lvi.pszText = tura_lpstr;
	lvi.iItem = tura - 1;
	lvi.iSubItem = 0;

	ListView_InsertItem(listViewRozgrywka, &lvi);

	LPSTR osoba_lpstr = const_cast<char *>(osoba.c_str());
	ListView_SetItemText(listViewRozgrywka, tura - 1, 1, osoba_lpstr);

	LPSTR slowo_lpstr = const_cast<char *>(slowo.c_str());
	ListView_SetItemText(listViewRozgrywka, tura - 1, 2, slowo_lpstr);

	string punkty_str = to_string(punkty);
	LPSTR punkty_lpstr = const_cast<char *>(punkty_str.c_str());
	ListView_SetItemText(listViewRozgrywka, tura - 1, 3, punkty_lpstr);

	(*ptura)++;
}


void RuchKomputera(HWND hwnd)
{
	literyKomputera.clear();

	while (literyKomputera.size() < 9)	//Wymiana liter przed ruchem PC
		literyKomputera.push_back(LosujLitere());

	string strLiteryKomputera = "";
	for (auto v : literyKomputera)
	{
		strLiteryKomputera += v;
	}
	//Do tabeli mo¿e s³owo | wylosowane litery ? bo jak na weilu graczy ?
	MessageBox(hwnd, "Teraz Komputer ", "Ha!", MB_ICONINFORMATION);
	string ulozoneSlowo = KomputerUkladaSlowo(strLiteryKomputera);
	int punkty = PunktujSlowo(ulozoneSlowo);
	RozgrywkaInsert(*ptura, "Komputer", ulozoneSlowo, punkty);
	punktyKomputera += punkty;
	SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyKomputera, true);
	//Musi tu wymieniaæ litery !
}

///Zmieniæ na jedn¹ funkcjê dla gracza i PC (chocia¿ dla wielu graczy beda tylko jedne zmieniajace sie litery) oraz przeniesc do pliku Gra.cpp
void WymienLiteryGraczaMain()
{
	literyGracza.clear();

	while (literyGracza.size() < 9)
		literyGracza.push_back(LosujLitere());

	string strLiteryGracza = "";
	for (auto v : literyGracza)
	{
		strLiteryGracza += v;
		strLiteryGracza += " ";
	}

	LPCSTR lpcstr = strLiteryGracza.c_str();			//Wyœwietlanie liter - mo¿e do wyœwietlania liter zrobiæ osobne metody 
	SetWindowText(staticTextTwojeLitery, lpcstr);
}

void PominRuch(HWND hwnd)
{
	RozgrywkaInsert(*ptura, "gracz", "", 0);
	RuchKomputera(hwnd);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// WYPE£NIANIE STRUKTURY
	WNDCLASSEX wc; //window class

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = NazwaKlasy;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// REJESTROWANIE KLASY OKNA
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Nie mozna zarejestrowac tego okna!", "Error",
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	// TWORZENIE OKNA
	HWND OknoAplikacji;

	OknoAplikacji = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, "Oto okienko", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 950, 700, NULL, NULL, hInstance, NULL);

	if (OknoAplikacji == NULL)
	{
		MessageBox(NULL, "Nie udalo sie stworzyc okna aplikacji!", "Error", MB_ICONEXCLAMATION);
		return 1;
	}


	//DIALOG WINDOW
	// WYPE£NIANIE STRUKTURY
	WNDCLASSEX dw; //dialog window
	LPSTR NazwaWd = "Wd";

	dw.cbSize = sizeof(WNDCLASSEX);
	dw.style = 0;
	dw.lpfnWndProc = WndProc;
	dw.cbClsExtra = 0;
	dw.cbWndExtra = 0;
	dw.hInstance = hInstance;
	dw.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	dw.hCursor = LoadCursor(NULL, IDC_ARROW);
	dw.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	dw.lpszMenuName = NULL;
	dw.lpszClassName = NazwaWd;
	dw.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// REJESTROWANIE KLASY OKNA
	if (!RegisterClassEx(&dw))
	{
		MessageBox(NULL, "Nie mozna zarejestrowac tego okna!", "Error",
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	// TWORZENIE OKNA
	HWND DialogWindow;

	DialogWindow = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaWd, "Oto okienko", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);

	if (DialogWindow == NULL)
	{
		MessageBox(NULL, "Nie udalo sie stworzyc okna aplikacji!", "Error", MB_ICONEXCLAMATION);
		return 1;
	}

#pragma region
	////----------------Elementy DW-----------------------/////////
	buttonOK = CreateWindowEx(0, "BUTTON", "ZatwierdŸ", WS_CHILD | WS_VISIBLE,
		10, 100, 150, 30, DialogWindow, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

	textBoxLiczbaTurPunktow = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		10, 60, 250, 30, DialogWindow, NULL, hInstance, NULL);

	staticTextTuryPunkty = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 10, 20, 150, 30, DialogWindow, NULL, hInstance, NULL);
	SetWindowText(staticTextTuryPunkty, "Gra do: liczba tur / liczba punktów:");

	radioTury = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Liczba tur",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		10, 140, 150, 30,
		DialogWindow, (HMENU)ID_RADIO_TURY,
		GetModuleHandle(NULL), 0);
	//SendMessage(radioTury, WM_SETFONT, (WPARAM)font, TRUE);

	radioPunkty = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Liczba punktów",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		200, 140, 150, 30,
		DialogWindow, (HMENU)ID_RADIO_PUNKTY,
		GetModuleHandle(NULL), 0);
	//SendMessage(radioPunkty, WM_SETFONT, (WPARAM)font, TRUE);

	////----------------End Elementy DW-----------------------/////////
	buttonWymienLiterki = CreateWindowEx(0, "BUTTON", "Wymieñ literki", WS_CHILD | WS_VISIBLE,
		550, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

	buttonZatwierdz = CreateWindowEx(0, "BUTTON", "Zatwierdz", WS_CHILD | WS_VISIBLE,
		350, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

	buttonPomin = CreateWindowEx(0, "BUTTON", "Pomin ruch", WS_CHILD | WS_VISIBLE,
		750, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

	textBoxWpisaneSlowo = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		50, 600, 250, 30, OknoAplikacji, NULL, hInstance, NULL);

	//staticTextEtykietaLiczbaLiterek = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
	//	SS_LEFT, 600, 50, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	//SetWindowText(staticTextEtykietaLiczbaLiterek, "Liczba pozosta³ych \n\tliterek:");

	//staticTextLiczbaLiterek = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
	//	SS_CENTER, 600, 80, 150, 30, OknoAplikacji, (HMENU)ID_ETYKIETA_LICZBA_LITEREK, hInstance, NULL);
	////--------------------------------------------------------------------------------------------------------------------------------
	//int i = 10;
	//SetDlgItemInt(OknoAplikacji, ID_ETYKIETA_LICZBA_LITEREK, i, true);	//---!!!---Ustwaienie etyliety liczby literek z inta ---!!!---
	////--------------------------------------------------------------------------------------------------------------------------------

	staticTextEtykietaTwojeLitery = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 50, 520, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	SetWindowText(staticTextEtykietaTwojeLitery, "Twoje literki:");

	staticTextTwojeLitery = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 50, 550, 250, 30, OknoAplikacji, NULL, hInstance, NULL);

	//--------------------------------------------------------------------------------------------------------------------------------
	//string test = "A B C D E F";
	//LPCSTR lpcstr = test.c_str();			//Wyœwietlanie liter - mo¿e do wyœwietlania liter i liczby liter zrobiæ osobne metody 
	//SetWindowText(staticTextTwojeLitery, lpcstr);

	//--------------------------------------------------------------------------------------------------------------------------------


	staticTextEtykietaPunktyGracza = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 750, 170, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	SetWindowText(staticTextEtykietaPunktyGracza, "Punkty Gracza:");

	staticTextPunktyGracza = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 750, 200, 150, 30, OknoAplikacji, (HMENU)ID_ETYKIETA_PUNKTY_GRACZA, hInstance, NULL);
	//--------------------------------------------------------------------------------------------------------------------------------
	//int pktGracz = 120;
	SetDlgItemInt(OknoAplikacji, ID_ETYKIETA_PUNKTY_GRACZA, punktyGracza, true);	//---!!!---Ustwaienie etyliety punkty gracza z inta ---!!!---
	//--------------------------------------------------------------------------------------------------------------------------------



	staticTextEtykietaPunktyKomputera = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 750, 250, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	SetWindowText(staticTextEtykietaPunktyKomputera, "Punkty Komputera:");

	staticTextPunktyKomputera = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 750, 280, 150, 30, OknoAplikacji, (HMENU)ID_ETYKIETA_PUNKTY_KOMPUTERA, hInstance, NULL);
	//--------------------------------------------------------------------------------------------------------------------------------
	//int pktKomputer = 20;
	SetDlgItemInt(OknoAplikacji, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyKomputera, true);	//---!!!---Ustwaienie etyliety punkty komputera z inta ---!!!---
	//--------------------------------------------------------------------------------------------------------------------------------


	listViewRozgrywka = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT |
		LVS_EDITLABELS, 50, 50, 500, 450,
		OknoAplikacji, (HMENU)1000, hInstance, NULL);

#pragma region
	LVCOLUMN lvc;
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.cx = 75;
	lvc.pszText = "Tura";
	ListView_InsertColumn(listViewRozgrywka, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.cx = 175;
	lvc.pszText = "Osoba";
	ListView_InsertColumn(listViewRozgrywka, 1, &lvc);

	lvc.iSubItem = 2;
	lvc.cx = 175;
	lvc.pszText = "S³owo";
	ListView_InsertColumn(listViewRozgrywka, 2, &lvc);

	lvc.iSubItem = 3;
	lvc.cx = 55;  // zmniejszone ¿eby siê nie pojawia³ scroll poziomy
	lvc.pszText = "Punkty";
	ListView_InsertColumn(listViewRozgrywka, 3, &lvc);
#pragma endregion Kolumny w listView

#pragma endregion Inicjalizacja kontrolek okna


	//----------------------------------Tutaj rozpoczynamy gre-------------------------------------------------------------------

	//Slownik s2;
	slownik.ZaczytajSlownik();
	//bool elo = SprawdzSlowo("koñ");
	//bool elo = SprawdzSlowo2("auto");

	srand(time(NULL));
	while (literyKomputera.size() < 9)
		literyKomputera.push_back(LosujLitere());

	while (literyGracza.size() < 9)
		literyGracza.push_back(LosujLitere());

	string strLiteryGracza = "";

	for (auto v : literyGracza)
	{
		strLiteryGracza += v;
		strLiteryGracza += " ";
	}



	LPCSTR lpcstr = strLiteryGracza.c_str();			//Wyœwietlanie liter - mo¿e do wyœwietlania liter i liczby liter zrobiæ osobne metody 
	SetWindowText(staticTextTwojeLitery, lpcstr);


	//------------------------------------------------------------------------------------------------------------------------------
	ShowWindow(OknoAplikacji, nCmdShow); // Poka¿ okienko...
	UpdateWindow(OknoAplikacji);

	ShowWindow(DialogWindow, nCmdShow); // Poka¿ okienko DW
	UpdateWindow(DialogWindow);



	//ShowWindow(DialogWindow, SW_HIDE);



	// Pêtla komunikatów - klikniêcie myszk¹, wciœniêcie klawisza czy zamkniêcie okna
	while (GetMessage(&Komunikat, NULL, 0, 0)) // Gdy robimy jak¹œ czynnoœæ, która wi¹¿e siê z powstaniem jakiejœ wiadomoœci, np. poruszamy mysz¹, wiadomoœæ posy³ana jest do kolejki wiadomoœci, sk¹d zabiera j¹ GetMessage. 
	{
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}
	return Komunikat.wParam;
}

// OBS£UGA ZDARZEÑ - funkcja zajmie siê w³aœciw¹ obs³ug¹ komunikatów systemu, czyli np. reakcj¹ na wciskanie ró¿nych klawiszy
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND://obs³uga naszego przycisku buttonWymienLiterki
		if ((HWND)lParam == buttonWymienLiterki) {
			literyGracza.clear();

			while (literyGracza.size() < 9)
				literyGracza.push_back(LosujLitere());

			string strLiteryGracza = "";
			for (auto v : literyGracza)
			{
				strLiteryGracza += v;
				strLiteryGracza += " ";
			}

			LPCSTR lpcstr = strLiteryGracza.c_str();			//Wyœwietlanie liter - mo¿e do wyœwietlania liter i liczby liter zrobiæ osobne metody 
			SetWindowText(staticTextTwojeLitery, lpcstr);
		}
		if ((HWND)lParam == buttonZatwierdz)
		{
			MessageBox(hwnd, "Nacisn¹³eœ przycisk Zatwierdz!", "Ha!", MB_ICONINFORMATION);

			DWORD dlugosc = GetWindowTextLength(textBoxWpisaneSlowo);
			LPSTR Bufor = (LPSTR)GlobalAlloc(GPTR, dlugosc + 1);
			GetWindowText(textBoxWpisaneSlowo, Bufor, dlugosc + 1);

			string wpisaneSlowo(Bufor);

			string strLiteryGracza = "";
			for (auto v : literyGracza)
			{
				strLiteryGracza += v;
			}

			if (CzyMoznaUtworzycSlowo(wpisaneSlowo, strLiteryGracza))
			{
				//bool istniejeSlowo = SprawdzSlowo2(wpisaneSlowo);
				//Slownik s1;
				bool istniejeSlowo = slownik.SprawdzSlowo(wpisaneSlowo);
				if (istniejeSlowo)
				{
					MessageBox(hwnd, "Istnieje takie s³owo !!! ", "Ha!", MB_ICONINFORMATION);
					int punkty = PunktujSlowo(wpisaneSlowo);
					RozgrywkaInsert(*ptura, "gracz", wpisaneSlowo, punkty);
					punktyGracza += punkty;
					SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_GRACZA, punktyGracza, true);
					///Wyczyœciæ pole wpisaneSlowo
					//----->>>>>>>> Przekopiowane z przycisku WymienLiterki - trzeba zrobiæ osobn¹ funkjce wymien liteki <<<<<<<<<<<<<<<-----------------------------------
					WymienLiteryGraczaMain();
					//-------------------->>>>>>>>>>>>>>>>>>>>>>----------------------------------------------<<<<<<<<<<<<<<<<<<<<<<<-------------------------------------



					////Do tabeli mo¿e s³owo | wylosowane litery ? bo jak na weilu graczy ?
					RuchKomputera(hwnd);
				}
				else
				{
					MessageBox(hwnd, "Nie ma takiego slowa", "Ha!", MB_ICONINFORMATION);
				}
			}///prawie gotowe, trzeba tylko podaæ litery które mamy i sprawdziæ ;) 
			// Mo¿na tak¿e u¿yæ tego do generowania przez PC tylko zamiast wpisaneSlowo daæ slowo ze slownika
			else
			{
				MessageBox(hwnd, "Z wylosowanych liter nie mo¿na utworzyæ takiego s³owa.", "Ha!", MB_ICONINFORMATION);
			}
		}
		if ((HWND)lParam == buttonPomin)
		{
			MessageBox(hwnd, "Nacisnales pomin.", "Ha!", MB_ICONINFORMATION);
			PominRuch(hwnd);
		}
		if ((HWND)lParam == buttonOK)
		{
			if (IsDlgButtonChecked(hwnd, ID_RADIO_PUNKTY) == BST_CHECKED) 
			{
				MessageBox(NULL, "Zaznaczono na liczbe punktow", "Dziala !", MB_OK);

				ShowWindow(hwnd, SW_HIDE);
			}
			else if (IsDlgButtonChecked(hwnd, ID_RADIO_TURY) == BST_CHECKED)
			{
				MessageBox(NULL, "Zaznaczono na liczbe tur", "Dziala !", MB_OK);

				ShowWindow(hwnd, SW_HIDE);
			}
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam); // inne odsy³amy dalej - do funkcji DefWindowProc, która zajmie siê ich domyœln¹ obs³ug¹.
	}

	return 0;
}