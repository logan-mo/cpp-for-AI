#include <iostream>
#include <Eigen/Dense>


// Exercise Question 1: 
// Using eigen, write a program to flip a matrix vertically or horizontally

// Reference code that I got from a different file:
// Eigen::MatrixXd::Random(N, N) * 50 + Eigen::MatrixXd::Constant(N, N, 50)


int main() {

    
    // Equivalrnt to np.random(10, 10) * 50 + 50
    constexpr int N = 10;
    Eigen::MatrixXd mat = Eigen::MatrixXd::Random(N, N) * 50 + Eigen::MatrixXd::Constant(N, N, 50);
   
    // For a vertical flip, mat.colwise().reverse() should do a vertical flip.
    // mat.rowwise().reverse() should probably be horizontal flip

    std::cout << "Mat:\n" << mat << std::endl;
    std::cout << "Mat Y flip:\n" << mat.colwise().reverse() << std::endl;
    std::cout << "Mat X flip:\n" << mat.rowwise().reverse() << std::endl;


	return 0;
}