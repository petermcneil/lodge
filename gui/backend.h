#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore>
#include <QProgressDialog>
#include <QObject>
#include <QString>
#include <QApplication>
#include <video.h>

using namespace lodge;
class backend : public QObject {
    Q_OBJECT
public:
    explicit backend(QObject *parent = nullptr);

    Q_INVOKABLE void encodeVideoFile(const QString &inputSubtitle, const QString &inputVideo, const QString &outputVideo);

    Q_INVOKABLE void decodeVideoFile(const QString &outputSubtitle, const QString &inputVideo);

    Q_INVOKABLE bool doesVideoContainSteg(const QString &videoPath);

    Q_INVOKABLE QString getOutputSubtitle();

    Q_INVOKABLE void playVideoWithSubs();

    Q_INVOKABLE void playVideo(const QString &videoPath);

signals:

    void subtitleFileWritten();

private:
    const QString vlcPath = QString("/Applications/VLC.app/Contents/MacOS/VLC");
    bool vlc = false;

    std::string input_video;
    std::string output_video;
    std::string input_sub;
    std::string output_sub;

    lodge::video * video = nullptr;
    lodge::subtitle * subtitle = nullptr;

    bool fileExists(const QString& path);
};

#endif // BACKEND_H
