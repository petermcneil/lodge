#include "backend.h"
#include <video.h>
#include <subtitle.h>

using namespace lodge;
using namespace std;

backend::backend(QObject *parent) :
        QObject(parent) {
}

QString backend::inputVideoFileName() {
    return m_inputVideoFileName;
}

QString backend::outputVideoFileName() {
    return m_outputVideoFileName;
}

QString backend::inputSubtitleFileName() {
    return m_inputSubtitleFileName;
}

QString backend::outputSubtitleFileName() {
    return m_outputSubtitleFileName;
}

void backend::setInputVideoFileName(const QString &videoFileName) {
    if (videoFileName == m_inputVideoFileName) {
        return;
    } else {
        m_inputVideoFileName = videoFileName;
        emit this->inputVideoFileNameChanged();
    }
}

void backend::setOutputVideoFileName(const QString &videoFileName) {
    if (videoFileName == m_outputVideoFileName) {
        return;
    } else {
        m_outputSubtitleFileName = videoFileName;
        emit this->outputVideoFileNameChanged();
    }
}

void backend::setInputSubtitleFileName(const QString &subtitleFileName) {
    if (subtitleFileName == m_inputSubtitleFileName) {
        return;
    } else {
        m_inputSubtitleFileName = subtitleFileName;
        emit this->inputSubtitleFileNameChanged();
    }
}

void backend::setOutputSubtitleFileName(const QString &subtitleFileName) {
    if (subtitleFileName == m_outputSubtitleFileName) {
        return;
    } else {
        m_outputSubtitleFileName = subtitleFileName;
        emit this->outputSubtitleFileNameChanged();
    }
}

bool replace(std::string &str, const std::string &from, const std::string &to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


void backend::encodeVideoFile(const QString &inputSubtitle, const QString &inputVideo, const QString &outputVideo) {
    input_sub = inputSubtitle.toStdString();
    replace(input_sub, "file://", "");
    input_video = inputVideo.toStdString();
    replace(input_video, "file://", "");
    output_video = outputVideo.toStdString();
    replace(output_video, "file://", "");

    subtitle *sub = new subtitle(input_sub, true);
    video *vid = new video(input_video, output_video, sub);

    int ret = vid->write_subtitle_file();
}

void backend::decodeVideoFile(const QString &outputSubtitle, const QString &inputVideo) {
    output_sub = outputSubtitle.toStdString();
    replace(output_sub, "file://", "");
    input_video = inputVideo.toStdString();
    replace(input_video, "file://", "");

    subtitle *sub = new subtitle(output_sub, false);
    video *vid = new video(input_video, sub);
    qDebug("Building progress");
    QProgressDialog *progress = new QProgressDialog(QString("Extracting file...."), QString(), 0, 0);
    progress->setWindowModality(Qt::WindowModal);

    progress->setMaximum(0);
    progress->setMinimum(0);
    qDebug("Opening progress");

    progress->open();
    qDebug("Reading");
    int ret = vid->read_subtitle_file();
    qDebug("Finished reading");

    qDebug("Closing");
    progress->close();

    emit this->subtitleFileWritten();
}

bool backend::doesVideoContainSteg(const QString &videoPath)
{
    input_video = videoPath.toStdString();
    replace(input_video, "file://", "");

    video *vid = new video(input_video, nullptr);

    return vid->has_steg_file();
}
