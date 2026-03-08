#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <gtest/gtest.h>
#include <chrono>
#include <random>

std::vector<float> glorot_initializer(int fan_in, int fan_out) {
	/*
	For a Glorot Initializer, the mean is 0 and the std-dev is sqrt(2 / (fan_in + fan_out))
	And that is exactly how we will test a glorot initializer
	*/
	int size = fan_in * fan_out;

	float std_dev = sqrt(2. / (fan_in + fan_out));

	unsigned seed = std::chrono::system_clock::now()
		.time_since_epoch()
		.count();
	
	std::default_random_engine generator(seed);
	std::normal_distribution<float> distribution(.0, std_dev);

	std::vector<float> result(size);
	std::generate(result.begin(), result.end(), [&generator, &distribution]
		() { return distribution(generator); }
	);

	return result;
}

using namespace boost::math;

/**
* The NULL hypothesis states sample_std eq population_std
*
* This function returns true if the NULL hypothesis was rejected
*/
bool chi_squared_test(float population_std, float sample_std, int sample_size, float confidence) {
	float variance_rate = sample_std / population_std;
	variance_rate = variance_rate * variance_rate;
	float t_stat = (sample_size - 1) * variance_rate;

	chi_squared distro(sample_size - 1);

	float upper_limit = quantile(complement(distro, confidence / 2));
	float lower_limit = quantile(distro, confidence / 2);

	return t_stat > upper_limit && t_stat < lower_limit;
}

bool t_test(float population_mean, float sample_mean, float sample_std_dev, int sample_size, float confidence) {
	float diff = sample_mean - population_mean;

	float t_stat = diff * sqrt(double(sample_size)) / sample_std_dev;

	unsigned degree_of_freedom = sample_size - 1;
	students_t distro(degree_of_freedom);
	float q = cdf(complement(distro, fabs(t_stat)));

	float confidence _2 = confidence / 2.;

	return q < confidence _2;
}

TEST(CheckGlorot, StatCases)
{
	int fan_in = 6;
	int fan_out = 5;

	auto weigths = glorot_initializer(fan_in, fan_out);

	const int N = weigths.size();

	EXPECT_EQ(N, fan_in * fan_out);

	float sum = std::accumulate(weigths.begin(), weigths.end(), 0.0);
	float weight_mean = sum / N;

	float acc = 0.0;

	auto differ = [&acc, &weight_mean](const float val) {
		const float diff = val - weight_mean;
		acc += diff * diff;
		};

	std::for_each(weigths.begin(), weigths.end(), differ);

	float actual_stdev = sqrt(acc / (N - 1));

	float expected_stddev = sqrt(2. / (fan_in + fan_out));

	bool std_dev_rejected = chi_squared_test(expected_stddev, actual_stdev, N, 0.05);

	if (std_dev_rejected) {
		FAIL() << "The weights standard deviation do not look like expected by the Glorot initializer";
	}

	bool mean_rejected = t_test(0., weight_mean, actual_stdev, N, 0.05);

	if (mean_rejected) {
		FAIL() << "The weights mean do not look like expected by the Glorot initializer";
	}

}

// I gave up on making this work because boost installation using vcpkg took forever and I don't have the patience.
// I'm sure this won't come back to bite me.
// so no, no exercises for this chapter