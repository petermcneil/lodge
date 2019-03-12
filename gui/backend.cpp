#include "backend.h"
#include <video.h>
#include <subtitle.h>

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

    subtitle *sub = new subtitle(in_sub, true);
    video *vid = new video(in_vid, out_vid, sub);

    int ret = vid->write_subtitle_file();
}

void Backend::decodeVideoFile(const QString &outputSubtitle, const QString &inputVideo) {
    string out_sub = outputSubtitle.toStdString();
    replace(out_sub, "file://", "");
    string in_vid = inputVideo.toStdString();
    replace(in_vid, "file://", "");

    subtitle *sub = new subtitle(out_sub, false);
    video *vid = new video(in_vid, sub);

    int ret = vid->read_subtitle_file();

}

bool Backend::doesVideoContainSteg(const QString &videoPath)
{
    string vid_path = videoPath.toStdString();
    replace(vid_path, "file://", "");

    video *vid = new video(vid_path, nullptr);

    return vid->has_steg_file();
}


