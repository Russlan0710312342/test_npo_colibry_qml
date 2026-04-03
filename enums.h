#pragma once

enum class Operation {
    MULTIPLICATION,
    DIVISION,
    SUBTRACTION,
    ADDITION,
    POWER,
    PERCENT
};

enum class ControlKey {
    EQUALS,     // Кнопка "Равно".
    CLEAR,      // Кнопка "C".
    PLUS_MINUS, // Кнопка "+-".
    BACKSPACE,  // Кнопка "Стереть последний символ".
    EXTRA_KEY,  // Дополнительная экстра-кнопка.
};

enum class ControllerType {
    UINT8_T,
    INT,
    INT64_T,
    SIZE_T,
    DOUBLE,
    FLOAT,
    RATIONAL,
};
