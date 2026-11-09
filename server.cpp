#include "server.h"
#include <cmath>
#include <stdexcept>

SolveResult solveQuadratic(double a, double b, double c) {
	SolveResult res;
	res.hasX1 = false;
	res.hasX2 = false;
	res.x1 = 0.0;
	res.x2 = 0.0;

	// Проверка: является ли уравнение квадратным
	if (a == 0.0) {
		res.success = false;
		res.message = "Ошибка: коэффициент 'a' не может быть равен нулю (это не квадратное уравнение).";
		return res;
	}

	try {
		double discriminant = b * b - 4 * a * c;

		if (discriminant > 0) {
			res.success = true;
			res.message = "Дискриминант > 0. Два действительных корня.";
			res.x1 = (-b + std::sqrt(discriminant)) / (2 * a);
			res.x2 = (-b - std::sqrt(discriminant)) / (2 * a);
			res.hasX1 = true;
			res.hasX2 = true;
		}
		else if (discriminant == 0) {
			res.success = true;
			res.message = "Дискриминант = 0. Один действительный корень (кратный).";
			res.x1 = -b / (2 * a);
			res.hasX1 = true;
		}
		else {
			res.success = true;
			res.message = "Дискриминант < 0. Действительных корней нет.";
		}
	}
	catch (const std::exception& e) {
		res.success = false;
		res.message = "Ошибка вычислений на сервере: " + std::string(e.what());
	}

	return res;
}