#include <windows.h>
#include <string>
#include <sstream>
#include "server.h"

// Идентификаторы элементов управления
#define ID_EDIT_A 101
#define ID_EDIT_B 102
#define ID_EDIT_C 103
#define ID_BTN_SOLVE 104
#define ID_STATIC_RES 105

HWND hEditA, hEditB, hEditC, hStaticRes;

// Вспомогательная функция для получения текста из Edit
double getDoubleFromEdit(HWND hEdit, bool& isValid) {
	char buffer[256];
	GetWindowTextA(hEdit, buffer, 256);
	std::string str(buffer);
	isValid = false;

	if (str.empty()) return 0.0;

	try {
		size_t pos;
		double val = std::stod(str, &pos);
		// Проверяем, что вся строка была числом (защита от "12abc")
		if (pos == str.length()) {
			isValid = true;
			return val;
		}
	}
	catch (...) {
		isValid = false;
	}
	return 0.0;
}

// Обработчик нажатия кнопки "Решить"
void OnSolve() {
	bool validA, validB, validC;

	// 1. Клиент собирает данные с формы
	double a = getDoubleFromEdit(hEditA, validA);
	double b = getDoubleFromEdit(hEditB, validB);
	double c = getDoubleFromEdit(hEditC, validC);

	// 2. Клиент проверяет корректность ввода
	if (!validA || !validB || !validC) {
		MessageBoxA(NULL, "Ошибка ввода! Введите корректные числовые значения.", "Ошибка", MB_ICONERROR);
		return;
	}

	// 3. Клиент передает данные серверу
	SolveResult result = solveQuadratic(a, b, c);

	// 4. Клиент отображает результат, полученный от сервера
	std::stringstream ss;
	ss << result.message << "\r\n\r\n";

	if (result.hasX1) {
		ss << "x1 = " << result.x1 << "\r\n";
	}
	if (result.hasX2) {
		ss << "x2 = " << result.x2 << "\r\n";
	}

	if (!result.success) {
		MessageBoxA(NULL, result.message.c_str(), "Ошибка сервера", MB_ICONWARNING);
	}
	else {
		SetWindowTextA(hStaticRes, ss.str().c_str());
	}
}

// Главная функция окна (обработка событий)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE: {
		// Создаем GUI элементы
		CreateWindowA("STATIC", "Введите коэффициенты a, b, c:", WS_VISIBLE | WS_CHILD, 20, 20, 300, 20, hwnd, NULL, NULL, NULL);

		CreateWindowA("STATIC", "a =", WS_VISIBLE | WS_CHILD, 20, 50, 30, 20, hwnd, NULL, NULL, NULL);
		hEditA = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 60, 50, 100, 20, hwnd, (HMENU)ID_EDIT_A, NULL, NULL);

		CreateWindowA("STATIC", "b =", WS_VISIBLE | WS_CHILD, 20, 80, 30, 20, hwnd, NULL, NULL, NULL);
		hEditB = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 60, 80, 100, 20, hwnd, (HMENU)ID_EDIT_B, NULL, NULL);

		CreateWindowA("STATIC", "c =", WS_VISIBLE | WS_CHILD, 20, 110, 30, 20, hwnd, NULL, NULL, NULL);
		hEditC = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 60, 110, 100, 20, hwnd, (HMENU)ID_EDIT_C, NULL, NULL);

		CreateWindowA("BUTTON", "Решить уравнение", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 180, 50, 150, 30, hwnd, (HMENU)ID_BTN_SOLVE, NULL, NULL);

		hStaticRes = CreateWindowA("STATIC", "Результат появится здесь...", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_BORDER, 20, 160, 310, 100, hwnd, (HMENU)ID_STATIC_RES, NULL, NULL);

		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == ID_BTN_SOLVE) {
			OnSolve();
		}
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProcA(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	const char CLASS_NAME[] = "QuadraticSolverClass";

	WNDCLASSA wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassA(&wc);

	// Размеры окна: 370x330
	HWND hwnd = CreateWindowExA(
		0, CLASS_NAME, "Клиент-Сервер: Решение квадратного уравнения",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, // Фиксированный размер
		CW_USEDEFAULT, CW_USEDEFAULT, 370, 330,
		NULL, NULL, hInstance, NULL
	);

	if (hwnd == NULL) return 0;

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}