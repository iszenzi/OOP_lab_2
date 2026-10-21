#pragma once

namespace lab02 {

class Polynomial {
    double* coeff_;
    int degree_;

public:
    Polynomial();

    Polynomial(const double* coeff, int degree);

    ~Polynomial();

    int degree() const {
        return degree_;
    }
};
}  // namespace lab02
