#include <string>
#include <vector>
#include <chrono>
#include<algorithm>
#include "Knapsack.hpp"

void nampack_app(std::vector<std::chrono::duration<double>>& all_results, bool isPrint, bool debug = false);

size_t Nampack::algorithm(bool debug) {

    std::vector<std::vector<int>> capacity_matrix;
    capacity_matrix.push_back(std::vector<int>(m_nap_size + 1 ,0));
    m_count_of_opers += m_nap_size + 1;

    for (size_t sub_number = 1; sub_number <= m_count_of_sub; ++sub_number) {

        ++m_count_of_opers;

        capacity_matrix.push_back(std::vector<int>(m_nap_size + 1 ,0));
        m_count_of_opers += m_nap_size + 1;

        for (size_t nap_weight = 0; nap_weight <= m_nap_size; ++nap_weight) {

            ++m_count_of_opers;

            if (static_cast<size_t>(m_sub_sizes[sub_number - 1]) <= nap_weight) {

                capacity_matrix[sub_number][nap_weight] = std::max(capacity_matrix[sub_number - 1][nap_weight], 
                capacity_matrix[sub_number - 1][nap_weight - m_sub_sizes[sub_number - 1]] + m_sub_values[sub_number - 1]);

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
    size_t column_pos = m_nap_size;
    while (capacity_matrix[row_pos][column_pos] != 0) {

        ++m_count_of_opers;

        if (capacity_matrix[row_pos - 1][column_pos] == capacity_matrix[row_pos][column_pos]) {
            --row_pos;
        } else {

            m_res_subs[row_pos - 1] = 1;
            --row_pos;
            column_pos = column_pos - m_sub_sizes[row_pos];
        }

    }

    print_res();

    return static_cast<size_t>(capacity_matrix[m_count_of_sub][m_nap_size]);
}

auto main() -> int {

    std::vector<std::chrono::duration<double>> all_results(9, static_cast<std::chrono::duration<double>>(0));

    std::vector<std::chrono::duration<double>> one_run(9);


    nampack_app(one_run, true);

    for (int j = 0; j < 9; ++j) {
        all_results[j] += one_run[j];
    }

}

void nampack_app(std::vector<std::chrono::duration<double>>& all_results, bool isPrint, bool debug) {


    // ALL TESTS
    std::chrono::duration<double> all_elapsed{};

    int num = 0;

    for (auto test: test_files) {
        if (isPrint) {
            std::cout << "Test: " << num << std::endl;
        }

        Nampack pack(test[0], test[1], test[2], test[3]);
        
        std::cout << "Capacity: " << pack.get_nap_size() << std::endl;
        std::cout << "Prices: ";
        pack.print_values();
        std::cout << "Weights: ";
        pack.print_weights();

        // ALGHORITM 1 TEST TIME START
        auto start = std::chrono::high_resolution_clock::now();

        auto result = pack.algorithm(debug);

        auto end = std::chrono::high_resolution_clock::now();

        // ALGHORITM 1 TEST TIME END
        std::chrono::duration<double> elapsed = end - start;

        all_elapsed += elapsed;

        if (isPrint) {
            std::cout << "Result: " << result << std::endl;
            std::cout << "Time: " << elapsed.count() << " s" << std::endl;
            std::cout << "Count of opers: " << pack.get_count_of_opers() << std::endl;

            std::cout << std::endl;
        }

        all_results[num] = elapsed;

        ++num;
    }

    auto average_time = all_elapsed/(test_files.size());

    if (isPrint) {
        std::cout << "Average time: " << average_time.count() << std::endl;
    }

    all_results[num] = average_time;
}