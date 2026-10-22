#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>

#include "lab02/polynomial.hpp"

namespace {

constexpr int kMaxDegree = 100;

double ReadNumber() {
    double value{};
    if (!(std::cin >> value) || !std::isfinite(value)) {
        throw std::invalid_argument("Введите конечное число");
    }

    if (!std::cin.eof()) {
        const int next = std::cin.peek();
        if (next != std::char_traits<char>::eof() && !std::isspace(static_cast<unsigned char>(next))) {
            throw std::invalid_argument("После числа должен быть пробел или перевод строки");
        }
    }
    return value;
}

lab02::Polynomial ReadPolynomial() {
    std::cout << "Введите степень от 0 до " << kMaxDegree << ": ";
    const double input_degree = ReadNumber();
    if (input_degree < 0 || input_degree > kMaxDegree || std::floor(input_degree) != input_degree) {
        throw std::invalid_argument("Степень должна быть целым числом от 0 до 100");
    }

    const int degree = static_cast<int>(input_degree);
    double coeff[kMaxDegree + 1]{};
    std::cout << "Введите " << degree + 1 << " коэффициентов от свободного члена к старшему: ";
    for (int i = 0; i <= degree; ++i) {
        coeff[i] = ReadNumber();
    }
    return lab02::Polynomial(coeff, degree);
}

}

int main() {
    lab02::Polynomial polynomial;
    std::cout << "Лабораторная работа 2. Вариант 11\n" << std::setprecision(12);

    while (true) {
        std::cout
            << "\n1. Ввести многочлен\n"
            << "2. Напечатать многочлен\n"
            << "3. Показать степень\n"
            << "4. Вычислить значение\n"
            << "5. Прибавить многочлен\n"
            << "6. Умножить на многочлен\n"
            << "7. Найти корень на отрезке\n"
            << "0. Выход\n"
            << "Ваш выбор: ";

        try {
            const double input_choice = ReadNumber();
            if (input_choice < 0 || input_choice > 7 || std::floor(input_choice) != input_choice) {
                throw std::invalid_argument("Выберите целое число от 0 до 7");
            }

            const int choice = static_cast<int>(input_choice);
            switch (choice) {
                case 0:
                    std::cout << "Завершение программы\n";
                    return 0;
                case 1:
                    polynomial = ReadPolynomial();
                    std::cout << "Многочлен сохранён\n";
                    break;
                case 2:
                    polynomial.print(std::cout);
                    std::cout << '\n';
                    break;
                case 3:
                    std::cout << "Степень: " << polynomial.degree() << '\n';
                    break;
                case 4: {
                    std::cout << "Введите x: ";
                    const double x = ReadNumber();
                    std::cout << "Значение: " << polynomial.evaluate(x) << '\n';
                    break;
                }
                case 5:
                case 6: {
                    const lab02::Polynomial other = ReadPolynomial();
                    if (choice == 5) {
                        polynomial.add(other);
                    } else {
                        polynomial.mul(other);
                    }
                    polynomial.print(std::cout);
                    std::cout << '\n';
                    break;
                }
                case 7: {
                    std::cout << "Введите левую и правую границы: ";
                    const double a = ReadNumber();
                    const double b = ReadNumber();
                    const double root = polynomial.find_root(a, b);
                    std::cout << "Корень: " << root << '\n';
                    break;
                }
            }
        } catch (const std::invalid_argument& error) {
            if (std::cin.fail() && (std::cin.eof() || std::cin.bad())) {
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: " << error.what() << '\n';
        } catch (const std::runtime_error& error) {
            std::cout << "Ошибка: " << error.what() << '\n';
        } catch (const std::bad_alloc&) {
            std::cerr << "Недостаточно памяти\n";
            return 1;
        }
    }

    std::cout << "Завершение программы\n";
    return 0;
}
