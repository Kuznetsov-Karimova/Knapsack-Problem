#include <string>
#include <vector>
#include <chrono>
#include<algorithm>
#include <algorithm>
#include "Knapsack.hpp"

void nampack_app(std::vector<std::chrono::duration<double>>& all_results, bool isPrint, bool debug = false);

size_t Nampack::algorithm(bool debug) {

    size_t max_greed_res = 0, q_greed_res = 0;

    std::vector<size_t> max_greed_indexes, q_greed_indexes;

    std::vector<int> max_greed_res_subjects(m_count_of_sub, 0);
    std::vector<int> q_greed_res_subjects(m_count_of_sub, 0);

    m_count_of_opers += m_count_of_sub * 2;

    for (size_t sub_number = 0; sub_number < m_count_of_sub; ++sub_number) {

        max_greed_indexes.push_back(sub_number);
        q_greed_indexes.push_back(sub_number);

        m_count_of_opers += 2;

    }

    std::sort(max_greed_indexes.begin(), max_greed_indexes.end(),
    [&](const int a, const int b) {
        ++m_count_of_opers;
        return static_cast<double>(m_sub_values[a]) > static_cast<double>(m_sub_values[b]);
    });

    std::sort(q_greed_indexes.begin(), q_greed_indexes.end(),
    [&](const int a, const int b) {
        ++m_count_of_opers;
        return static_cast<double>(m_sub_values[a])/static_cast<double>(m_sub_sizes[a]) >
            static_cast<double>(m_sub_values[b])/static_cast<double>(m_sub_sizes[b]);
    });

    size_t remainder_of_cap = m_nap_size;

    for(int index : max_greed_indexes) {

        ++m_count_of_opers;
        if (static_cast<size_t>(m_sub_sizes[index]) <= remainder_of_cap) {
            max_greed_res += m_sub_values[index];
            max_greed_res_subjects[index] = 1;
            remainder_of_cap -= m_sub_sizes[index];

            m_count_of_opers += 3;
        }

    }

    remainder_of_cap = m_nap_size;

    for(int index : q_greed_indexes) {

        ++m_count_of_opers;
        if (static_cast<size_t>(m_sub_sizes[index]) <= remainder_of_cap) {
            q_greed_res += m_sub_values[index];
            q_greed_res_subjects[index] = 1;
            remainder_of_cap -= m_sub_sizes[index];

            m_count_of_opers += 3;
        }
       
    }

    if (max_greed_res >= q_greed_res) {

        m_res_subs = max_greed_res_subjects;
        print_res();
        return max_greed_res;

    } else {

        m_res_subs = q_greed_res_subjects;
        print_res();
        return q_greed_res;

    }

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