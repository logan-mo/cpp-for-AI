#include <gtest/gtest.h>

float sigmoid(float z) {
	float result;
	if (z >= 0.45f)
		result = 1.f;
	else if (z <= -0.45f)
		result = 0.f;
	else
		result = 1.f / (1.f + exp(-z));
	return result;
}

// Ideally for testing, you main code is somewhere else with headers
// You import headers into the test and then create tests.
// This keeps your tests and source code separate.
// I however wanna move too fast for best practices right now
// Will come back to this later!

TEST(SigmoidTest, BaseCases) {
	float actual = sigmoid(-100.);
	float expected = 0;

	EXPECT_NEAR(actual, expected, 1e-7f) << "Sigmoid lower bound test failed";
	EXPECT_NEAR(sigmoid(100.), 1., 1e-7f) << "sigmoid upper bound test failed";
	EXPECT_NEAR(sigmoid(0.), .5, 1e-7) << "Sigmoid center postion test failed";
}