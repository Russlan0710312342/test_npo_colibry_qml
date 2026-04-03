#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "controller.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Controller double_controller;
    engine.rootContext()->setContextProperty("controller", &double_controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("test_npo_colibry_qml", "Main");

    return QCoreApplication::exec();
}
