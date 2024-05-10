#include <queue>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include "Knapsack.hpp"


class Node {
public:
    double upper_bound; // relaxation solution
    int total_value;
    int total_weight;
    int level;
    std::vector<int> included_objects = {};
};

auto calc_upper_bound(const Node& node, size_t obj_count, size_t knapsack_capacity,
                      const std::vector<Object>& obj_arr, size_t& count_of_opers) -> double {
    
    int value_bound = node.total_value;
    int idx = node.level + 1;
    int totweight = node.total_weight;

    while ((idx < obj_count) && (totweight + obj_arr[idx].weight <= knapsack_capacity)) {
        ++count_of_opers;
        totweight += obj_arr[idx].weight;
        value_bound += obj_arr[idx].value;
        idx++;
    }
    ++count_of_opers;
    
    ++count_of_opers;
    if (idx < obj_count) {
        value_bound += (static_cast<int>(knapsack_capacity) - totweight) * obj_arr[idx].value / obj_arr[idx].weight;
    }
    
    return value_bound;
}

auto Knapsack::algorithm(bool debug) -> size_t {
    // sort by ratio
    std::sort(obj_arr.begin(), obj_arr.end(),
            [&](const Object& a, const Object& b) {
            ++m_count_of_opers;
            return (static_cast<double>(a.value) / a.weight) > (static_cast<double>(b.value) / b.weight);
    });

    int value_res = 0;
    std::vector<int> objects_in_res;
    // положим в value_res - нижнюю границу (подсчет жадной эвристики)
    auto calc_greedy = [&] () {
        int current_weight = 0;
        int total_value = 0;

        for (const auto& obj : obj_arr) {
            m_count_of_opers++;
            if (current_weight + obj.weight <= m_knapsack_capacity) {
                current_weight += obj.weight;
                total_value += obj.value;
                objects_in_res.push_back(obj.number);
            }
        }
        return total_value;
    };
    value_res = calc_greedy();

    std::vector<Node> search_vec;

    Node parent;
    Node child;

    parent.level = -1;
    parent.total_value = parent.total_weight = 0;
    search_vec.push_back(parent);

    while (!search_vec.empty()) {
        int child_count = 0; // ЕСЛИ ДЕТЕЙ НЕ БУДЕТ, ТО ПРОМЕЖУТОЧНОЕ РЕШЕНИЕ (костыль немного)

        ++m_count_of_opers;

        parent = search_vec.front();
        search_vec.erase(search_vec.begin());
        
        ++m_count_of_opers;
        if (parent.level == m_count_of_sub - 1) {
            m_count_of_sol++; // ПРОМЕЖУТОЧНОЕ РЕШЕНИЕ, ТАК КАК ЛИСТ
            continue;
        }

        child.level = parent.level + 1;

        // Берем предмет
        child.total_weight = parent.total_weight + obj_arr[child.level].weight;
        child.total_value = parent.total_value + obj_arr[child.level].value;
        child.included_objects = parent.included_objects;
        child.included_objects.push_back(obj_arr[child.level].number);

        ++m_count_of_opers;
        if (child.total_weight <= m_knapsack_capacity && child.total_value > value_res) {
            value_res = child.total_value;
            objects_in_res = child.included_objects;
        }

        child.upper_bound = calc_upper_bound(child, m_count_of_sub,
                        m_knapsack_capacity, obj_arr, m_count_of_opers);

        ++m_count_of_opers;
        if (child.upper_bound > value_res) {
            search_vec.push_back(child);
            child_count++;
        }

        // Не берем
        child.total_weight = parent.total_weight;
        child.total_value = parent.total_value;
        child.included_objects.pop_back();

        child.upper_bound = calc_upper_bound(child, m_count_of_sub,
                        m_knapsack_capacity, obj_arr, m_count_of_opers);

        ++m_count_of_opers;
        if (child.upper_bound > value_res) {
            search_vec.push_back(child);
            child_count++;
        }

        if (child_count == 0) {
            m_count_of_sol++; // ПРОМЕЖУТОЧНОЕ РЕШЕНИЕ
        }
    }
    ++m_count_of_opers;

    for (auto obj: objects_in_res) {
        m_res_subs[obj] = 1;
    }

    print_res();

    return value_res;
}

auto main() -> int {

    std::vector<std::chrono::duration<double>> all_results(9, static_cast<std::chrono::duration<double>>(0));

    std::vector<std::chrono::duration<double>> one_run(9);


    Knapsack_app(one_run, true, false, true);

    for (int j = 0; j < 9; ++j) {
        all_results[j] += one_run[j];
    }

}