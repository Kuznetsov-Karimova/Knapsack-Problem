#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include "Knapsack.hpp"

auto Knapsack::algorithm(bool debug) -> size_t {

    std::vector<std::vector<int>> capacity_matrix;
    capacity_matrix.emplace_back(m_knapsack_capacity + 1 ,0);
    m_count_of_opers += m_knapsack_capacity + 1;

    for (size_t sub_number = 1; sub_number <= m_count_of_sub; ++sub_number) {

        ++m_count_of_opers;

        capacity_matrix.emplace_back(m_knapsack_capacity + 1 ,0);
        m_count_of_opers += m_knapsack_capacity + 1;

        for (size_t nap_weight = 0; nap_weight <= m_knapsack_capacity; ++nap_weight) {

            ++m_count_of_opers;

            if (static_cast<size_t>(m_weights[sub_number - 1]) <= nap_weight) {

                capacity_matrix[sub_number][nap_weight] = std::max(capacity_matrix[sub_number - 1][nap_weight], 
                capacity_matrix[sub_number - 1][nap_weight - m_weights[sub_number - 1]] + m_values[sub_number - 1]);

                m_count_of_opers += 2;

            } else {
                capacity_matrix[sub_number][nap_weight] = capacity_matrix[sub_number - 1][nap_weight];
                m_count_of_opers += 1;
            }

        }

        if (debug) {
            for (int elem: capacity_matrix[sub_number]) {
                std::cout << elem << " ";
            }
            std::cout << "\n";
        }
    }

    size_t row_pos = m_count_of_sub;
    size_t column_pos = m_knapsack_capacity;
    while (capacity_matrix[row_pos][column_pos] != 0) {

        ++m_count_of_opers;

        if (capacity_matrix[row_pos - 1][column_pos] == capacity_matrix[row_pos][column_pos]) {
            --row_pos;
        } else {

            m_res_subs[row_pos - 1] = 1;
            --row_pos;
            column_pos = column_pos - m_weights[row_pos];
        }

    }

    print_res();

    return static_cast<size_t>(capacity_matrix[m_count_of_sub][m_knapsack_capacity]);
}

auto main() -> int {

    std::vector<std::chrono::duration<double>> all_results(9, static_cast<std::chrono::duration<double>>(0));

    std::vector<std::chrono::duration<double>> one_run(9);


    Knapsack_app(one_run, true);

    for (int j = 0; j < 9; ++j) {
        all_results[j] += one_run[j];
    }

}