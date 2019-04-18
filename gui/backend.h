#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore>
#include <QProgressDialog>
#include <QObject>
#include <QString>

class backend : public QObject {
Q_OBJECT
    Q_PROPERTY(QString inputVideoFileName
                       READ
                       inputVideoFileName
                       WRITE
                       setInputVideoFileName
                       NOTIFY
                       inputVideoFileNameChanged)
    Q_PROPERTY(QString outputVideoFileName
                       READ
                       outputVideoFileName
                       WRITE
                       setOutputVideoFileName
                       NOTIFY
                       outputVideoFileNameChanged)
    Q_PROPERTY(QString inputSubtitleFileName
                       READ
                       inputSubtitleFileName
                       WRITE
                       setInputSubtitleFileName
                       NOTIFY
                       inputSubtitleFileNameChanged)
    Q_PROPERTY(QString outputSubtitleFileName
                       READ
                       outputSubtitleFileName
                       WRITE
                       setOutputSubtitleFileName
                       NOTIFY
                       outputSubtitleFileNameChanged)
public:
    explicit backend(QObject *parent = nullptr);

    QString inputVideoFileName();

    void setInputVideoFileName(const QString &videoFileName);

    QString outputVideoFileName();

    void setOutputVideoFileName(const QString &videoFileName);

    QString inputSubtitleFileName();

    void setInputSubtitleFileName(const QString &videoFileName);

    QString outputSubtitleFileName();

    void setOutputSubtitleFileName(const QString &videoFileName);

    Q_INVOKABLE void encodeVideoFile(const QString &inputSubtitle, const QString &inputVideo, const QString &outputVideo);

    Q_INVOKABLE void decodeVideoFile(const QString &outputSubtitle, const QString &inputVideo);

    Q_INVOKABLE bool doesVideoContainSteg(const QString &videoPath);

signals:

    void inputVideoFileNameChanged();

    void outputVideoFileNameChanged();

    void inputSubtitleFileNameChanged();

    void outputSubtitleFileNameChanged();

    void subtitleFileWritten();

private:
    QString m_inputVideoFileName;
    QString m_outputVideoFileName;
    QString m_inputSubtitleFileName;
    QString m_outputSubtitleFileName;

    std::string input_video;
    std::string output_video;
    std::string input_sub;
    std::string output_sub;

};

#endif // BACKEND_H
