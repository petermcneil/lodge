#ifndef LODGE_BACKEND_H
#define LODGE_BACKEND_H

#include <QObject>
#include <QString>
#include <memory>

// Forward declarations to avoid including full headers
namespace lodge {
    class video;
    class subtitle;
}

/**
 * @brief Backend controller for Lodge video steganography GUI
 *
 * Provides Qt/QML interface to the core lodge library for encoding
 * and decoding steganographic subtitles in video files.
 */
class Backend : public QObject {
    Q_OBJECT

    // QML-accessible properties
    Q_PROPERTY(QString outputSubtitle READ outputSubtitle NOTIFY outputSubtitleChanged)
    Q_PROPERTY(bool hasVlc READ hasVlc CONSTANT)

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend() override;

    // Disable copy and move (Qt objects shouldn't be copied)
    Backend(const Backend&) = delete;
    Backend& operator=(const Backend&) = delete;
    Backend(Backend&&) = delete;
    Backend& operator=(Backend&&) = delete;

    // QML-invokable methods
    Q_INVOKABLE void encodeVideoFile(const QString &inputSubtitle,
                                      const QString &inputVideo,
                                      const QString &outputVideo);

    Q_INVOKABLE void decodeVideoFile(const QString &inputVideo,
                                      const QString &outputSubtitle);

    Q_INVOKABLE bool doesVideoContainSteg(const QString &videoPath);

    Q_INVOKABLE void playVideoWithSubs();

    Q_INVOKABLE void playVideo(const QString &videoPath);

    // Property accessors
    QString outputSubtitle() const { return m_outputSubtitle; }
    bool hasVlc() const { return !m_vlcPath.isEmpty(); }

signals:
    void subtitleFileWritten();
    void outputSubtitleChanged();
    void encodingProgress(int percent, const QString &status);
    void error(const QString &title, const QString &message);
    void info(const QString &title, const QString &message);

private:
    QString findVlcPath();
    bool fileExists(const QString& path) const;

    // Member variables with m_ prefix (Qt convention)
    QString m_vlcPath;
    QString m_outputSubtitle;

    QString m_inputVideo;
    QString m_outputVideo;
    QString m_inputSubtitle;

    // Use smart pointers for automatic memory management
    std::unique_ptr<lodge::video> m_video;
    std::unique_ptr<lodge::subtitle> m_subtitle;
};

#endif // LODGE_BACKEND_H
