#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>

#include "Backend.h"

#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    // Enable spdlog for debugging
    spdlog::set_level(spdlog::level::debug);

    // Qt 6 handles high DPI automatically - no need for setAttribute
    QGuiApplication app(argc, argv);

    // Set application metadata
    app.setOrganizationName("Lodge");
    app.setOrganizationDomain("lodge.video");
    app.setApplicationName("Lodge Video Steganography");
    app.setApplicationVersion("1.0.0");

    // Set application icon if available
    app.setWindowIcon(QIcon(":/resources/logo_transparent.png"));

    // Register Backend type for QML
    qmlRegisterType<Backend>("lodge.backend", 1, 0, "Backend");

    // Create QML engine
    QQmlApplicationEngine engine;

    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    // Connect error handler
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() {
            spdlog::error("Failed to load QML");
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    // Check if loading was successful
    if (engine.rootObjects().isEmpty()) {
        spdlog::error("No root objects created - QML loading failed");
        return -1;
    }

    spdlog::info("Lodge GUI started successfully");
    return app.exec();
}
