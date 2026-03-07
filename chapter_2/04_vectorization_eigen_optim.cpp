#include <iostream>
#include <random>
#include <string>
#include <array>
#include <vector>
#include <memory>
#include <chrono>

#define EIGEN_VECTORIZE
#define EIGEN_VECTORIZE_AVX
#define EIGEN_VECTORIZE_AVX2
#include <Eigen/Dense>
// In this file, I just want to manuall implement a matrix multiplication of two matrices
// First I will do it using my own hand written code
// Then I will use Eigen to see how much of a speedup I can get



constexpr int N = 10000; // ← change this one value to resize everything

std::random_device rd{};
std::mt19937 random_generator{ rd() };
std::uniform_int_distribution<int> dist(0, 100);

std::unique_ptr<Eigen::MatrixXd> init_2d_matrix_eigen() {
	return std::make_unique<Eigen::MatrixXd>(
		Eigen::MatrixXd::Random(N, N) * 50 + Eigen::MatrixXd::Constant(N, N, 50)
	);
}

// Benchmark helper
void print_time(const std::string& label, std::chrono::duration<double, std::milli> elapsed) {
	std::cout << label << ": " << elapsed.count() << " ms\n";
}

int main() {
	std::cout << "Matrix size: " << N << "x" << N << "\n\n";

	auto A_eigen = init_2d_matrix_eigen();
	auto B_eigen = init_2d_matrix_eigen();

	auto t7 = std::chrono::high_resolution_clock::now();
	Eigen::MatrixXd C_eigen = (*A_eigen) * (*B_eigen);
	auto t8 = std::chrono::high_resolution_clock::now();
	print_time("Eigen	", t8 - t7);
	return 0;
}