#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "Knapsack.hpp"

void generate_combinations(size_t n, int max_k, std::vector<std::vector<int>>& combinations) {
    combinations.emplace_back();
    for (int k = 1; k <= max_k; ++k) {
        std::string bitmask(k, 1);
        bitmask.resize(n, 0);
        do {
            std::vector<int> comb;
            for (int i = 0; i < n; ++i) {
                if (bitmask[i] != 0) {
                    comb.push_back(i);
                }
            }
            combinations.push_back(comb);
        } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    }
}

// ptas by sahni
auto Knapsack::algorithm(bool debug) -> size_t {
    // procedure GS
    auto grid_search = [&] (std::vector<int>& res, const std::vector<int>& subset_M) {
        int subset_weight = 0;
        for (auto obj_M : subset_M) {
            subset_weight += m_weights[obj_M];
        }

        int available_cap = m_knapsack_capacity - subset_weight;
        int not_in_subset_value = 0;

        for (int obj = 0; obj < m_count_of_sub; ++obj) {
            m_count_of_opers++;
            if ((std::find(subset_M.begin(), subset_M.end(), obj) != subset_M.end())
                && m_weights[obj] <= available_cap) {
                res.push_back(obj);
                available_cap -= m_weights[obj];
                not_in_subset_value += m_values[obj];
            }
        }
        return not_in_subset_value;
    };

    // generate M_subsets
    std::vector<std::vector<int>> combinations;
    int k = 3;
    generate_combinations(m_count_of_sub, k, combinations);
    std::vector<std::vector<int>> all_subsets_M;
    for (const auto& subset : combinations) {
        m_count_of_opers++;
        int subset_weight = 0;
        for (auto obj : subset) {
            subset_weight += m_weights[obj];
        }
        m_count_of_opers++;
        if (subset_weight <= m_knapsack_capacity) {
            all_subsets_M.push_back(subset);
        }
    }

    int knapsack_profit = 0;
    std::vector<int> knapsack_objs;

    for (const auto& subset_M : all_subsets_M) {
        std::vector<int> not_in_M_subset;
        // not M values sum
        int this_value = grid_search(not_in_M_subset, subset_M);
        // + M values sum
        for (auto obj : subset_M) {
            this_value += m_values[obj];
        }
        m_count_of_opers++;
        if (this_value > knapsack_profit) {
            knapsack_profit = this_value;
            knapsack_objs.clear();
            not_in_M_subset.insert(not_in_M_subset.end(), subset_M.begin(), subset_M.end());
            knapsack_objs = not_in_M_subset;
        }
    }

    for (auto obj: knapsack_objs) {
        m_res_subs[obj] = 1;
    }

    print_res();

    return knapsack_profit;
}

auto main() -> int {

    std::vector<std::chrono::duration<double>> all_results(9, static_cast<std::chrono::duration<double>>(0));

    std::vector<std::chrono::duration<double>> one_run(9);


    Knapsack_app(one_run, true);

    for (int j = 0; j < 9; ++j) {
        all_results[j] += one_run[j];
    }

}