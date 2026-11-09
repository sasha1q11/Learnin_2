#ifndef SERVER_H
#define SERVER_H

#include <string>

// Структура для передачи результата от сервера клиенту
struct SolveResult {
	bool success;       // Успешно ли прошли вычисления
	std::string message;// Текстовое сообщение (ошибка или пояснение)
	double x1;          // Первый корень
	double x2;          // Второй корень
	bool hasX1;
	bool hasX2;
};

// Функция-сервер для решения уравнения
SolveResult solveQuadratic(double a, double b, double c);

#endif // SERVER_H