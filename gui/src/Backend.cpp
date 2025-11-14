#include "Backend.h"
#include "video.h"
#include "subtitle.h"

#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>
#include <QUrl>
#include <QDir>

#include <spdlog/spdlog.h>

Backend::Backend(QObject *parent)
    : QObject(parent)
{
    m_vlcPath = findVlcPath();

    if (!m_vlcPath.isEmpty()) {
        spdlog::info("VLC found at: {}", m_vlcPath.toStdString());
    } else {
        spdlog::info("VLC not found, will use ffplay as fallback");
    }
}

Backend::~Backend() {
    // Smart pointers automatically clean up
}

QString Backend::findVlcPath() {
    // Try to find VLC in system PATH (cross-platform)
    QString vlcExecutable = QStandardPaths::findExecutable("vlc");
    if (!vlcExecutable.isEmpty()) {
        return vlcExecutable;
    }

    // Fallback to macOS-specific path if on macOS
#ifdef Q_OS_MACOS
    QString macVlcPath = "/Applications/VLC.app/Contents/MacOS/VLC";
    if (QFile::exists(macVlcPath)) {
        return macVlcPath;
    }
#endif

    return QString();
}

bool Backend::fileExists(const QString &path) const {
    QFileInfo fileInfo(path);
    return fileInfo.exists() && fileInfo.isFile();
}

void Backend::encodeVideoFile(const QString &inputSubtitle,
                               const QString &inputVideo,
                               const QString &outputVideo)
{
    try {
        // Convert file:// URLs to local paths
        QString subtitlePath = QUrl(inputSubtitle).toLocalFile();
        if (subtitlePath.isEmpty()) subtitlePath = inputSubtitle;

        QString videoPath = QUrl(inputVideo).toLocalFile();
        if (videoPath.isEmpty()) videoPath = inputVideo;

        QString outputPath = QUrl(outputVideo).toLocalFile();
        if (outputPath.isEmpty()) outputPath = outputVideo;

        // Validate inputs
        if (!fileExists(subtitlePath)) {
            emit error("File Not Found", "Input subtitle file does not exist:\n" + subtitlePath);
            return;
        }

        if (!fileExists(videoPath)) {
            emit error("File Not Found", "Input video file does not exist:\n" + videoPath);
            return;
        }

        // Store paths for later use
        m_inputSubtitle = subtitlePath;
        m_inputVideo = videoPath;
        m_outputVideo = outputPath;

        spdlog::debug("Encoding: video={}, subtitle={}, output={}",
                     videoPath.toStdString(),
                     subtitlePath.toStdString(),
                     outputPath.toStdString());

        emit encodingProgress(10, "Loading subtitle file...");

        // Create subtitle object with RAII
        m_subtitle = std::make_unique<lodge::subtitle>(
            subtitlePath.toStdString(),
            lodge::RW::READ
        );

        emit encodingProgress(30, "Initializing video encoder...");

        // Create video object
        m_video = std::make_unique<lodge::video>(
            videoPath.toStdString(),
            outputPath.toStdString(),
            m_subtitle.get()  // Pass raw pointer (video doesn't own it)
        );

        emit encodingProgress(50, "Encoding steganography...");

        // Perform encoding
        int result = m_video->write_subtitle_file();

        if (result == 0) {
            emit encodingProgress(100, "Encoding complete!");
            emit info("Success", "Subtitle successfully encoded into video:\n" + outputPath);
        } else {
            emit error("Encoding Failed",
                      "Failed to encode subtitle into video.\n"
                      "Check the console for error details.");
        }

    } catch (const std::exception &e) {
        emit error("Exception", QString("Error during encoding: %1").arg(e.what()));
        spdlog::error("Encoding exception: {}", e.what());
    }
}

void Backend::decodeVideoFile(const QString &inputVideo,
                               const QString &outputSubtitle)
{
    try {
        // Convert file:// URLs to local paths
        QString videoPath = QUrl(inputVideo).toLocalFile();
        if (videoPath.isEmpty()) videoPath = inputVideo;

        QString subtitlePath = QUrl(outputSubtitle).toLocalFile();
        if (subtitlePath.isEmpty()) subtitlePath = outputSubtitle;

        // Validate input
        if (!fileExists(videoPath)) {
            emit error("File Not Found", "Input video file does not exist:\n" + videoPath);
            return;
        }

        m_inputVideo = videoPath;
        m_outputSubtitle = subtitlePath;

        spdlog::debug("Decoding: video={}, output={}",
                     videoPath.toStdString(),
                     subtitlePath.toStdString());

        emit encodingProgress(10, "Opening video file...");

        // Create subtitle object for writing
        m_subtitle = std::make_unique<lodge::subtitle>(
            subtitlePath.toStdString(),
            lodge::RW::WRITE
        );

        emit encodingProgress(30, "Initializing video decoder...");

        // Create video object
        m_video = std::make_unique<lodge::video>(
            videoPath.toStdString(),
            m_subtitle.get()
        );

        emit encodingProgress(50, "Extracting steganography...");

        // Perform decoding
        int result = m_video->read_subtitle_file();

        if (result == 0) {
            emit encodingProgress(100, "Decoding complete!");
            emit outputSubtitleChanged();
            emit subtitleFileWritten();
            emit info("Success", "Subtitle successfully extracted to:\n" + subtitlePath);
        } else {
            emit error("Decoding Failed",
                      "Failed to extract subtitle from video.\n"
                      "Check the console for error details.");
        }

    } catch (const std::exception &e) {
        emit error("Exception", QString("Error during decoding: %1").arg(e.what()));
        spdlog::error("Decoding exception: {}", e.what());
    }
}

bool Backend::doesVideoContainSteg(const QString &videoPath) {
    try {
        // Convert file:// URL to local path
        QString localPath = QUrl(videoPath).toLocalFile();
        if (localPath.isEmpty()) localPath = videoPath;

        if (!fileExists(localPath)) {
            spdlog::warn("Video file does not exist: {}", localPath.toStdString());
            return false;
        }

        m_inputVideo = localPath;

        // Create temporary video object to check for steganography
        auto tempVideo = std::make_unique<lodge::video>(
            localPath.toStdString(),
            nullptr
        );

        return tempVideo->has_steg_file();

    } catch (const std::exception &e) {
        spdlog::error("Error checking for steganography: {}", e.what());
        return false;
    }
}

void Backend::playVideoWithSubs() {
    if (m_inputVideo.isEmpty() || m_outputSubtitle.isEmpty()) {
        emit error("Cannot Play", "No video or subtitle loaded");
        return;
    }

    QProcess *process = new QProcess(this);  // Parent manages lifecycle

    if (!m_vlcPath.isEmpty()) {
        // Play with VLC
        QStringList args;
        args << "--sub-file=" + m_outputSubtitle
             << "--video-title-show"
             << m_inputVideo;

        process->start(m_vlcPath, args);

        if (!process->waitForStarted(3000)) {
            emit error("Player Error", "Failed to start VLC");
            process->deleteLater();
        }
    } else {
        // Fallback to ffplay
        QStringList args;
        args << "-vf" << "subtitles=" + m_outputSubtitle
             << "-i" << m_inputVideo;

        process->start("ffplay", args);

        if (!process->waitForStarted(3000)) {
            emit error("Player Error",
                      "Failed to start ffplay.\n"
                      "Please install VLC or ffmpeg for video playback.");
            process->deleteLater();
        }
    }
}

void Backend::playVideo(const QString &videoPath) {
    QString localPath = QUrl(videoPath).toLocalFile();
    if (localPath.isEmpty()) localPath = videoPath;

    if (!fileExists(localPath)) {
        emit error("File Not Found", "Video file does not exist:\n" + localPath);
        return;
    }

    QProcess *process = new QProcess(this);  // Parent manages lifecycle

    if (!m_vlcPath.isEmpty()) {
        // Play with VLC
        QStringList args;
        args << "--video-title-show" << localPath;

        process->start(m_vlcPath, args);

        if (!process->waitForStarted(3000)) {
            emit error("Player Error", "Failed to start VLC");
            process->deleteLater();
        }
    } else {
        // Fallback to ffplay
        QStringList args;
        args << "-i" << localPath;

        process->start("ffplay", args);

        if (!process->waitForStarted(3000)) {
            emit error("Player Error",
                      "Failed to start ffplay.\n"
                      "Please install VLC or ffmpeg for video playback.");
            process->deleteLater();
        }
    }
}
