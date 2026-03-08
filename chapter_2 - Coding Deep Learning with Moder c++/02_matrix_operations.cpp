#include <iostream>
#include <Eigen/Dense>

int main(int, char**) {

    Eigen::MatrixXd A(2, 2);
    A(0, 0) = 2.;
    A(1, 0) = -2.;
    A(0, 1) = 3.;
    A(1, 1) = 1.;

    Eigen::MatrixXd B(2, 3);
    B(0, 0) = 1.;
    B(1, 0) = 1.;
    B(0, 1) = 2.;
    B(1, 1) = 2.;
    B(0, 2) = -1.;
    B(1, 2) = 1.;

    auto C = A * B;

    std::cout << "A:\n" << A << std::endl;
    std::cout << "B:\n" << B << std::endl;
    std::cout << "C:\n" << C << std::endl;

    std::cout << "A inverse\n" << A.inverse() << std::endl;
    std::cout << "A * A Inverse\n" << A * A.inverse() << std::endl;
    ////std::cout << "Seeing what error I see if I inverse a non square matrix\n" << B.inverse() << std::endl;  //Row size /= Column size assetion failed. Not shooting myself in the foot. Fantastic so far.

    std::cout << "det(A)\n" << A.determinant() << std::endl;
    std::cout << "transpose(B)\n" << B.transpose() << std::endl;

    std::cout << "Unary and Binary coefficient-wise operation" << std::endl;
    
    
    auto funcX_X = [](double x) {return x * x; };

    std::cout << "Unary x^2 on A\n" << A.unaryExpr(funcX_X) << std::endl;
    // So unary functions are applied element wise on a whole matrix
    auto funcX_Y = [](double x, double y) {return x * y; };

    std::cout << "Binary x*y on B and C\n" << B.binaryExpr(C, funcX_Y) << std::endl;
    // And binary functions are applied on two same shape matrices in binary element wise. Got it.


    /*
    More operations:
        A.inverse() Gives invrse of a square matrix
        A.determinant()
        A.transpose()
    */


    return 0;
}
