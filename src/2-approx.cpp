#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include "Knapsack.hpp"


auto Knapsack::algorithm(bool debug) -> size_t {

    size_t max_greed_res = 0;
    size_t q_greed_res = 0;

    std::vector<size_t> max_greed_indexes;
    std::vector<size_t> q_greed_indexes;

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
        return static_cast<double>(m_values[a]) > static_cast<double>(m_values[b]);
    });

    std::sort(q_greed_indexes.begin(), q_greed_indexes.end(),
    [&](const int a, const int b) {
        ++m_count_of_opers;
        return static_cast<double>(m_values[a])/static_cast<double>(m_weights[a]) >
            static_cast<double>(m_values[b])/static_cast<double>(m_weights[b]);
    });

    size_t remainder_of_cap = m_knapsack_capacity;

    for(auto index : max_greed_indexes) {

        ++m_count_of_opers;
        if (static_cast<size_t>(m_weights[index]) <= remainder_of_cap) {
            max_greed_res += m_values[index];
            max_greed_res_subjects[index] = 1;
            remainder_of_cap -= m_weights[index];

            m_count_of_opers += 3;
        }

    }

    remainder_of_cap = m_knapsack_capacity;

    for(auto index : q_greed_indexes) {

        ++m_count_of_opers;
        if (static_cast<size_t>(m_weights[index]) <= remainder_of_cap) {
            q_greed_res += m_values[index];
            q_greed_res_subjects[index] = 1;
            remainder_of_cap -= m_weights[index];

            m_count_of_opers += 3;
        }
       
    }

    if (max_greed_res >= q_greed_res) {

        m_res_subs = max_greed_res_subjects;
        print_res();
        return max_greed_res;

    }  
        m_res_subs = q_greed_res_subjects;
        print_res();
        return q_greed_res;

   

}

auto main() -> int {

    std::vector<std::chrono::duration<double>> all_results(9, static_cast<std::chrono::duration<double>>(0));

    std::vector<std::chrono::duration<double>> one_run(9);


    Knapsack_app(one_run, true);

    for (int j = 0; j < 9; ++j) {
        all_results[j] += one_run[j];
    }

}
