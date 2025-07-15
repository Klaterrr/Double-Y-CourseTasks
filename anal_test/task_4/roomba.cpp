/**
 * @author Иван Пинчуков (a.k.a @kuretaru)
 */

#include <iostream>
#include <vector>
#include <string>

using namespace std;
// Псевдоним для 64-битного целого числа, чтобы избежать переполнения
using ll = long long;

/**
 * @brief Решает задачу "Робот-пылесос".
 * @details Функция вычисляет минимальное время, которое необходимо для очистки
 *			всех свободных клеток в квартире.
 *          Задача не требует сложного поиска пути. Так как все свободные клетки 
 *			связаны - минимальное количество перемещений для посещения всех K 
 *			свободных клеток всегда равно K-1.
 *          Алгоритм:
 *          1. Подсчитать общее количество свободных клеток (K).
 *          2. Время на очистку фиксировано: K * y.
 *          3. Минимальное время на перемещение также фиксировано: (K - 1) * x.
 *          4. Итоговое минимальное время = (K - 1) * x + K * y.
 *          5. Обрабатывается случай, когда свободных клеток нет (K = 0).
 */
void solve() {
	ll N, M, x, y;
	cin >> N >> M >> x >> y;

	ll empty_cell_count = 0;
	for (int i = 0; i < M; ++i) { // M - высота, N - ширина
		string row;
		cin >> row;
		for (char cell : row) {
			if (cell == '0') {
				empty_cell_count++;
			}
		}
	}

	ll total_time = 0;
	if (empty_cell_count > 0) {
		ll move_time = (empty_cell_count - 1) * x;
		ll clean_time = empty_cell_count * y;
		total_time = move_time + clean_time;
	}
    
	// Если empty_cell_count == 0, total_time останется 0.
	cout << total_time << std::endl;
}

/**
 * @brief Главная функция для запуска решения.
 * @details Настраивает быстрый ввод-вывод и вызывает основную решающую функцию.
 * @return 0 при успешном выполнении.
 */
int main() {
	// Ускорение IO для повышения производительности
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	solve();

	return 0;
}