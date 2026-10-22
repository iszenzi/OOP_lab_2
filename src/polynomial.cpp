#include "lab02/polynomial.hpp"

#include <algorithm>
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace lab02 {

Polynomial::Polynomial() : coeff_(nullptr), degree_(-1) {}

Polynomial::Polynomial(const double* coeff, int degree) : coeff_(nullptr), degree_(degree) {
    while (degree_ >= 0 && coeff[degree_] == 0.0) {
        --degree_;
    }

    if (degree_ == -1) {
        return;
    }

    coeff_ = new double[degree_ + 1];

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

void Polynomial::trim() {
    int new_degree = degree_;

    while (new_degree >= 0 && coeff_[new_degree] == 0.0) {
        --new_degree;
    }

    if (new_degree == degree_) {
        return;
    }

    if (new_degree == -1) {
        delete[] coeff_;
        coeff_ = nullptr;
        degree_ = -1;
        return;
    }

    double* new_coeff = new double[new_degree + 1];

    for (int i = 0; i <= new_degree; ++i) {
        new_coeff[i] = coeff_[i];
    }

    delete[] coeff_;
    coeff_ = new_coeff;
    degree_ = new_degree;
}

double Polynomial::evaluate(const double x) const {
    double res{0};
    int i{degree_};
    while (i >= 0) {
        res = res * x + coeff_[i];
        --i;
    }

    return res;
}

void Polynomial::add(const Polynomial& other) {
    if (other.degree_ == -1) {
        return;
    }

    const int result_degree = std::max(other.degree_, degree_);
    Polynomial result;
    result.degree_ = result_degree;
    result.coeff_ = new double[result_degree + 1]{};

    for (int i = 0; i <= result_degree; ++i) {
        if (i <= degree_) {
            result.coeff_[i] += coeff_[i];
        }

        if (i <= other.degree_) {
            result.coeff_[i] += other.coeff_[i];
        }
    }

    result.trim();
    swap(result);
}

void Polynomial::mul(const Polynomial& other) {
    if (degree_ == -1 || other.degree_ == -1) {
        delete[] coeff_;
        coeff_ = nullptr;
        degree_ = -1;
        return;
    }

    Polynomial result;
    result.degree_ = degree_ + other.degree_;
    result.coeff_ = new double[result.degree_ + 1]{};

    for (int i = 0; i <= degree_; i++) {
        for (int j = 0; j <= other.degree_; j++) {
            result.coeff_[i + j] += coeff_[i] * other.coeff_[j];
        }
    }

    result.trim();
    swap(result);
}

void Polynomial::print(std::ostream& out) const {
    if (degree_ == -1) {
        out << 0;
        return;
    }
    bool first = true;

    for (int i = degree_; i >= 0; --i) {
        if (coeff_[i] == 0.0) {
            continue;
        }

        if (first) {
            if (coeff_[i] < 0) {
                out << '-';
            }
            first = false;
        } else {
            if (coeff_[i] < 0) {
                out << " - ";
            } else {
                out << " + ";
            }
        }

        out << std::abs(coeff_[i]);

        if (i == 1) {
            out << "*x";
        } else if (i > 1) {
            out << "*x^" << i;
        }
    }
}

double Polynomial::find_root(double a, double b) const {
    if (!(a < b)) {
        throw std::invalid_argument("Левая граница должна быть меньше правой");
    }

    double result_a = evaluate(a);
    double result_b = evaluate(b);
    constexpr double epsilon = 1e-9;
    constexpr int iterations = 1000;

    if (std::abs(result_a) < epsilon) {
        return a;
    }

    if (std::abs(result_b) < epsilon) {
        return b;
    }

    if ((result_a < 0.0) == (result_b < 0.0)) {
        throw std::invalid_argument("Значения многочлена на концах отрезка должны иметь разные знаки");
    }

    for (int i = 0; i < iterations; ++i) {
        const double middle = a + (b - a) / 2.0;
        const double result_middle = evaluate(middle);

        if (std::abs(result_middle) < epsilon) {
            return middle;
        }

        if ((result_a < 0.0) != (result_middle < 0.0)) {
            b = middle;
        } else {
            a = middle;
            result_a = result_middle;
        }
    }
    throw std::runtime_error("Не удалось найти корень с требуемой точностью");
}
}  // namespace lab02
