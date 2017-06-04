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


list<char> literyKomputera;
list<char> literyGracza;
int punktyKomputera = 0;
int punktyGracza = 0;
int tura = 1;
int *ptura = &tura;
int graDoTury = -1;
int graDoLiczbyPunktow = -1;
int trybGry = -1;
int iloscWymianLiter = 3;



Slownik slownik;
Gra gra;
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
RECT rcl; // list view bo nale¿y co grupy Common Controls

//elementy Dialog Window
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


void WymienLiteryGracza()
{
	literyGracza.clear();

	while (literyGracza.size() < 9)
		literyGracza.push_back(Gra::LosujLitere());

	string strLiteryGracza = Gra::LiteryDoWyswietlenia(literyGracza);

	LPCSTR lpcstr = strLiteryGracza.c_str();
	SetWindowText(staticTextTwojeLitery, lpcstr);
}

void RuchKomputera(HWND hwnd)
{
	literyKomputera.clear();

	while (literyKomputera.size() < 9)
		literyKomputera.push_back(Gra::LosujLitere());

	string literyKomputeraDoWyswietlenia = Gra::LiteryDoWyswietlenia(literyKomputera);
	string strLiteryKomputera = "";
	for (auto v : literyKomputera)
	{
		strLiteryKomputera += v;
 	}

	if(trybGry != 3)
		MessageBox(hwnd, "Teraz Komputer ", "Ruch komputera.", MB_ICONINFORMATION);

	string ulozoneSlowo = slownik.KomputerUkladaSlowo(strLiteryKomputera);
	int punkty = Gra::PunktujSlowo(ulozoneSlowo);
	ulozoneSlowo += " ( " + literyKomputeraDoWyswietlenia + ")";
	RozgrywkaInsert(*ptura, "Komputer", ulozoneSlowo, punkty);
	punktyKomputera += punkty;
	SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyKomputera, true);
	if (graDoLiczbyPunktow != -1 && punktyKomputera >= graDoLiczbyPunktow) 
	{
		MessageBox(hwnd, "Wygral Komputer ", "Koniec gry.", MB_ICONINFORMATION);
		ExitProcess(1);
	}
	if (graDoTury != -1 && *ptura > graDoTury)
	{
		MessageBox(hwnd, "Koniec gry.", "Koniec gry.", MB_ICONINFORMATION);
		ExitProcess(1);
	}
}


void RuchKomputeraBruteForce(HWND hwnd)
{
	literyGracza.clear();

	while (literyGracza.size() < 9)
		literyGracza.push_back(Gra::LosujLitere());

	string literyKomputeraDoWyswietlenia = Gra::LiteryDoWyswietlenia(literyGracza);
	string strLiteryKomputera = "";
	for (auto v : literyGracza)
	{
		strLiteryKomputera += v;
	}

	string ulozoneSlowo = slownik.KomputerUkladaSlowoBruteForce(strLiteryKomputera);
	int punkty = Gra::PunktujSlowo(ulozoneSlowo);
	ulozoneSlowo += " ( " + literyKomputeraDoWyswietlenia + ")";
	RozgrywkaInsert(*ptura, "Komputer BF", ulozoneSlowo, punkty);
	punktyGracza += punkty;
	SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_GRACZA, punktyGracza, true);
	ShowWindow(staticTextPunktyGracza, SW_HIDE);
	ShowWindow(staticTextPunktyGracza, SW_SHOW);
	if (graDoLiczbyPunktow != -1 && punktyGracza >= graDoLiczbyPunktow)
	{
		MessageBox(hwnd, "Wygral Komputer Brute Force", "Koniec gry.", MB_ICONINFORMATION);
		ExitProcess(1);
	}
	if (graDoTury != -1 && *ptura > graDoTury)
	{
		MessageBox(hwnd, "Koniec gry.", "Koniec gry.", MB_ICONINFORMATION);
		ExitProcess(1);
	}
}

bool RuchGracza(HWND hwnd, int nrGracza)
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

	if (Gra::CzyMoznaUtworzycSlowo(wpisaneSlowo, strLiteryGracza))
	{
		bool istniejeSlowo = slownik.SprawdzSlowo(wpisaneSlowo);
		if (istniejeSlowo)
		{
			MessageBox(hwnd, "Istnieje takie s³owo!", "Komunikat", MB_ICONINFORMATION);
			int punkty = Gra::PunktujSlowo(wpisaneSlowo);
			string literyDoWysietlenia = Gra::LiteryDoWyswietlenia(literyGracza);
			wpisaneSlowo += " ( " + literyDoWysietlenia + ")";
			if(nrGracza == 1)	
				RozgrywkaInsert(*ptura, "Gracz 1", wpisaneSlowo, punkty);
			else if(nrGracza == 2)
				RozgrywkaInsert(*ptura, "Gracz 2", wpisaneSlowo, punkty);
			else
				RozgrywkaInsert(*ptura, "Gracz", wpisaneSlowo, punkty);

			if (nrGracza == 2)
				punktyKomputera += punkty;
			else
				punktyGracza += punkty;

			if (nrGracza == 2)
				SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyKomputera, true);
			else
				SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_GRACZA, punktyGracza, true);

			if (graDoLiczbyPunktow != -1 && punktyGracza >= graDoLiczbyPunktow && nrGracza == 1)
			{
				MessageBox(hwnd, "Wygral Gracz 1", "Koniec gry.", MB_ICONINFORMATION);
				ExitProcess(1);
			}
			else if (graDoLiczbyPunktow != -1 && punktyKomputera >= graDoLiczbyPunktow && nrGracza == 2)
			{
				MessageBox(hwnd, "Wygral Gracz 2", "Koniec gry.", MB_ICONINFORMATION);
				ExitProcess(1);
			}
			else if (graDoLiczbyPunktow != -1 && punktyGracza >= graDoLiczbyPunktow && nrGracza == 0)
			{
				MessageBox(hwnd, "Wygral Gracz ", "Koniec gry.", MB_ICONINFORMATION);
				ExitProcess(1);
			}

			if (graDoTury != -1 && *ptura > graDoTury)
			{
				MessageBox(hwnd, "Koniec gry.", "Koniec gry.", MB_ICONINFORMATION);
				ExitProcess(1);
			} 
			SetWindowText(textBoxWpisaneSlowo, (""));
			WymienLiteryGracza();
			iloscWymianLiter = 3;
			return true;
		}
		else
		{
			MessageBox(hwnd, "Nie istnieje takie s³owo.", "Komunikat", MB_ICONINFORMATION);
			return false;
		}
	}
	else
	{
		MessageBox(hwnd, "Z wylosowanych liter nie mo¿na utworzyæ takiego s³owa.", "Komunikat", MB_ICONINFORMATION);
		return false;
	}
}



void PominRuch(HWND hwnd)
{
	string strLiteryGracza = Gra::LiteryDoWyswietlenia(literyGracza);
	if (trybGry == 1)
		RozgrywkaInsert(*ptura, "Gracz", ("( " + strLiteryGracza + ")"), 0);
	else 
	{
		int graczNr;
		if (*ptura % 2 == 1)
			RozgrywkaInsert(*ptura, "Gracz 1", ("( " + strLiteryGracza + ")"), 0);
		else
			RozgrywkaInsert(*ptura, "Gracz 2", ("( " + strLiteryGracza + ")"), 0);
	}
	iloscWymianLiter = 3; 
	WymienLiteryGracza();
	if(trybGry == 1)
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


	// DIALOG WINDOW
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
		10, 250, 340, 70, DialogWindow, NULL, hInstance, NULL);

	textBoxLiczbaTurPunktow = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		10, 60, 250, 30, DialogWindow, NULL, hInstance, NULL);

	staticTextTuryPunkty = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 10, 20, 150, 30, DialogWindow, NULL, hInstance, NULL);
	SetWindowText(staticTextTuryPunkty, "Gra do: liczba tur / liczba punktów:");

	radioTury = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Liczba tur",
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		10, 100, 150, 30,
		DialogWindow, (HMENU)ID_RADIO_TURY,
		GetModuleHandle(NULL), 0);

	radioPunkty = CreateWindowExW(WS_EX_TRANSPARENT, L"BUTTON", L"Liczba punktów",
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

	buttonWymienLiterki = CreateWindowEx(0, "BUTTON", "Wymieñ literki", WS_CHILD | WS_VISIBLE,
		550, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); 

	buttonZatwierdz = CreateWindowEx(0, "BUTTON", "Zatwierdz", WS_CHILD | WS_VISIBLE,
		350, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); 

	buttonPomin = CreateWindowEx(0, "BUTTON", "Pomin ruch", WS_CHILD | WS_VISIBLE,
		750, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); 

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
	lvc.pszText = "S³owo";
	ListView_InsertColumn(listViewRozgrywka, 2, &lvc);

	lvc.iSubItem = 3;
	lvc.cx = 55;  
	lvc.pszText = "Punkty";
	ListView_InsertColumn(listViewRozgrywka, 3, &lvc);
#pragma endregion Kolumny w listView

#pragma endregion Inicjalizacja kontrolek okna

	slownik.ZaczytajSlownik();

	srand(time(NULL));
	while (literyKomputera.size() < 9)
		literyKomputera.push_back(Gra::LosujLitere());

	while (literyGracza.size() < 9)
		literyGracza.push_back(Gra::LosujLitere());

	string strLiteryGracza = Gra::LiteryDoWyswietlenia(literyGracza);

	LPCSTR lpcstr = strLiteryGracza.c_str();
	SetWindowText(staticTextTwojeLitery, lpcstr);


	ShowWindow(OknoAplikacji, nCmdShow); 
	UpdateWindow(OknoAplikacji);

	ShowWindow(DialogWindow, nCmdShow); 
	UpdateWindow(DialogWindow);

	while (GetMessage(&Komunikat, NULL, 0, 0))
	{
		TranslateMessage(&Komunikat);
		DispatchMessage(&Komunikat);
	}
	return Komunikat.wParam;
}

// OBS£UGA ZDARZEÑ
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
	case WM_COMMAND:

		if ((HWND)lParam == buttonWymienLiterki) 
		{
			if (iloscWymianLiter > 0)
			{
				WymienLiteryGracza();
				iloscWymianLiter--;
			}
			else
			{
				MessageBox(hwnd, "Nie mo¿na ju¿ wymieniæ liter.", "Wymiana liter.", MB_ICONINFORMATION); \
			}
		}

		if ((HWND)lParam == buttonZatwierdz)
		{
			switch (trybGry)
			{
			case 1:
				if (RuchGracza(hwnd, 0))
				{
					RuchKomputera(hwnd);
				}
				break;
			case 2:
				int graczNr;
				if (*ptura % 2 == 1)
  					graczNr = 1;
				else
					graczNr = 2;
				RuchGracza(hwnd, graczNr);
				break;

			}
		}
		if ((HWND)lParam == buttonPomin)
		{
				SetWindowText(textBoxWpisaneSlowo, (""));
				PominRuch(hwnd);
				if (graDoTury != -1 && *ptura > graDoTury)
				{
					MessageBox(hwnd, "Koniec gry.", "Koniec gry.", MB_ICONINFORMATION);
					ExitProcess(1);
				}
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
					graDoTury = -1;
					graDoLiczbyPunktow = lbTurLubPkt;
					wybranoTuryLubPunkty = true;
				}
				else if (IsDlgButtonChecked(hwnd, ID_RADIO_TURY) == BST_CHECKED)
				{
					if (lbTurLubPkt % 2 == 1)
					{
						lbTurLubPkt += 1;
						MessageBox(NULL, "Dodano jedn¹ turê aby liczba by³a parzysta", "Dodanie tury", MB_OK);
					}
					graDoLiczbyPunktow = -1;
					graDoTury = lbTurLubPkt;
					wybranoTuryLubPunkty = true;
				}
				if (IsDlgButtonChecked(hwnd, ID_RADIO_PVSC) == BST_CHECKED) 
				{
					trybGry = 1;
					wybranoTrybGry = true;
					MessageBox(hwnd, "Rozpoczyna gracz.", "Komunikat", MB_ICONINFORMATION);
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
				{
					ShowWindow(hwnd, SW_HIDE);

					if (trybGry == 3)
					{
						SetWindowText(staticTextEtykietaPunktyGracza, "Punkty Komputera BF:");
						ShowWindow(buttonPomin, SW_HIDE);
						ShowWindow(buttonWymienLiterki, SW_HIDE);
						ShowWindow(buttonZatwierdz, SW_HIDE);
						ShowWindow(textBoxWpisaneSlowo, SW_HIDE);
						ShowWindow(staticTextEtykietaTwojeLitery, SW_HIDE);
						ShowWindow(staticTextTwojeLitery, SW_HIDE);
						while (true)
						{
							RuchKomputera(hwnd);							
							SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyKomputera, true);
							MessageBox(hwnd, "Komputer u³o¿y³ s³owo.", "U³o¿ono s³owo", MB_ICONINFORMATION);
							RuchKomputeraBruteForce(hwnd); 							
							SetDlgItemInt(hwnd, ID_ETYKIETA_PUNKTY_KOMPUTERA, punktyGracza, true);
							MessageBox(hwnd, "Komputer Brute Force u³o¿y³ s³owo.", "U³o¿ono s³owo", MB_ICONINFORMATION);									
						}
					}
					else if (trybGry == 2) 
					{
						SetWindowText(staticTextEtykietaPunktyGracza, "Punkty Gracza 1:");
						SetWindowText(staticTextEtykietaPunktyKomputera, "Punkty Gracza 2:");
					}
				}
			}
		}
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam); 
	}
	return 0;
}