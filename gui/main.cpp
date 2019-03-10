#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <VideoFile.h>
#include <backend.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

        qmlRegisterType<Backend>("lodge.backend", 1, 0, "Backend");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
