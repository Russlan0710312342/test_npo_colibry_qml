#pragma once

#include <fstream>
#include <numeric>
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <compare>
#include <iostream>
#include <ostream>

class Rational {
public:
    Rational() : numerator_(0), denominator_(1) {}

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        if (denominator_ == 0) {
            std::abort();
        }
        Reduction();
    }

    Rational(int value) : numerator_(value), denominator_(1) {}

    // Явно объявляем конструктор копирования
    Rational(const Rational&) = default;

    Rational operator-() const {
        return Rational{-numerator_, denominator_};
    }

    Rational operator+() const {
        return *this;
    }
    friend std::istream& operator>>(std::istream& is, Rational& r);
    friend std::ostream& operator<<(std::ostream& os, const Rational& r);

    Rational& operator=(const int value){
        numerator_=value;
        denominator_=1;
        return *this;
    }
    Rational& operator=(const Rational& other){
        numerator_=other.numerator_;
        denominator_=other.denominator_;
        return *this;
    }
    // Перегрузка операции сравнения.
    // Вы научитесь этому в следующем уроке.
    int GetNumerator() const {
        return numerator_;
    }

    int GetDenominator() const {
        return denominator_;
    }
    Rational Inv() const{
        if (numerator_ == 0) {
            std::abort(); // или другое поведение при ошибке
        }
        return Rational(denominator_, numerator_);
    }

    explicit operator double() const {
        return static_cast<double>(numerator_) / denominator_;
    }


    // Напишите здесь перегрузки четырёх операций.
    Rational& operator+=(const Rational& p2) {
        numerator_ = numerator_ * p2.denominator_ + p2.numerator_ * denominator_;
        denominator_ = denominator_*p2.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator-=(const Rational& p2) {
        numerator_ = numerator_ * p2.denominator_ - p2.numerator_ * denominator_;
        denominator_ = denominator_*p2.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator*=(const Rational& p2) {
        numerator_ = numerator_ * p2.numerator_ ;
        denominator_ = denominator_*p2.denominator_;
        Reduction();
        return *this;
    }

    Rational& operator/=(const Rational& p2) {
        numerator_ = numerator_ * p2.denominator_ ;
        denominator_ = denominator_*p2.numerator_;
        Reduction();
        return *this;
    }


    // Добавляем перегрузки арифметических операторов
    Rational operator+(const Rational& rhs) {
        Rational result = {this->GetNumerator(), this->GetDenominator()};
        result += rhs;
        return result;
    }

    Rational operator-(const Rational& rhs) {
        Rational result = {this->GetNumerator(), this->GetDenominator()};
        result -= rhs;
        return result;
    }

    Rational operator*(const Rational& rhs) {
        Rational result = {this->GetNumerator(), this->GetDenominator()};
        result *= rhs;
        return result;
    }

    Rational operator/(const Rational& rhs) {
        Rational result = {this->GetNumerator(), this->GetDenominator()};
        result /= rhs;
        return result;
    }


private:
    // Метод для приведения дроби к корректной форме.
    void Reduction() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = std::gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};

inline std::istream& operator>>(std::istream& is, Rational& r) {
    int n, d;
    char div;

    if (!(is >> n)) {
        return is;
    }

    if (!(is >> std::ws >> div)) {
        r = Rational(n, 1);
        is.clear();
        return is;
    }

    if (div != '/') {
        r = Rational(n, 1);
        is.unget();
        return is;
    }

    if (!(is >> d) || (d == 0)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    r = Rational(n, d);

    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Rational& r) {
    using namespace std::literals;
    if (r.denominator_ == 1) {
        os << r.numerator_;
    } else {
        os << r.numerator_ << " / "s << r.denominator_;
    }
    return os;
}

// Равенство
inline bool operator==(const Rational& r1, const Rational& r2) {
    int64_t c1 = static_cast<int64_t>(r1.GetNumerator()) * r2.GetDenominator();
    int64_t c2 = static_cast<int64_t>(r2.GetNumerator()) * r1.GetDenominator();
    return c1 == c2;
}

// Неравенство
inline bool operator!=(const Rational& r1, const Rational& r2) {
    return !(r1 == r2);
}

// Меньше
inline bool operator<(const Rational& r1, const Rational& r2) {
    std::int64_t c1 = static_cast<std::int64_t>(r1.GetNumerator()) * r2.GetDenominator();
    std::int64_t c2 = static_cast<std::int64_t>(r2.GetNumerator()) * r1.GetDenominator();
    return c1 < c2;
}

// Больше
inline bool operator>(const Rational& r1, const Rational& r2) {
    return r2 < r1;
}

// Меньше или равно
inline bool operator<=(const Rational& r1, const Rational& r2) {
    return !(r1 > r2);
}

// Больше или равно
inline bool operator>=(const Rational& r1, const Rational& r2) {
    return !(r1 < r2);
}
