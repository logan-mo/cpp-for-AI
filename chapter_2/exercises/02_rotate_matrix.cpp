#include <iostream>
#include <Eigen/Dense>



// Exercise Question 2 
// Using eigen, write a program to rotate a matrix 90 degrees clockwise or anticlockwise

// I just looked up Eigen rotate, and boy, Is there alotta stuff.
// But all that geometric and spatial transfomations was overkill for what I was looking for.
// Just rotating a matrix involves a mix of transpose, colwise, rowwise and reverse



int main() {

    
    // Equivalrnt to np.random(10, 10) * 50 + 50
    constexpr int N = 10;
    Eigen::MatrixXd mat = Eigen::MatrixXd::Random(N, N) * 50 + Eigen::MatrixXd::Constant(N, N, 50);
   

    std::cout << "Mat:\n" << mat << std::endl;
   
    // For a clockwise 90 degree rotation... I think a horizontal flip along y axis, followed by whatever is the reverse of a transpose on the opposite diagonal? knowing what i know i don't think that operation exists
    // I mean a normal transpose followed by a rowwise reverse should do the trick?

    std::cout << "Mat rotated 90 degree clockwise:\n" << mat.transpose().rowwise().reverse() << std::endl; // works like a charm

    // for anticlockwise 90 degree rotation, A transpose followed by a vertical flip?

    std::cout << "Mat rotated 90 degrees anti clockwise\n" << mat.transpose().colwise().reverse() << std::endl; // I might be the smartest person alive. (lol or just I have good mental imagination)


	return 0;
}