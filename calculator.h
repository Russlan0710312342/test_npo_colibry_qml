#pragma once
#include <optional>
#include <string>
#include <cmath>
#include <type_traits>
#include "pow.h"
#include "rational.h"

using Error = std::string;

template<typename Number>
class Calculator {
public:
    void Set(Number n) { n_ = n; }
    Number GetNumber() const { return n_; }
    std::string GetNumberRepr() const { return std::to_string(n_); }

    bool GetHasMem() const { return mem_.has_value(); }

    std::optional<Error> Add(Number n) {
        n_ += n;
        return std::nullopt;
    }

    std::optional<Error> Sub(Number n) {
        n_ -= n;
        return std::nullopt;
    }

    std::optional<Error> Mul(Number n) {
        n_ *= n;
        return std::nullopt;
    }

    std::optional<Error> Div(Number n) {
        if constexpr (std::is_same_v<Number, float> || std::is_same_v<Number, double>) {
            n_ /= n;
            return std::nullopt;
        }
        if (n == 0) {
            return "Division by zero";
        }
        n_ /= n;
        return std::nullopt;
    }

    std::optional<Error> Percent() {
        n_ /= 100;
        return std::nullopt;
    }

    std::optional<Error> Pow(Number exponent) {
        if constexpr (std::is_same_v<Number, float> || std::is_same_v<Number, double>) {
            n_ = std::pow(n_, exponent);
            return std::nullopt;
        }
        if(n_ == 0 && exponent == 0) {
            return "Zero power to zero";
        }
        if constexpr (std::is_integral_v<Number> ){
            if (exponent < 0) {
                return "Integer negative power";
            }
            n_ = IntegerPow(n_, exponent);
            return std::nullopt;
        }
        if constexpr (std::is_same_v<Number, Rational>) {
            if (exponent.GetDenominator() != 1) {
                return "Fractional power is not supported";
            }
            n_ = ::Pow(n_, exponent);
        }
        return std::nullopt;
    }

    void Save() { mem_ = n_; }
    void Load() { if (mem_) n_ = *mem_; }

private:
    Number n_{};
    std::optional<Number> mem_;
};
