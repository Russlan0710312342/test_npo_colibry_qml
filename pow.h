#pragma once
#include "rational.h"  // Добавляем включение заголовка с определением Rational

template<typename T>
T IntegerPow(T base, unsigned exponent) {
    T result = 1;
    while (exponent) {
        if (exponent & 1)
            result *= base;
        exponent >>= 1;
        base *= base;
    }
    return result;
}

inline Rational Pow(Rational base, Rational exponent) {
    if (exponent.GetDenominator() != 1) {
        throw std::invalid_argument("Fractional exponents not supported");
    }
    int exp = exponent.GetNumerator();
    if (exp >= 0) {
        return Rational(
            IntegerPow(base.GetNumerator(), exp),
            IntegerPow(base.GetDenominator(), exp)
            );
    } else {
        return Rational(
            IntegerPow(base.GetDenominator(), -exp),
            IntegerPow(base.GetNumerator(), -exp)
            );
    }
}
