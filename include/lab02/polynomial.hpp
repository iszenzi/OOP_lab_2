#pragma once
#include <iosfwd>
namespace lab02 {

class Polynomial {
    double* coeff_;
    int degree_;

    void trim();

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

    double evaluate(const double x) const;
    void add(const Polynomial& other);
    void mul(const Polynomial& other);
    void print(std::ostream& out) const;
};

}  // namespace lab02
