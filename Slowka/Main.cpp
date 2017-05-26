#include <iostream> 
#include <windows.h>
#include <string> 
#include <commctrl.h>
#include <list>
#include <ctime>

#include "Slownik.h"
#include "Gra.h"

using namespace std;

#define ID_ETYKIETA_LICZBA_LITEREK 10
#define ID_ETYKIETA_PUNKTY_GRACZA 11
#define ID_ETYKIETA_PUNKTY_KOMPUTERA 12
#define ID_RADIO_TURY 13
#define ID_RADIO_PUNKTY 14
#define ID_RADIO_PVSC 15
#define ID_RADIO_PVSP 16
#define ID_RADIO_CVSC 17


//Pozmienia� teksty okien 
// Jakie� komunikaty
//wylaczyc maksymalizacje
// by� mo�e p�niej doda� ID_PRZYCISKOW 

list<char> literyKomputera;
list<char> literyGracza;
int punktyKomputera = 0;
int punktyGracza = 0;
int tura = 1;
int *ptura = &tura;
int graDoTury = -1;
int graDoLiczbyPunktow = -1;
int trybGry = -1; //1 - PVsC, 2 - PVsP, 3 - CVsC



Slownik slownik;
LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma region

HWND buttonWymienLiterki;
HWND buttonZatwierdz;
HWND buttonPomin;
HWND textBoxWpisaneSlowo;
HWND staticTextEtykietaTwojeLitery;
HWND staticTextTwojeLitery;
HWND listViewRozgrywka;
HWND staticTextEtykietaPunktyGracza;
HWND staticTextPunktyGracza;
HWND staticTextEtykietaPunktyKomputera;
HWND staticTextPunktyKomputera;
RECT rcl; // list view bo nale�y co grupy Common Controls

//elementy DW
HWND buttonOK;
HWND textBoxLiczbaTurPunktow;
HWND staticTextTuryPunkty;
HWND radioTury;
HWND radioPunkty;
HWND radioPVsP;
HWND radioPVsC;
HWND radioCVsC;

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

///Zmieni� na jedn� funkcj� dla gracza i PC (chocia� dla wielu graczy beda tylko jedne zmieniajace sie litery) oraz przeniesc do pliku Gra.cpp
void WymienLiteryGraczaMain()
{
	literyGracza.clear();

	while (literyGracza.size() < 9)
		literyGracza.push_back(LosujLitere());

	string strLiteryGracza = LiteryDoWyswietlenia(literyGracza);

	LPCSTR lpcstr = strLiteryGracza.c_str();
	SetWindowText(staticTextTwojeLitery, lpcstr);
}

void RuchKomputera(HWND hwnd)
{
	literyKomputera.clear();

	while (literyKomputera.size() < 9)	//Wymiana liter przed ruchem PC
		literyKomputera.push_back(LosujLitere());

	string literyKomputeraDoWyswietlenia = LiteryDoWyswietlenia(literyKomputera);
	string strLiteryKomputera = "";
	for (auto v : literyKomputera)
	{
		strLiteryKomputera += v;
 	}

	MessageBox(hwnd, "Teraz Komputer ", "Ha!", MB_ICONINFORMATION);
	string ulozoneSlowo = slownik.KomputerUkladaSlowo(strLiteryKomputera);
	int punkty = PunktujSlowo(ulozoneSlowo);
	ulozoneSlowo += " ( " + literyKomputeraDoWyswietlenia + ")";
	RozgrywkaInsert(*ptura, "Komputer", ulozoneSlowo, punkty);
	punktyKomputera += punkty;
	SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyKomputera, true);
	if (graDoLiczbyPunktow != -1 && punktyKomputera >= graDoLiczbyPunktow) 
	{
		MessageBox(hwnd, "Wygral Komputer ", "Ha!", MB_ICONINFORMATION);
		ExitProcess(1);
	}
	if (graDoTury != -1 && *ptura > graDoTury)
	{
		MessageBox(hwnd, "Koniec gry.", "Ha!", MB_ICONINFORMATION);
		ExitProcess(1);
	}
}

bool RuchGracza(HWND hwnd)
{

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
		bool istniejeSlowo = slownik.SprawdzSlowo(wpisaneSlowo);
		if (istniejeSlowo)
		{
			MessageBox(hwnd, "Istnieje takie s�owo !!! ", "Ha!", MB_ICONINFORMATION);
			int punkty = PunktujSlowo(wpisaneSlowo);
			wpisaneSlowo += " ( " + strLiteryGracza + ")";
			RozgrywkaInsert(*ptura, "Gracz", wpisaneSlowo, punkty);
			punktyGracza += punkty;
			SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_GRACZA, punktyGracza, true);
			if (graDoLiczbyPunktow != -1 && punktyGracza >= graDoLiczbyPunktow)
			{
				MessageBox(hwnd, "Wygral Gracz ", "Ha!", MB_ICONINFORMATION);
				ExitProcess(1);
			}
			if (graDoTury != -1 && *ptura > graDoTury)
			{
				MessageBox(hwnd, "Koniec gry.", "Ha!", MB_ICONINFORMATION);
				ExitProcess(1);
			}
			///Wyczy�ci� pole wpisaneSlowo
			WymienLiteryGraczaMain();
			return true;
		}
		else
		{
			MessageBox(hwnd, "Nie ma takiego slowa", "Ha!", MB_ICONINFORMATION);
			return false;
		}
	}
	else
	{
		MessageBox(hwnd, "Z wylosowanych liter nie mo�na utworzy� takiego s�owa.", "Ha!", MB_ICONINFORMATION);
		return false;
	}
}



void PominRuch(HWND hwnd)
{
	string strLiteryGracza = LiteryDoWyswietlenia(literyGracza);
	RozgrywkaInsert(*ptura, "gracz", ("( " + strLiteryGracza + ")"), 0);
	WymienLiteryGraczaMain();
	RuchKomputera(hwnd);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// WYPE�NIANIE STRUKTURY
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
	// WYPE�NIANIE STRUKTURY
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
	buttonOK = CreateWindowEx(0, "BUTTON", "Zatwierd�", WS_CHILD | WS_VISIBLE,
		10, 250, 340, 70, DialogWindow, NULL, hInstance, NULL);

	textBoxLiczbaTurPunktow = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		10, 60, 250, 30, DialogWindow, NULL, hInstance, NULL);

	staticTextTuryPunkty = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 10, 20, 150, 30, DialogWindow, NULL, hInstance, NULL);
	SetWindowText(staticTextTuryPunkty, "Gra do: liczba tur / liczba punkt�w:");

	radioTury = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Liczba tur",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		10, 100, 150, 30,
		DialogWindow, (HMENU)ID_RADIO_TURY,
		GetModuleHandle(NULL), 0);

	radioPunkty = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Liczba punkt�w",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		200, 100, 150, 30,
		DialogWindow, (HMENU)ID_RADIO_PUNKTY,
		GetModuleHandle(NULL), 0);

	radioPVsC = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Gracz vs Komputer",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		10, 160, 150, 30,
		DialogWindow, (HMENU)ID_RADIO_PVSC,
		GetModuleHandle(NULL), 0);

	radioPVsP = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Gracz vs Gracz",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		200, 160, 150, 30,
		DialogWindow, (HMENU)ID_RADIO_PVSP,
		GetModuleHandle(NULL), 0);

	radioCVsC = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Komputer vs Komputer",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		100, 200, 180, 30,
		DialogWindow, (HMENU)ID_RADIO_CVSC,
		GetModuleHandle(NULL), 0);


	////----------------End Elementy DW-----------------------/////////
	buttonWymienLiterki = CreateWindowEx(0, "BUTTON", "Wymie� literki", WS_CHILD | WS_VISIBLE,
		550, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g��wnego

	buttonZatwierdz = CreateWindowEx(0, "BUTTON", "Zatwierdz", WS_CHILD | WS_VISIBLE,
		350, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g��wnego

	buttonPomin = CreateWindowEx(0, "BUTTON", "Pomin ruch", WS_CHILD | WS_VISIBLE,
		750, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g��wnego

	textBoxWpisaneSlowo = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		50, 600, 250, 30, OknoAplikacji, NULL, hInstance, NULL);

	staticTextEtykietaTwojeLitery = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 50, 520, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	SetWindowText(staticTextEtykietaTwojeLitery, "Twoje literki:");

	staticTextTwojeLitery = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 50, 550, 250, 30, OknoAplikacji, NULL, hInstance, NULL);

	staticTextEtykietaPunktyGracza = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 750, 170, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	SetWindowText(staticTextEtykietaPunktyGracza, "Punkty Gracza:");

	staticTextPunktyGracza = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 750, 200, 150, 30, OknoAplikacji, (HMENU)ID_ETYKIETA_PUNKTY_GRACZA, hInstance, NULL);

	SetDlgItemInt(OknoAplikacji, ID_ETYKIETA_PUNKTY_GRACZA, punktyGracza, true); //Ustawieni pkt gracza na etykiecie

	staticTextEtykietaPunktyKomputera = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 750, 250, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	SetWindowText(staticTextEtykietaPunktyKomputera, "Punkty Komputera:");

	staticTextPunktyKomputera = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 750, 280, 150, 30, OknoAplikacji, (HMENU)ID_ETYKIETA_PUNKTY_KOMPUTERA, hInstance, NULL);

	SetDlgItemInt(OknoAplikacji, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyKomputera, true);	//Ustawieni pkt komputera na etykiecie

	listViewRozgrywka = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT |
		LVS_EDITLABELS, 50, 50, 600, 450,
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
	lvc.cx = 275;
	lvc.pszText = "S�owo";
	ListView_InsertColumn(listViewRozgrywka, 2, &lvc);

	lvc.iSubItem = 3;
	lvc.cx = 55;  // zmniejszone �eby si� nie pojawia� scroll poziomy
	lvc.pszText = "Punkty";
	ListView_InsertColumn(listViewRozgrywka, 3, &lvc);
#pragma endregion Kolumny w listView

#pragma endregion Inicjalizacja kontrolek okna

	slownik.ZaczytajSlownik();

	srand(time(NULL));
	while (literyKomputera.size() < 9)
		literyKomputera.push_back(LosujLitere());

	while (literyGracza.size() < 9)
		literyGracza.push_back(LosujLitere());

	string strLiteryGracza = LiteryDoWyswietlenia(literyGracza);

	LPCSTR lpcstr = strLiteryGracza.c_str();
	SetWindowText(staticTextTwojeLitery, lpcstr);


	//------------------------------------------------------------------------------------------------------------------------------
	ShowWindow(OknoAplikacji, nCmdShow); // Poka� okienko...
	UpdateWindow(OknoAplikacji);

	ShowWindow(DialogWindow, nCmdShow); // Poka� okienko DW
	UpdateWindow(DialogWindow);

	// P�tla komunikat�w - klikni�cie myszk�, wci�ni�cie klawisza czy zamkni�cie okna
	while (GetMessage(&Komunikat, NULL, 0, 0)) // Gdy robimy jak�� czynno��, kt�ra wi��e si� z powstaniem jakiej� wiadomo�ci, np. poruszamy mysz�, wiadomo�� posy�ana jest do kolejki wiadomo�ci, sk�d zabiera j� GetMessage. 
	{
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}
	return Komunikat.wParam;
}

// OBS�UGA ZDARZE� - funkcja zajmie si� w�a�ciw� obs�ug� komunikat�w systemu, czyli np. reakcj� na wciskanie r�nych klawiszy
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
	case WM_COMMAND://obs�uga naszego przycisku buttonWymienLiterki

		if ((HWND)lParam == buttonWymienLiterki) 
		{
			WymienLiteryGraczaMain();
		}

		if ((HWND)lParam == buttonZatwierdz)
		{
			MessageBox(hwnd, "Nacisn��e� przycisk Zatwierdz!", "Ha!", MB_ICONINFORMATION);
			if (RuchGracza(hwnd))
			{
				RuchKomputera(hwnd);
			}
	
		}
		if ((HWND)lParam == buttonPomin)
		{
			MessageBox(hwnd, "Nacisnales pomin.", "Ha!", MB_ICONINFORMATION);
			PominRuch(hwnd);
		}

		if ((HWND)lParam == buttonOK)
		{
			DWORD dlugosc = GetWindowTextLength(textBoxLiczbaTurPunktow);
			LPSTR Bufor = (LPSTR)GlobalAlloc(GPTR, dlugosc + 1);
			GetWindowText(textBoxLiczbaTurPunktow, Bufor, dlugosc + 1);

			string liczbaTurLubPunktow(Bufor);
			int lbTurLubPkt = atoi(liczbaTurLubPunktow.c_str());
			if (lbTurLubPkt > 0)
			{
				bool wybranoTuryLubPunkty = false;
				bool wybranoTrybGry = false;

				if (IsDlgButtonChecked(hwnd, ID_RADIO_PUNKTY) == BST_CHECKED)
				{
					MessageBox(NULL, "Zaznaczono na liczbe punktow", "Dziala !", MB_OK);
					graDoTury = -1;
					graDoLiczbyPunktow = lbTurLubPkt;
					wybranoTuryLubPunkty = true;
				}
				else if (IsDlgButtonChecked(hwnd, ID_RADIO_TURY) == BST_CHECKED)
				{
					MessageBox(NULL, "Zaznaczono na liczbe tur", "Dziala !", MB_OK);
					if (lbTurLubPkt % 2 == 1)
					{
						lbTurLubPkt += 1;
						MessageBox(NULL, "Dodano jedn� tur� aby liczba by�a parzysta", "Dodanie tury", MB_OK);
					}
					graDoLiczbyPunktow = -1;
					graDoTury = lbTurLubPkt;
					wybranoTuryLubPunkty = true;
				}
				if (IsDlgButtonChecked(hwnd, ID_RADIO_PVSC) == BST_CHECKED) 
				{
					trybGry = 1;
					wybranoTrybGry = true;
				}
				else if (IsDlgButtonChecked(hwnd, ID_RADIO_PVSP) == BST_CHECKED)
				{
					trybGry = 2;
					wybranoTrybGry = true;
				}
				else if (IsDlgButtonChecked(hwnd, ID_RADIO_CVSC) == BST_CHECKED)
				{
					trybGry = 3;
					wybranoTrybGry = true;
				}

				if (wybranoTrybGry && wybranoTuryLubPunkty)
					ShowWindow(hwnd, SW_HIDE);

				if (trybGry == 3)
				{
					while (true)
						RuchKomputera(hwnd);

				}
			}
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam); // inne odsy�amy dalej - do funkcji DefWindowProc, kt�ra zajmie si� ich domy�ln� obs�ug�.
	}

	return 0;
}