#include <iostream>
#include <Eigen/Dense>
/*
d(Output) = (m - k + 1, n - k + 1)
k is the side of the kernel
m is the number of rows of the input matrix
n is the numer of columns of the input matrix
kernel is always squared
*/

auto Conv_2D = [](const Eigen::MatrixXd& X, const Eigen::MatrixXd& K) {
	const int k_rows = K.rows();
	const int k_cols = K.cols();
	// generallt k_rows and k_cols should be the same as traditionally kernels are squares.
	const int rows = (X.rows() - k_rows) + 1; // m - k + 1
	const int cols = (X.cols() - k_cols) + 1; // n - k + 1

	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(rows, cols);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			double sum = X.block(i, j, k_rows, k_cols).cwiseProduct(K).sum();
			res(i, j) = sum;
		}
	}

	return res;
	};

auto naive_Padded_Conv_2D = [](const Eigen::MatrixXd& X, const Eigen::MatrixXd& K, int padding) {
	const int k_rows = K.rows();
	const int k_cols = K.cols();
	const int rows = X.rows() + 2 * padding - k_rows + 1;
	const int cols= X.cols() + 2 * padding - k_cols + 1;

	Eigen::MatrixXd pad = Eigen::MatrixXd::Zero(X.rows() + 2 * padding, X.cols() + 2 * padding);
	pad.block(padding, padding, X.rows(), X.cols()) = X;

	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(rows, cols);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < rows; j++) {
			double sum = pad.block(i, j, k_rows, k_cols).cwiseProduct(K).sum();
			res(i, j) = sum;
		}
	}

	return res;

	};

auto Padded_Conv2D = [](const Eigen::MatrixXd& X, const Eigen::MatrixXd& K, int padding) {
	const int X_rows = X.rows();
	const int X_cols = X.cols();
	const int K_rows = K.rows();
	const int K_cols = K.cols();

	// Best practice, could be used to test in googletest. I will not bother, however
	if (X_rows < K_rows) throw std::invalid_argument("The input has less rows than the kernel");
	if (X_cols < K_cols) throw std::invalid_argument("The input has less columns than the kernel");

	// just like the naive function
	const int rows = X_rows - K_rows + 2 * padding + 1;
	const int cols = X_cols - K_cols + 2 * padding + 1;

	auto fitting_dims = [&padding](int pos, int k, int length) {
		/*
		Took me some time to get it (<5 minutes), but i finally god it.
		So, when we add padding on the top left corner of X, when the kernel passes over the top left most block
		The number in the kernel overlapping with the padding, all return 0. Thus no contribution.
		This function trims the kernel in such a way that is removed all the numbers what would've given a zero,
		giving the exact same result, but with no matrix copy into 'pad'
		*/
		int input = pos - padding;
		int kernel = 0;
		int size = k;
		if (input < 0)
		{
			kernel = -input;
			size += input;
			input = 0;
		}
		if (input + size > length)
		{
			size = length - input;
		}
		return std::make_tuple(input, kernel, size);
		};

	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(rows, cols);

	for (int i = 0; i < rows; ++i)
	{
		const auto [input_i, kernel_i, size_i] = fitting_dims(i, K_rows, X_rows);
		for (int j = 0; size_i > 0 && j < cols; ++j)
		{
			const auto [input_j, kernel_j, size_j] = fitting_dims(j, K_cols, X_cols);
			if (size_j > 0)
			{
				auto input_roi = X.block(input_i, input_j, size_i, size_j); // We do this as we did before
				auto in_kernel = K.block(kernel_i, kernel_j, size_i, size_j); // But whenever there's overlap with padding, we are clipping the kernel to match.
				res(i, j) = input_roi.cwiseProduct(in_kernel).sum();
			}
		}
	}
	return res;
	};


void run_conv_2d() {

	Eigen::MatrixXd K(3, 3);
	K <<
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1;

	Eigen::MatrixXd X(6, 6);
	X <<
		3, 1, 0, 2, 5, 6,
		4, 2, 1, 1, 4, 7,
		5, 4, 0, 0, 1, 2,
		1, 2, 2, 1, 3, 4,
		6, 3, 1, 0, 5, 2,
		3, 1, 0, 1, 3, 3;

	auto out = Conv_2D(X, K);

	std::cout << "Kernel\n" << K << "\n\n";
	std::cout << "Input\n" << X << "\n\n";
	std::cout << "Convolution\n" << out << "\n\n";
}

void run_conv_2d_padded() {

	Eigen::MatrixXd K(3, 3);
	K <<
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1;

	Eigen::MatrixXd X(6, 6);
	X <<
		3, 1, 0, 2, 5, 6,
		4, 2, 1, 1, 4, 7,
		5, 4, 0, 0, 1, 2,
		1, 2, 2, 1, 3, 4,
		6, 3, 1, 0, 5, 2,
		3, 1, 0, 1, 3, 3;

	auto out = naive_Padded_Conv_2D(X, K, 1);

	std::cout << "Kernel\n" << K << "\n\n";
	std::cout << "Input\n" << X << "\n\n";
	std::cout << "Convolution\n" << out << "\n\n";
}

void run_conv_2d_padded_pro() {

	Eigen::MatrixXd K(3, 3);
	K <<
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1;

	Eigen::MatrixXd X(6, 6);
	X <<
		3, 1, 0, 2, 5, 6,
		4, 2, 1, 1, 4, 7,
		5, 4, 0, 0, 1, 2,
		1, 2, 2, 1, 3, 4,
		6, 3, 1, 0, 5, 2,
		3, 1, 0, 1, 3, 3;

	auto out = Padded_Conv2D(X, K, 1);

	std::cout << "Kernel\n" << K << "\n\n";
	std::cout << "Input\n" << X << "\n\n";
	std::cout << "Convolution\n" << out << "\n\n";
}

int main(int, char**) {
	run_conv_2d();
	run_conv_2d_padded();
	run_conv_2d_padded_pro();
}