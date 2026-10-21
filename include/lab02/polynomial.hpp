#pragma once

namespace lab02 {

class Polynomial {
    double* coeff_;
    int degree_;

public:
    Polynomial();
    Polynomial(const double* coeff, int degree);
    Polynomial(const Polynomial& other);
    Polynomial(Polynomial&& other) noexcept;
    ~Polynomial();

    Polynomial& operator=(Polynomial other);
    void swap(Polynomial& other) noexcept;

    int degree() const {
        return degree_;
    }
};

}  // namespace lab02
