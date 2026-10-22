#include "lab02/polynomial.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <gtest/gtest.h>

namespace lab02 {

TEST(PolynomialTest, DefaultPolynomialIsZero) {
    const Polynomial polynomial;
    std::ostringstream out;
    polynomial.print(out);

    EXPECT_EQ(polynomial.degree(), -1);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(5.0), 0.0);
    EXPECT_EQ(out.str(), "0");
}

TEST(PolynomialTest, AllZeroCoefficientsProduceZero) {
    const double coeff[]{0.0, 0.0, 0.0};
    const Polynomial polynomial(coeff, 2);

    EXPECT_EQ(polynomial.degree(), -1);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 0.0);
}

TEST(PolynomialTest, ConstructorCopiesInputAndTrimsTrailingZeros) {
    double coeff[]{3.0, 2.0, 0.0, 0.0};
    const Polynomial polynomial(coeff, 3);
    coeff[0] = 100.0;

    EXPECT_EQ(polynomial.degree(), 1);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 7.0);
}

TEST(PolynomialTest, SingleCoefficientIsConstant) {
    const double coeff[]{7.0};
    const Polynomial polynomial(coeff, 0);

    EXPECT_EQ(polynomial.degree(), 0);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(-10.0), 7.0);
}

TEST(PolynomialTest, CopyConstructorCreatesIndependentBuffer) {
    const double coeff[]{3.0, 2.0, 5.0};
    Polynomial original(coeff, 2);
    const Polynomial copy(original);
    original.add(original);

    EXPECT_DOUBLE_EQ(copy.evaluate(2.0), 27.0);
    EXPECT_DOUBLE_EQ(original.evaluate(2.0), 54.0);
    const Polynomial zero;
    const Polynomial zero_copy(zero);
    EXPECT_EQ(zero_copy.degree(), -1);
}

TEST(PolynomialTest, MoveConstructorTransfersBufferAndEmptiesSource) {
    const double coeff[]{3.0, 2.0, 5.0};
    Polynomial original(coeff, 2);
    const Polynomial moved(std::move(original));

    EXPECT_DOUBLE_EQ(moved.evaluate(2.0), 27.0);
    EXPECT_EQ(original.degree(), -1);
    EXPECT_DOUBLE_EQ(original.evaluate(2.0), 0.0);
}

TEST(PolynomialTest, CopyAssignmentReplacesBufferIndependently) {
    const double coeff[]{3.0, 2.0, 5.0};
    const double old_coeff[]{7.0};
    Polynomial original(coeff, 2);
    Polynomial copy(old_coeff, 0);

    EXPECT_EQ(&(copy = original), &copy);
    original.add(original);
    EXPECT_DOUBLE_EQ(copy.evaluate(2.0), 27.0);
    EXPECT_DOUBLE_EQ(original.evaluate(2.0), 54.0);
}

TEST(PolynomialTest, SelfAssignmentPreservesPolynomial) {
    const double coeff[]{3.0, 2.0, 5.0};
    Polynomial polynomial(coeff, 2);
    const Polynomial& alias = polynomial;
    polynomial = alias;

    EXPECT_EQ(polynomial.degree(), 2);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 27.0);
}

TEST(PolynomialTest, MoveAssignmentAllowsSourceReuse) {
    const double coeff[]{3.0, 2.0, 5.0};
    const double constant[]{7.0};
    Polynomial source(coeff, 2);
    Polynomial target(constant, 0);
    target = std::move(source);

    EXPECT_DOUBLE_EQ(target.evaluate(2.0), 27.0);
    EXPECT_EQ(source.degree(), -1);
    source = Polynomial(constant, 0);
    EXPECT_DOUBLE_EQ(source.evaluate(2.0), 7.0);
}

TEST(PolynomialTest, SwapExchangesZeroAndNonzeroStates) {
    const double coeff[]{3.0, 2.0, 5.0};
    Polynomial polynomial(coeff, 2);
    Polynomial zero;
    polynomial.swap(zero);

    EXPECT_EQ(polynomial.degree(), -1);
    EXPECT_EQ(zero.degree(), 2);
    EXPECT_DOUBLE_EQ(zero.evaluate(2.0), 27.0);
}

TEST(PolynomialTest, EvaluateUsesAllCoefficients) {
    const double coeff[]{3.0, 2.0, 5.0};
    const Polynomial polynomial(coeff, 2);

    EXPECT_DOUBLE_EQ(polynomial.evaluate(0.0), 3.0);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 27.0);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(-1.0), 6.0);
}

TEST(PolynomialTest, AdditionMatchesAssignmentExample) {
    const double first[]{1.0, 2.0};
    const double second[]{0.0, 1.0, 1.0};
    Polynomial polynomial(first, 1);
    const Polynomial other(second, 2);
    polynomial.add(other);

    EXPECT_EQ(polynomial.degree(), 2);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(0.0), 1.0);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 11.0);
    EXPECT_DOUBLE_EQ(other.evaluate(2.0), 6.0);
}

TEST(PolynomialTest, AddingOppositesRemovesAllCoefficients) {
    const double first[]{3.0, 2.0, 5.0};
    const double second[]{-3.0, -2.0, -5.0};
    Polynomial polynomial(first, 2);
    const Polynomial other(second, 2);
    polynomial.add(other);

    EXPECT_EQ(polynomial.degree(), -1);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 0.0);
}

TEST(PolynomialTest, AdditionSupportsZeroAndSelf) {
    const double coeff[]{3.0, 2.0, 5.0};
    const Polynomial other(coeff, 2);
    Polynomial polynomial;
    polynomial.add(other);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 27.0);

    const Polynomial zero;
    polynomial.add(zero);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 27.0);

    polynomial.add(polynomial);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 54.0);
}

TEST(PolynomialTest, MultiplicationMatchesAssignmentExample) {
    const double first[]{1.0, 2.0};
    const double second[]{0.0, 1.0, 1.0};
    Polynomial polynomial(first, 1);
    const Polynomial other(second, 2);
    polynomial.mul(other);

    EXPECT_EQ(polynomial.degree(), 3);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(0.0), 0.0);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 30.0);
    EXPECT_DOUBLE_EQ(other.evaluate(2.0), 6.0);
}

TEST(PolynomialTest, MultiplicationByZeroReleasesPreviousBuffer) {
    const double coeff[]{3.0, 2.0, 5.0};
    Polynomial polynomial(coeff, 2);
    Polynomial zero;
    polynomial.mul(zero);

    EXPECT_EQ(polynomial.degree(), -1);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 0.0);
    const Polynomial other(coeff, 2);
    zero.mul(other);
    EXPECT_EQ(zero.degree(), -1);
}

TEST(PolynomialTest, SelfMultiplicationSquaresPolynomial) {
    const double coeff[]{1.0, 2.0};
    Polynomial polynomial(coeff, 1);
    polynomial.mul(polynomial);

    EXPECT_EQ(polynomial.degree(), 2);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(0.0), 1.0);
    EXPECT_DOUBLE_EQ(polynomial.evaluate(2.0), 25.0);
}

TEST(PolynomialTest, PrintHandlesSignsMissingTermsAndConstants) {
    const double coeff[]{3.0, 0.0, -2.5};
    const Polynomial polynomial(coeff, 2);
    std::ostringstream out;
    polynomial.print(out);
    EXPECT_EQ(out.str(), "-2.5*x^2 + 3");

    const double constant[]{-7.0};
    const Polynomial number(constant, 0);
    std::ostringstream constant_out;
    number.print(constant_out);
    EXPECT_EQ(constant_out.str(), "-7");
}

TEST(PolynomialTest, BisectionFindsPositiveAndNegativeRoots) {
    const double coeff[]{-2.0, 0.0, 1.0};
    const Polynomial polynomial(coeff, 2);
    const double positive = polynomial.find_root(1.0, 2.0);
    const double negative = polynomial.find_root(-2.0, -1.0);

    EXPECT_NEAR(positive, std::sqrt(2.0), 1e-9);
    EXPECT_NEAR(negative, -std::sqrt(2.0), 1e-9);
    EXPECT_LT(std::abs(polynomial.evaluate(positive)), 1e-9);
    EXPECT_LT(std::abs(polynomial.evaluate(negative)), 1e-9);
}

TEST(PolynomialTest, BisectionAcceptsEndpointRoots) {
    const double coeff[]{-1.0, 1.0};
    const Polynomial polynomial(coeff, 1);

    EXPECT_DOUBLE_EQ(polynomial.find_root(1.0, 3.0), 1.0);
    EXPECT_DOUBLE_EQ(polynomial.find_root(-1.0, 1.0), 1.0);
}

TEST(PolynomialTest_Throw, BisectionRejectsInvalidBoundaryOrder) {
    const double coeff[]{-2.0, 0.0, 1.0};
    const Polynomial polynomial(coeff, 2);

    EXPECT_THROW(polynomial.find_root(2.0, 1.0), std::invalid_argument);
    EXPECT_THROW(polynomial.find_root(1.0, 1.0), std::invalid_argument);
}

TEST(PolynomialTest_Throw, BisectionRejectsSameSigns) {
    const double positive[]{1.0, 0.0, 1.0};
    const double negative[]{-1.0, 0.0, -1.0};
    const Polynomial first(positive, 2);
    const Polynomial second(negative, 2);

    EXPECT_THROW(first.find_root(-1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(second.find_root(-1.0, 1.0), std::invalid_argument);
}

TEST(PolynomialTest_Throw, BisectionReportsUnreachableAccuracy) {
    const double scale = std::ldexp(1.0, 512);
    const double coeff[]{-2.0 * scale, 0.0, scale};
    const Polynomial polynomial(coeff, 2);

    EXPECT_THROW(polynomial.find_root(1.0, 2.0), std::runtime_error);
}

}
