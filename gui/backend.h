#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class Backend : public QObject {
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
    explicit Backend(QObject *parent = nullptr);

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

signals:

    void inputVideoFileNameChanged();

    void outputVideoFileNameChanged();

    void inputSubtitleFileNameChanged();

    void outputSubtitleFileNameChanged();

private:
    QString m_inputVideoFileName;
    QString m_outputVideoFileName;
    QString m_inputSubtitleFileName;
    QString m_outputSubtitleFileName;

};

#endif // BACKEND_H
