#include <iostream> 
#include <windows.h>

using namespace std;


///mo�emy sparsowa� lini� �cie�ki �eby doda� co� ciekawego np. jakie� cheaty - u�atwienia
//Wymiana literek... ile wymieni� i kt�re - chyba �e na pocz�tek wymiana wszystkich 
//Czytanie ze s�owanika
//Sprawdzanie czy podane s�owo istnieje w s�owniku 
//Zliczanie punkt�w
//?? mo�e na pocz�tku pytanie ile tur - max - do sko�czenia literek
//pass - gdy nie mamy nic (chocia� mo�e lepiej tylko wymie� literki)
// Jakie� komunikaty
// Licznik ile pozosta�o literek w "worku"



LPSTR NazwaKlasy = "Klasa Okienka";
MSG Komunikat;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND buttonWymienLiterki;
HWND buttonZatwierdz;

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
		MessageBox(NULL, "Okno odm�wi�o przyj�cia na �wiat!", "Ale kicha...", MB_ICONEXCLAMATION);
		return 1;
	}

#pragma region
	buttonWymienLiterki = CreateWindowEx(0, "BUTTON", "Wymie� literki", WS_CHILD | WS_VISIBLE,
		1000, 600, 150, 30, hwnd, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g��wnego

	buttonZatwierdz = CreateWindowEx(0, "BUTTON", "Zatwierdz", WS_CHILD | WS_VISIBLE,
		800, 600, 150, 30, hwnd, NULL, hInstance, NULL); //hwnd - nasz uchyt okna g��wnego

#pragma endregion Inicjalizacja przyciskow

	ShowWindow(hwnd, nCmdShow); // Poka� okienko...
	UpdateWindow(hwnd);

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
			MessageBox(hwnd, "Nacisn��e� przycisk Wymien literki!", "Ha!", MB_ICONINFORMATION);
		if ((HWND)lParam == buttonZatwierdz)
			MessageBox(hwnd, "Nacisn��e� przycisk Zatwierdz!", "Ha!", MB_ICONINFORMATION);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam); // inne odsy�amy dalej - do funkcji DefWindowProc, kt�ra zajmie si� ich domy�ln� obs�ug�.
	}

	return 0;
}