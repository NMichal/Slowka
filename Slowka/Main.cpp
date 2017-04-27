#include <iostream> 
#include <windows.h>

using namespace std;


///mo¿emy sparsowaæ liniê œcie¿ki ¿eby dodaæ coœ ciekawego np. jakieœ cheaty - u³atwienia
//Wymiana literek... ile wymieniæ i które - chyba ¿e na pocz¹tek wymiana wszystkich 
//Czytanie ze s³owanika
//Sprawdzanie czy podane s³owo istnieje w s³owniku 
//Zliczanie punktów
//?? mo¿e na pocz¹tku pytanie ile tur - max - do skoñczenia literek
//pass - gdy nie mamy nic (chocia¿ mo¿e lepiej tylko wymieñ literki)
// Jakieœ komunikaty
// Licznik ile pozosta³o literek w "worku"



LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND buttonWymienLiterki;
HWND buttonZatwierdz;

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
	HWND hwnd;

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, "Oto okienko", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Okno odmówi³o przyjœcia na œwiat!", "Ale kicha...", MB_ICONEXCLAMATION);
		return 1;
	}

#pragma region
	buttonWymienLiterki = CreateWindowEx(0, "BUTTON", "Wymieñ literki", WS_CHILD | WS_VISIBLE,
		1000, 600, 150, 30, hwnd, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

	buttonZatwierdz = CreateWindowEx(0, "BUTTON", "Zatwierdz", WS_CHILD | WS_VISIBLE,
		800, 600, 150, 30, hwnd, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g³ównego

#pragma endregion Inicjalizacja przyciskow

	ShowWindow(hwnd, nCmdShow); // Poka¿ okienko...
	UpdateWindow(hwnd);

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