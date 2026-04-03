#pragma once

#include <qdebug.h>
#include <string>
#include <sstream>
#include "calculator.h"
#include "mainwindow.h"
#include "rational.h"

template<class T>
std::string ToString(T number) {
    std::ostringstream tmp_str;
    tmp_str << number;
    return tmp_str.str();
}

template<class T>
inline T FromString(const std::string& number) {
    std::istringstream tmp_str{number};
    T result{};
    tmp_str >> result;
    return result;
}

inline std::string ToString(std::uint8_t number) {
    std::ostringstream tmp_str;
    tmp_str << +number;
    return tmp_str.str();
}

template<>
inline std::uint8_t FromString<std::uint8_t>(const std::string& number) {
    std::istringstream tmp_str{number};
    unsigned result{};
    tmp_str >> result;
    return static_cast<std::uint8_t>(result);
}

class Controller : public QObject
{
    Q_OBJECT
    using Number = double;
public:
    Controller() {}

signals:
    void updateInput(const QString& str);
    void updateFormula(const QString& str);
    void updateExtraKey(const std::optional<std::string>& key);

public slots:
    void handleButtonClick(const QString &text)
    {
        qDebug() << "Button pressed:" << text;

        // Цифры 0-9
        if (text.size() == 1 && text[0] >= '0' && text[0] <= '9') {
            int digit = text[0].toLatin1() - '0';
            PressDigitKey(digit);
            return;
        }
        // Операции
        else if (text == "+") {
            ProcessOperationKey(Operation::ADDITION);
            return;
        }
        else if (text == "-") {
            ProcessOperationKey(Operation::SUBTRACTION);
            return;
        }
        else if (text == "%") {
            ProcessOperationKey(Operation::PERCENT);
            return;
        }
        else if (text == "x") {
            ProcessOperationKey(Operation::MULTIPLICATION);
            return;
        }
        else if (text == "÷") {
            ProcessOperationKey(Operation::DIVISION);
            return;
        }
        else if (text == "^") {
            ProcessOperationKey(Operation::POWER);
            return;
        }
        // Управляющие кнопки
        else if (text == "=") {
            ProcessControlKey(ControlKey::EQUALS);
            return;
        }
        else if (text == "C") {
            ProcessControlKey(ControlKey::CLEAR);
            return;
        }
        else if (text == "+/-") {
            ProcessControlKey(ControlKey::PLUS_MINUS);
            return;
        }
        else if (text == ".") {
            ProcessControlKey(ControlKey::EXTRA_KEY);
            return;
        }
        else if (text == "(" || text == ")") {
            AddChar(text[0].toLatin1());
            return;
        }
    }

private:
    void PressDigitKey(int digit) {
        AddChar(digit + '0');
    }

    void ProcessOperationKey(Operation operation) {
        switch(operation) {
        case Operation::ADDITION:
            OnOperation([this](Number x){return calculator_.Add(x);}, " + ");
            break;
        case Operation::SUBTRACTION:
            OnOperation([this](Number x){return calculator_.Sub(x);}, " − ");
            break;
        case Operation::MULTIPLICATION:
            OnOperation([this](Number x){return calculator_.Mul(x);}, " × ");
            break;
        case Operation::DIVISION:
            OnOperation([this](Number x){return calculator_.Div(x);}, " ÷ ");
            break;
        case Operation::POWER:
            OnOperation([this](Number x){return calculator_.Pow(x);}, " ^ ");
            break;
        case Operation::PERCENT:
            OnOperation([this](Number x){return calculator_.Percent();}, " % ");
            break;
        }
    }

    void ProcessControlKey(ControlKey key) {
        switch(key) {
        case ControlKey::EQUALS:
        {
            if (!operation_) {
                return;
            }

            std::string formula;
            if(operation_name_ != " % ") {
                formula = ToString(calculator_.GetNumber()) + operation_name_ + ToString(active_number_) + " = ";
            }
            else
            {
                formula = ToString(calculator_.GetNumber()) + operation_name_ +  " = ";
            }
            qDebug() << "operation_name_ : " << operation_name_;

            auto error = operation_(active_number_);
            if (error.has_value()) {
                SetErrorInView(error.value());
                break;
            }

            SetFormulaInView(formula);
            SetInputAsNumber(calculator_.GetNumber());
            operation_ = {};
            break;
        }
        case ControlKey::CLEAR:
            SetInputAsNumber(Number{});
            SetFormulaInView("");
            operation_ = {};
            break;
        case ControlKey::PLUS_MINUS:
            if (input_as_number_) {
                active_number_ = -active_number_;
                SetInputAsNumber(active_number_);
            } else {
                if (input_.size() && input_.front() == '-') {
                    SetInputAsString(input_.substr(1));
                } else {
                    SetInputAsString("-" + input_);
                }
            }
            break;
        case ControlKey::BACKSPACE:
            if (input_.size() > 0) {
                SetInputAsString(input_.substr(0, input_.size() - 1));
            }
            break;
        case ControlKey::EXTRA_KEY:
            DoExtraAction();
            break;
        }
    }

    void OnOperation(std::function<std::optional<Error> (Number)> action, std::string text) {
        if (!operation_) {
            calculator_.Set(active_number_);
        }

        operation_name_ = text;
        operation_ = action;
        input_ = "";

        SetFormulaInView(ToString(calculator_.GetNumber()) + text);
    }

    void SetInputAsString(const std::string& new_input) {
        input_as_number_ = false;
        input_ = new_input;
        active_number_ = FromString<Number>(input_);
        UpdateInputInView(new_input);
    }

    void SetInputAsNumber(Number number) {
        input_as_number_ = true;
        input_ = "";
        active_number_ = number;
        UpdateInputInView(ToString(active_number_));
    }

    void AddChar(char character) {
        SetInputAsString(input_ + character);
    }

    void DoExtraAction() {
        if constexpr (std::is_integral_v<Number>) {
            return;
        }
        if constexpr (std::is_same_v<Number, Rational>) {
            if (input_.size() == 0 || input_.find('/') != std::string::npos) {
                return;
            }
            AddChar('/');
        } else {
            if (input_.find('.') != std::string::npos) {
                return;
            }
            AddChar('.');
        }
    }

    void UpdateInputInView(const std::string& string) {
        updateInput(QString::fromStdString(string));
        text_ = string;
    }

    void SetErrorInView(const std::string& string) {

        updateInput(QString::fromStdString(string) );
    }

    void SetFormulaInView(const std::string& string) {
        updateFormula(QString::fromStdString(string));
        formula_ = string;
    }

    void SetMemInView(const std::string& string) {
        // view_->SetMemText(string);
        mem_text_ = string;
    }

    std::optional<std::string> GetExtraKey() const {
        if constexpr (std::is_integral_v<Number>) {
            return std::nullopt;
        }
        if constexpr (std::is_same_v<Number, Rational>) {
            return "/";
        }
        return ".";
    }

    void RevealView() {
        updateInput(QString::fromStdString(text_));
        updateFormula(QString::fromStdString(formula_));
    }

private:
    std::function<std::optional<Error>(Number number)> operation_;
    std::string operation_name_;

    Calculator<Number> calculator_;

    Number active_number_ = {};
    std::string input_;

    std::optional<Number> mem_ = {};

    bool input_as_number_ = true;

    std::string text_;
    std::string formula_;
    std::string mem_text_;
};
