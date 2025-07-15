/**
 * @author Иван Пинчуков (a.k.a \@kuretaru)
 */

#include <iostream>
#include <algorithm>
#include <map>

using namespace std;
using u64 = unsigned long long;


/**
 * @brief Решает проблему Яна.
 * @details  Функция определяет количество свободных дорожек 
 *       для Яна по обе стороны от него. Ян - (K+1)
 *      человек, который занял дорожку.
 * Логика выбора следующая:
 * 1. Первые два человека (при K+1>=2) занимают самые дальние 
 *    дорожки, тем самым покрывая краевые ситуации с 1 и 
 *    N-дорожками. Это создает большой сегмент N-2 свободных
 *    дорожек
 * 2. Следующие люди всегда выбирают дорожки в центре бОльшего
 *    свободного сегмента, что создает бинарное заполнение.
 * Так как N и K могут достигать 10^18-1 в определенных случаях,
 * рациональнее использовать быструю симуляцию на основе 
 * инструментария встроенной библиотеки map. Карта counts 
 * является счетчиком и хранит {segment_length -> num_of_each_segment}
 * Мы всегда определяем размер самого большого свободного сегмента (L)
 * 
 */
void solve () {
	u64 N, K;
	cin >> N >> K;

if ((N>pow(10, 18)) || (N<1) || (K<1) || (K>N)) {
	cout << "Err!" << endl;
	return;
} 

if (K == 0) {
	std::cout << 0 << " " << N - 1 << std::endl;
	return;
}

// У Яна только один друг (внутреннего сегмента нет).
if (K==1){
	cout << 0 << " " << N-2 << endl;
    return;
}

  	map<u64, u64> counts;
  	counts[N - 2] = 1; // Начальное состояние: один сегмент
  	u64 splits_to_perform = K - 1;
  	u64 yan_segment_length = 0;

while (splits_to_perform > 0) {
  	auto it = counts.rbegin();
  	u64 largest_len = it->first;
  	u64 num_largest_segments = it->second;
	// Если оставшихся людей меньше, чем самых больших сегментов,
	// значит Ян гарантированно займет один из них.
if (num_largest_segments >= splits_to_perform) {
  	yan_segment_length = largest_len;
  	break;
}

  	counts.erase(largest_len);
  	u64 sub_len1 = (largest_len - 1) / 2;
	u64 sub_len2 = largest_len / 2;

  	//Новые сегменты в map
  	counts[sub_len1] += num_largest_segments;
  	counts[sub_len2] += num_largest_segments;
  	splits_to_perform -= num_largest_segments;
}

  	u64 free_lanes1 = (yan_segment_length - 1) / 2;
  	u64 free_lanes2 = yan_segment_length / 2;

  	cout << min(free_lanes1, free_lanes2) << " " 
       	 << max(free_lanes1, free_lanes2) << endl;
}

/**
 * @brief Главная функция для запуска решения проблемы.
 * @details Настраивает быстрый IO и вызывает solve().
 * @return [0] при успешной работе.
 */
int main() {
  	// Ускорение стандартных потоков I/O
  	ios_base::sync_with_stdio(false);
  	cin.tie(NULL);

	solve();
  	return 0;
}