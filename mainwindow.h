#pragma once

#include <functional>
#include <optional>
#include <qobject.h>
#include <string>
#include "enums.h"



class MainWindow  {

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Методы для установки текста
    void SetInputText(const std::string& text);
    void SetErrorText(const std::string& text);
    void SetFormulaText(const std::string& text);
    void SetMemText(const std::string& text);
    void SetExtraKey(const std::optional<std::string>& key);

    // Методы для установки колбэков
    void SetDigitKeyCallback(std::function<void(int)> cb);
    void SetProcessOperationKeyCallback(std::function<void(Operation)> cb);
    void SetProcessControlKeyCallback(std::function<void(ControlKey)> cb);
    void SetControllerCallback(std::function<void(ControllerType)> cb);

private slots:
    void OnControllerTypeChanged();

private:
    // Колбэк-функции
    std::function<void(int)> digit_cb_;
    std::function<void(Operation)> operation_cb_;
    std::function<void(ControlKey)> control_cb_;
    std::function<void(ControllerType)> controller_cb_;

    void ConnectSignals();
    ControllerType GetCurrentControllerType() const;
};
