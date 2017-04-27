#include <iostream> 
#include <windows.h>
#include <string> 

using namespace std;

#define ID_ETYKIETA_LICZBA_LITEREK 10



///mo¿emy sparsowaæ liniê œcie¿ki ¿eby dodaæ coœ ciekawego np. jakieœ cheaty - u³atwienia
//Wymiana literek... ile wymieniæ i które - chyba ¿e na pocz¹tek wymiana wszystkich 
//Czytanie ze s³owanika
//Sprawdzanie czy podane s³owo istnieje w s³owniku 
//Zliczanie punktów
//?? mo¿e na pocz¹tku pytanie ile tur - max - do skoñczenia literek
//pass - gdy nie mamy nic (chocia¿ mo¿e lepiej tylko wymieñ literki)
// Jakieœ komunikaty
// Licznik ile pozosta³o literek w "worku"
// lowercase - przy pobieraniu z textboxa (sprawdziæ czy wszystko w s³owniku bêdzie ma³e)

// byæ mo¿e póŸniej dodaæ ID_PRZYCISKOW 



LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma region
HWND buttonWymienLiterki;
HWND buttonZatwierdz;
HWND textBoxWpisaneSlowo;
HWND staticTextEtykietaLiczbaLiterek;
HWND staticTextLiczbaLiterek;
#pragma endregion Deklaracja kontrolek okna

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
		MessageBox(NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
			MB_ICONEXCLAMATION | MB_OK);
		return 1;
	}

	// TWORZENIE OKNA
	HWND OknoAplikacji;

	OknoAplikacji = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, "Oto okienko", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 750, 700, NULL, NULL, hInstance, NULL);

	if (OknoAplikacji == NULL)
	{
		MessageBox(NULL, "Okno odmówi³o przyjœcia na œwiat!", "Ale kicha...", MB_ICONEXCLAMATION);
		return 1;
	}

#pragma region
	buttonWymienLiterki = CreateWindowEx(0, "BUTTON", "Wymieñ literki", WS_CHILD | WS_VISIBLE,
		550, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

	buttonZatwierdz = CreateWindowEx(0, "BUTTON", "Zatwierdz", WS_CHILD | WS_VISIBLE,
		350, 600, 150, 30, OknoAplikacji, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

	textBoxWpisaneSlowo = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		50, 600, 250, 30, OknoAplikacji, NULL, hInstance, NULL);

	staticTextEtykietaLiczbaLiterek = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 600, 50, 150, 30, OknoAplikacji, NULL, hInstance, NULL);
	SetWindowText(staticTextEtykietaLiczbaLiterek, "Liczba pozosta³ych \n\tliterek:");

	staticTextLiczbaLiterek = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 600, 80, 150, 30, OknoAplikacji, (HMENU)ID_ETYKIETA_LICZBA_LITEREK, hInstance, NULL);

	int i = 10;
	SetDlgItemInt(OknoAplikacji, ID_ETYKIETA_LICZBA_LITEREK, i, true);	//---!!!---Ustwaienie etyliety liczby literek z inta ---!!!---

#pragma endregion Inicjalizacja kontrolek okna

	ShowWindow(OknoAplikacji, nCmdShow); // Poka¿ okienko...
	UpdateWindow(OknoAplikacji);

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
		if ((HWND)lParam == buttonWymienLiterki)
			MessageBox(hwnd, "Nacisn¹³eœ przycisk Wymien literki!", "Ha!", MB_ICONINFORMATION);
		if ((HWND)lParam == buttonZatwierdz)
			MessageBox(hwnd, "Nacisn¹³eœ przycisk Zatwierdz!", "Ha!", MB_ICONINFORMATION);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam); // inne odsy³amy dalej - do funkcji DefWindowProc, która zajmie siê ich domyœln¹ obs³ug¹.
	}

	return 0;
}