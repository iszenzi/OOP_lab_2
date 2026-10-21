#include "lab02/polynomial.hpp"

#include <utility>

namespace lab02 {

Polynomial::Polynomial() : coeff_(nullptr), degree_(-1) {}

Polynomial::Polynomial(const double* coeff, int degree) : coeff_(new double[degree + 1]), degree_(degree) {
    for (int i = 0; i <= degree_; ++i) {
        coeff_[i] = coeff[i];
    }
}

Polynomial::Polynomial(const Polynomial& other) : coeff_(nullptr), degree_(other.degree_) {
    if (degree_ != -1) {
        coeff_ = new double[degree_ + 1];

        for (int i = 0; i <= degree_; ++i) {
            coeff_[i] = other.coeff_[i];
        }
    }
}

Polynomial::Polynomial(Polynomial&& other) noexcept : coeff_(other.coeff_), degree_(other.degree_) {
    other.coeff_ = nullptr;
    other.degree_ = -1;
}

Polynomial::~Polynomial() {
    delete[] coeff_;
}

Polynomial& Polynomial::operator=(Polynomial other) {
    swap(other);
    return *this;
}

void Polynomial::swap(Polynomial& other) noexcept {
    std::swap(coeff_, other.coeff_);
    std::swap(degree_, other.degree_);
}

}  // namespace lab02
