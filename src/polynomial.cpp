#include "polynomial.hpp"

namespace lab02 {

Polynomial::Polynomial() : coeff_(nullptr), degree_(-1) {}
Polynomial::Polynomial(const double* coeff, int degree) : coeff_(new double[degree + 1]), degree_(degree) {
    for (int i = 0; i <= degree; ++i) {
        coeff_[i] = coeff[i];
    }
}
Polynomial::~Polynomial() {
    delete[] coeff_;
}

}  // namespace lab02
