#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <memory>
#include <array>
#include <utility>
#include <climits>
// Exercise Question 3
// Using lambdas, write a function to find the k-smallest numbers of an arbitrary vector of integers.

// Oh boy, I just learnt c++ lambdas like yesterday. I can find k smallest numers in k passes. But I think the fun part would be to do so in one pass? Let's give it a shot!
// Attempting top k in k passes in this file.

// So My idea was to keep a list of the top 5 currently largest numbers in a k sized queue. Max on one size, min on another.
// Everytime we encounter a number larger than the min, we insertion sort in
// And the dequeue the smallest number in the top list.

// This would've taken quite some time, but for the sake of moving onto deep learning, I am skipping that implementation.

/*
Reference Lambda structure from memory:

auto func1 = [/pass through parameters? dunno what they are called/] (/actual lambda parameters/) { /lambda code/}; ... and somewhere in there is an option to make it mutable
*/

// bool is_in(const int& num, const std::vector<int>& vec) {
//     for (int iter : vec)
//         if (num == iter)
//             return true;
//     return false;
// }

// std::unique_ptr<std::vector<int>> top_k_k_pass(const std::vector<int>& vec, const int& k) {
//     auto top_k = std::make_unique <std::vector<int>>();
//     std::vector<int> top_k_indices;
//     int size_top_k = 0;
    
//     for (int i = 0; i < k; i++) {
//         int max = -1;
//         int idx_max = -1;
        
//         for (int j = 0; j < vec.size(); j++) {
//             // check if j is already in top_k_indices
//             if (is_in(j, top_k_indices))
//                 continue;
            
//             // Start by assuming that the largest number is the first number
//             if (idx_max == -1) {
//                 idx_max = j;
//                 max = vec.at(j);
//                 continue;
//             }

//             //Challenge the above assumption
//             if (vec.at(j) > max) {
//                 max = vec.at(j);
//                 idx_max = j;
//             }
//         }
//         (*top_k).push_back(max);
//         top_k_indices.push_back(idx_max);
//     }
//     return top_k;
// }

// int main() {

    
//     constexpr int N = 10;
//     constexpr int k = 2;
//     std::vector<int> arbitrary_vector;

//     std::random_device rd{};
//     std::uniform_real_distribution<> dist{0, 100};

//     // dist(rd) for a random int
//     for (int i = 0; i < N; i++)
//         arbitrary_vector.push_back(dist(rd));

//     std::cout << "Arbitraty Vector: ";
//     for(auto num: arbitrary_vector)
//         std::cout << num << " ";
//     std::cout << "\n" << "K: " << k << "\n";
//     std::vector<int> top_k = *top_k_k_pass(arbitrary_vector, k);

//     std::cout << "Top K: ";
//     for (auto num : top_k)
//         std::cout << num << " ";
//     std::cout << "\n";


// 	return 0;
// }

// since the actual goal was to do this all using lambdas, transforming the above working code into lambdas


auto is_in = [](int num, const std::vector<int>& vec) {
    for (int iter : vec)
        if (num == iter)
            return true;
    return false;
};

auto get_max = [](const std::vector<int>& vec, const std::vector<int>& excluded_indices) {
    int max = INT_MIN;
    int idx_max = -1;

    for (int j = 0; j < vec.size(); j++) {
        // check if j is already in top_k_indices
        if (is_in(j, excluded_indices))
            continue;

        //Challenge the above assumption
        if (vec.at(j) > max) {
            max = vec.at(j);
            idx_max = j;
        }
    }
    return std::pair<int, int> {max, idx_max};
    // previously:
    // return std::make_unique<std::array<int, 2>>({max, idx_max})
    // and then dereferencing the return value and then splitting it into to variables using indices 0 and 1. yuck.
};

auto get_top_k = [](const std::vector<int>& vec, int k) {
    std::vector<int> top_k;
    std::vector<int> top_k_indices;

    for (int i = 0; i < k; i++) {
        auto [max, idx_max] = get_max(vec, top_k_indices);
        top_k.push_back(max);
        top_k_indices.push_back(idx_max);
    }
    return top_k;
    
};

int main() {
    constexpr int N = 10;
    constexpr int k = 2;

    std::vector<int> arbitrary_vector;
    std::random_device rd{};
    std::uniform_real_distribution<> dist{0, 100};

    // dist(rd) for a random int
    for (int i = 0; i < N; i++)
        arbitrary_vector.push_back(dist(rd));

    std::cout << "Arbitraty Vector: ";
    for(auto num: arbitrary_vector)
        std::cout << num << " ";
    std::cout << "\n" << "K: " << k << "\n";
    std::vector<int> top_k = get_top_k(arbitrary_vector, k);

    std::cout << "Top K: ";
    for (auto num : top_k)
        std::cout << num << " ";
    std::cout << "\n";


	return 0;
}