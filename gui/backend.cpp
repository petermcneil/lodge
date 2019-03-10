#include "backend.h"
#include <VideoFile.h>
#include <SubtitleFile.h>

using namespace lodge;
using namespace std;

Backend::Backend(QObject *parent) :
        QObject(parent) {
}

QString Backend::inputVideoFileName() {
    return m_inputVideoFileName;
}

QString Backend::outputVideoFileName() {
    return m_outputVideoFileName;
}

QString Backend::inputSubtitleFileName() {
    return m_inputSubtitleFileName;
}

QString Backend::outputSubtitleFileName() {
    return m_outputSubtitleFileName;
}

void Backend::setInputVideoFileName(const QString &videoFileName) {
    if (videoFileName == m_inputVideoFileName) {
        return;
    } else {
        m_inputVideoFileName = videoFileName;
        emit this->inputVideoFileNameChanged();
    }
}

void Backend::setOutputVideoFileName(const QString &videoFileName) {
    if (videoFileName == m_outputVideoFileName) {
        return;
    } else {
        m_outputSubtitleFileName = videoFileName;
        emit this->outputVideoFileNameChanged();
    }
}

void Backend::setInputSubtitleFileName(const QString &subtitleFileName) {
    if (subtitleFileName == m_inputSubtitleFileName) {
        return;
    } else {
        m_inputSubtitleFileName = subtitleFileName;
        emit this->inputSubtitleFileNameChanged();
    }
}

void Backend::setOutputSubtitleFileName(const QString &subtitleFileName) {
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


void Backend::encodeVideoFile(const QString &inputSubtitle, const QString &inputVideo, const QString &outputVideo) {
    string in_sub = inputSubtitle.toStdString();
    replace(in_sub, "file://", "");
    string in_vid = inputVideo.toStdString();
    replace(in_vid, "file://", "");
    string out_vid = outputVideo.toStdString();
    replace(out_vid, "file://", "");

    SubtitleFile *subtitle = new SubtitleFile(in_sub, true);
    VideoFile *video = new VideoFile(in_vid, out_vid, subtitle);

    int ret = video->write_subtitle_file();
}

void Backend::decodeVideoFile(const QString &outputSubtitle, const QString &inputVideo) {
    string out_sub = outputSubtitle.toStdString();
    replace(out_sub, "file://", "");
    string in_vid = inputVideo.toStdString();
    replace(in_vid, "file://", "");

    SubtitleFile *subtitle = new SubtitleFile(out_sub, false);
    VideoFile *video = new VideoFile(in_vid, subtitle);

    int ret = video->read_subtitle_file();

}


