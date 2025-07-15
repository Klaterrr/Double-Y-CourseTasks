import sys
import csv
from collections import defaultdict

def solve():
    """
    Финальная версия решения.
    1. Рассчитывает p_naf (вероятность ошибки 'не цветок') для каждой группы.
    2. Рассчитывает p_correct_adj (скорректированную точность на реальных цветах).
    3. Вычисляет итоговое качество как взвешенное среднее p_correct_adj.
    """
    quality_stats = defaultdict(lambda: {'total': 0, 'correct': 0, 'is_not_a_flower': 0})
    try:
        with open('detection_quality.csv', 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            next(reader)
            for detected_name, human_name in reader:
                stats = quality_stats[detected_name]
                stats['total'] += 1
                if detected_name == human_name:
                    stats['correct'] += 1
                if human_name == 'not_a_flower':
                    stats['is_not_a_flower'] += 1
    except FileNotFoundError:
        print("Error: detection_quality.csv not found.", file=sys.stderr)
        return

    popular_flowers = {name for name, stats in quality_stats.items() if name != 'not_a_flower' and stats['correct'] >= 10}
    other_flowers = {name for name in quality_stats if name != 'not_a_flower' and name not in popular_flowers}

    # p_naf (вероятность, что на фото не было цветка)
    p_naf_other = sum(quality_stats[n]['is_not_a_flower'] for n in other_flowers) / sum(quality_stats[n]['total'] for n in other_flowers) if other_flowers else 0
    p_naf_popular = sum(quality_stats[n]['is_not_a_flower'] for n in popular_flowers) / sum(quality_stats[n]['total'] for n in popular_flowers) if popular_flowers else 0
    p_naf_naf = quality_stats['not_a_flower']['correct'] / quality_stats['not_a_flower']['total'] if quality_stats['not_a_flower']['total'] > 0 else 0

    # p_correct_adj (точность на подвыборке реальных цветов)
    def get_adj_correct_prob(stats):
        true_flower_detections = stats['total'] - stats['is_not_a_flower']
        return stats['correct'] / true_flower_detections if true_flower_detections > 0 else 0

    p_correct_adj_popular = {name: get_adj_correct_prob(quality_stats[name]) for name in popular_flowers}

    other_correct_sum = sum(quality_stats[n]['correct'] for n in other_flowers)
    other_true_flower_sum = sum(quality_stats[n]['total'] - quality_stats[n]['is_not_a_flower'] for n in other_flowers)
    p_correct_adj_other = other_correct_sum / other_true_flower_sum if other_true_flower_sum > 0 else 0
    
    # для группы not_a_flower, p_correct_adj не имеет смысла, т.к. там нет "верно распознанных цветков".
    # числитель для этой группы всегда будет 0.
    
    numerator = 0.0
    denominator = 0.0

    try:
        with open('detection_count.csv', 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            next(reader)
            for detected_name, count_str in reader:
                monthly_count = int(count_str)
                p_naf = 0
                p_correct_adj = 0

                if detected_name == 'not_a_flower':
                    p_naf = p_naf_naf
                    p_correct_adj = 0 # Верно распознанных ЦВЕТКОВ здесь 0
                elif detected_name in popular_flowers:
                    p_naf = p_naf_popular
                    p_correct_adj = p_correct_adj_popular[detected_name]
                else: # Группа 'other' или цветок, отсутствующий в quality.csv
                    p_naf = p_naf_other
                    p_correct_adj = p_correct_adj_other
                
                weight = monthly_count * (1 - p_naf)
                
                denominator += weight
                numerator += weight * p_correct_adj

    except FileNotFoundError:
        print("Error: detection_count.csv not found.", file=sys.stderr)
        return

    final_quality = numerator / denominator if denominator > 0 else 0.0
    print(f"{final_quality:.4f}")

solve()