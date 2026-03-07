#include <iostream>
#include <random>
#include <string>
#include <array>
#include <vector>
#include <memory>
#include <chrono>
#include <Eigen/Dense>
// In this file, I just want to manuall implement a matrix multiplication of two matrices
// First I will do it using my own hand written code
// Then I will use Eigen to see how much of a speedup I can get



constexpr int N = 1000; // ← change this one value to resize everything

std::random_device rd{};
std::mt19937 random_generator{ rd() };
std::uniform_int_distribution<int> dist(0, 100);

// Using static arrays
using Matrix_arr = std::array<std::array<int, N>, N>;
std::unique_ptr<Matrix_arr> init_2d_matrix_arr() {
	auto m = std::make_unique<Matrix_arr>();
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			(*m)[i][j] = dist(random_generator);
	return m;
}

std::unique_ptr<Matrix_arr> multiply_arr(const Matrix_arr& A, const Matrix_arr& B) {
	auto C = std::make_unique<Matrix_arr>();
	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				(*C)[i][k] += A[i][j] * B[j][k];
	return C;
}

// Using a more "cache friendly" approach as GPT recommended.
std::unique_ptr<Matrix_arr> multiply_arr_cache(const Matrix_arr& A, const Matrix_arr& B) {
	auto C = std::make_unique<Matrix_arr>();
	for (int i = 0; i < N; i++)
		for (int k = 0; k < N; k++)
			for (int j = 0; j < N; j++)
				(*C)[i][j] += A[i][k] * B[k][j];
	return C;
}



//Using st::vector
using Matrix_vec = std::vector<std::vector<int>>;

std::unique_ptr<Matrix_vec> init_2d_matrix_vec() {
	auto m = std::make_unique<Matrix_vec>(N, std::vector<int>(N));
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			(*m)[i][j] = dist(random_generator);
	return m;
}

std::unique_ptr<Matrix_vec> multiply_vec(const Matrix_vec& A, const Matrix_vec& B) {
	auto C = std::make_unique<Matrix_vec>(N, std::vector<int>(N));
	for (int i = 0; i < N; i++)
		for (int k = 0; k < N; k++)
			for (int j = 0; j < N; j++)
				(*C)[i][j] += A[i][k] * B[k][j];
	return C;
}


// Using Eigen
std::unique_ptr<Eigen::MatrixXd> init_2d_matrix_eigen() {
	auto m = std::make_unique< Eigen::MatrixXd>(N, N);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			(*m)(i,j) = dist(random_generator);

	return m;
}

// Benchmark helper
void print_time(const std::string& label, std::chrono::duration<double, std::milli> elapsed) {
	std::cout << label << ": " << elapsed.count() << " ms\n";
}

int main() {
	std::cout << "Matrix size: " << N << "x" << N << "\n\n";

	auto A_arr = init_2d_matrix_arr();
	auto B_arr = init_2d_matrix_arr();

	auto t1 = std::chrono::high_resolution_clock::now();
	auto C_arr = multiply_arr(*A_arr, *B_arr);
	auto t2 = std::chrono::high_resolution_clock::now();
	print_time("Array (naive)      ", t2 - t1);

	auto t3 = std::chrono::high_resolution_clock::now();
	auto C_arr_cache = multiply_arr_cache(*A_arr, *B_arr);
	auto t4 = std::chrono::high_resolution_clock::now();
	print_time("Array (cache)      ", t4 - t3);

	auto A_vec = init_2d_matrix_vec();
	auto B_vec = init_2d_matrix_vec();

	auto t5 = std::chrono::high_resolution_clock::now();
	auto C_vec = multiply_vec(*A_vec, *B_vec);
	auto t6 = std::chrono::high_resolution_clock::now();
	print_time("Vector (cache)     ", t6 - t5);

	auto A_eigen = init_2d_matrix_eigen();
	auto B_eigen = init_2d_matrix_eigen();

	auto t7 = std::chrono::high_resolution_clock::now();
	Eigen::MatrixXd C_eigen = (*A_eigen) * (*B_eigen);
	auto t8 = std::chrono::high_resolution_clock::now();
	print_time("Eigen	", t8 - t7);

/*
Output at 1000x1000 (Release Mode):
Matrix size: 1000x1000 (1M Params)

Array (naive)      : 1653.69 ms
Array (cache)      : 519.864 ms // Using a different loop order gives better performance because of CPU cache hits. Magic.
Vector (cache)     : 1506.36 ms // Apparently this is slower because I made nester vectors, which are heap allocation and thus not optimized
// For optimized, I shouldv'e used a flat vector
Eigen   : 187.55 ms // And there, the beauty, the queen!

Let's try for 10k x 10k (100M Params)
Matrix size: 10000x10000

Array (cache)      : 544046 ms
Eigen   : 177951 ms
3x

...

Now to see the full potential of Eigen, Replacing the eigen init witl an eigen native init
and doing some AVX2, OpenMP and float model to fast
Final code in the file 04_vectorization_eigen_optim.cpp
Matrix size: 10000x10000

Eigen   : 17466.6 ms
31x compared to cache friendly arrays. WTF. this is almost native python vs numpy levels of optim and that is a hard line to beat.


*/

	return 0;
}