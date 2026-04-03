#pragma once
#include <QObject>
#include <QString>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <cmath>

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void handleButtonClick(const QString &text) {
        if (text == "C") clearAll();
        else if (text == "=") evaluateExpression();
        else inputChar(text);
    }

signals:
    void updateInput(const QString& str);
    void updateFormula(const QString& str);

private:
    QString currentInput;   // текущий набор цифр
    QString currentFormula; // вся формула
    bool lastWasOperator = false;

    void clearAll() {
        currentInput.clear();
        currentFormula.clear();
        lastWasOperator = false;
        emit updateInput(currentInput);
        emit updateFormula(currentFormula);
    }

    void inputChar(const QString &text) {
        if (text[0].isDigit()) {
            // нельзя вводить несколько ведущих нулей
            if (!(currentInput == "0" && text == "0")) {
                if (currentInput == "0" && text != ".") currentInput.clear();
                currentInput += text;
                lastWasOperator = false;
            }
        } else if (text == ".") {
            if (!currentInput.contains(".")) {
                if (currentInput.isEmpty()) currentInput = "0";
                currentInput += ".";
            }
        } else if (text == "+/-") {
            if (!currentInput.isEmpty()) {
                if (currentInput.startsWith("-")) currentInput = currentInput.mid(1);
                else currentInput = "-" + currentInput;
            }
        } else if (text == "%") {
            if (!currentInput.isEmpty()) {
                double val = currentInput.toDouble() / 100.0;
                currentInput = QString::number(val, 'g', 15);
            }
        } else if (text == "(" || text == ")") {
            currentFormula += currentInput + text;
            currentInput.clear();
            lastWasOperator = false;
        } else { // операторы + - x ÷ ^
            if (lastWasOperator) return; // два оператора подряд запрещены
            if (!currentInput.isEmpty()) {
                currentFormula += currentInput;
                currentInput.clear();
            }
            currentFormula += text;
            lastWasOperator = true;
        }

        emit updateInput(currentInput);
        emit updateFormula(currentFormula + currentInput);
    }

    void evaluateExpression() {
        if (!currentInput.isEmpty()) currentFormula += currentInput;

        try {
            double result = computeExpression(currentFormula.toStdString());
            currentInput = QString::number(result, 'g', 15);
            currentFormula.clear();
            lastWasOperator = false;
        } catch (const std::runtime_error& e) {
            currentInput = "Error";
        } catch (...) {
            currentInput = "Error";
        }

        emit updateInput(currentInput);
        emit updateFormula(currentFormula);
    }

    double computeExpression(const std::string &expr) {
        std::istringstream in(expr);
        return parseExpression(in);
    }

    double parseExpression(std::istringstream &in) {
        double result = parseTerm(in);
        while (true) {
            char op = 0;
            if (!(in >> op)) break;
            if (op != '+' && op != '-') {
                in.putback(op);
                break;
            }
            double term = parseTerm(in);
            if (op == '+') result += term;
            else result -= term;
        }
        return result;
    }

    double parseTerm(std::istringstream &in) {
        double result = parseFactor(in);
        while (true) {
            char op = 0;
            if (!(in >> op)) break;
            if (op != 'x' && op != '÷') {
                in.putback(op);
                break;
            }
            double factor = parseFactor(in);
            if (op == 'x') result *= factor;
            else {
                if (factor == 0) throw std::runtime_error("Division by zero");
                result /= factor;
            }
        }
        return result;
    }

    double parseFactor(std::istringstream &in) {
        double result = 0;
        char ch;
        if (!(in >> ch)) throw std::runtime_error("Unexpected end");

        if (ch == '(') {
            result = parseExpression(in);
            if (!(in >> ch) || ch != ')') throw std::runtime_error("Expected ')'");
        } else if (ch == '-') {
            result = -parseFactor(in);
        } else {
            in.putback(ch);
            if (!(in >> result)) throw std::runtime_error("Invalid number");
        }

        char next = in.peek();
        if (next == '^') {
            in.get();
            double exponent = parseFactor(in);
            result = std::pow(result, exponent);
        }

        return result;
    }
};