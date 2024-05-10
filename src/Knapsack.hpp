#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

const std::vector<std::vector<std::string>> test_files = {
    {"./benchmarks/p01_p.txt", "./benchmarks/p01_w.txt", "./benchmarks/p01_c.txt", "./benchmarks/p01_s.txt"},
    {"./benchmarks/p02_p.txt", "./benchmarks/p02_w.txt", "./benchmarks/p02_c.txt", "./benchmarks/p02_s.txt"},
    {"./benchmarks/p03_p.txt", "./benchmarks/p03_w.txt", "./benchmarks/p03_c.txt", "./benchmarks/p03_s.txt"},
    {"./benchmarks/p04_p.txt", "./benchmarks/p04_w.txt", "./benchmarks/p04_c.txt", "./benchmarks/p04_s.txt"},
    {"./benchmarks/p05_p.txt", "./benchmarks/p05_w.txt", "./benchmarks/p05_c.txt", "./benchmarks/p05_s.txt"},
    {"./benchmarks/p06_p.txt", "./benchmarks/p06_w.txt", "./benchmarks/p06_c.txt", "./benchmarks/p06_s.txt"},
    {"./benchmarks/p07_p.txt", "./benchmarks/p07_w.txt", "./benchmarks/p07_c.txt", "./benchmarks/p07_s.txt"},
};

void read_file(const std::string& path_to_file, std::vector<int>& vec) {

    std::ifstream in;
    in.open(path_to_file);

    int temp_int;

    if (!in.is_open()) {
        std::cout << "incorrect file name" << std::endl;
    } else {

        while(!in.eof()) {
            in >> temp_int;
            if(!in.eof()) {
                vec.push_back(temp_int);
            }
        }

        in.close();
    }
}

void read_file(const std::string& path_to_file, size_t& res) {

    std::ifstream in;
    in.open(path_to_file);

    if (!in.is_open()) {
        std::cout << "incorrect file name" << std::endl;
    } else {
        in >> res;
        in.close();
    }
}


struct Object {
    int weight;
    int value;
    int number;

    Object(int wgt, int val, int num) :
        weight(wgt),
        value(val),
        number(num) {}

    auto get_ratio() -> double {
        return static_cast<double>(value) / weight;
    }
};


class Knapsack {
public:

    Knapsack(const std::string& path_to_values, const std::string& path_to_weights,
            const std::string& path_to_knapsack_capacity, const std::string& path_to_res) {
        // profits of each object
        read_file(path_to_values, m_values);
        // weights of the objects
        read_file(path_to_weights, m_weights);
        // knapsack capacity
        read_file(path_to_knapsack_capacity, m_knapsack_capacity);
        // optimal selection of weights
        read_file(path_to_res, m_res);

        m_count_of_sub = m_values.size();
        m_res_subs = std::vector<int>(m_count_of_sub, 0);
    }

    void print_values() const {

        for (int elem: m_values) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;

    }

    void print_weights() const {

        for (int elem: m_weights) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;

    }

    void print_res() const {
    std::cout << "----------RESULT----------" << std::endl;
    std::cout << "Items list:" << " ";
        for (int elem: m_res_subs) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;

    }

    auto algorithm(bool debug = false) -> size_t;

    [[nodiscard]] auto get_total_weight() const -> int {
        int total_weight = 0;
        for (int i = 0; i < m_count_of_sub; ++i) {
            if (m_res_subs[i] == 1) {
                total_weight += m_weights[i];
            }
        }
        return total_weight;
    }
    [[nodiscard]] auto get_count_of_opers() const -> size_t { return m_count_of_opers; } 
    [[nodiscard]] auto get_knapsack_capacity() const -> size_t { return m_knapsack_capacity; }
    [[nodiscard]] auto get_count_of_sol() const -> int { return m_count_of_sol; }

    // to easy work with branch_and_bound_alg
    void define_by_object_struct() {
        for (int i = 0; i < m_count_of_sub; ++i) {
            obj_arr.emplace_back(m_weights[i], m_values[i], i);
        }
    }

private:
    std::vector<int> m_values;
    std::vector<int> m_weights;
    size_t m_knapsack_capacity;
    size_t m_res;

    size_t m_count_of_sub;
    std::vector<int> m_res_subs = {};
    
    size_t m_count_of_opers = 0;

    std::vector<Object> obj_arr;

    int m_count_of_sol = 0;
};


void Knapsack_app(std::vector<std::chrono::duration<double>>& all_results, bool isPrint, bool debug = false, bool obj_struct = false) {
    // ALL TESTS
    std::chrono::duration<double> all_elapsed{};

    int num = 0;

    for (auto test: test_files) {
        if (isPrint) {
            std::cout << "----------TEST " << num << "----------" <<std::endl;
        }

        Knapsack pack(test[0], test[1], test[2], test[3]);
        
        std::cout << "Capacity: " << pack.get_knapsack_capacity() << std::endl;
        std::cout << "Prices: ";
        pack.print_values();
        std::cout << "Weights: ";
        pack.print_weights();
        std::cout << std::endl;

        if (obj_struct) {
            pack.define_by_object_struct();
        }

        // ALGHORITM 1 TEST TIME START
        auto start = std::chrono::high_resolution_clock::now();

        auto result = pack.algorithm(debug);

        auto end = std::chrono::high_resolution_clock::now();

        // ALGHORITM 1 TEST TIME END
        std::chrono::duration<double> elapsed = end - start;

        all_elapsed += elapsed;

        if (isPrint) {
            std::cout << "Knapsack value: " << result << std::endl;
            std::cout << "Total weight: " << pack.get_total_weight() << std::endl;
            std::cout << "Time: " << elapsed.count() << " s" << std::endl;
            std::cout << "Count of opers: " << pack.get_count_of_opers() << std::endl;
            std::cout << "Count of interim solutions: " << pack.get_count_of_sol() << std::endl;

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
