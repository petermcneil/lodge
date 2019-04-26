#include "backend.h"
#include <spdlog/spdlog.h>

namespace log = spdlog;
using namespace std;

backend::backend(QObject *parent) :
        QObject(parent) {
    vlc = fileExists(vlcPath);
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

    log::debug("Input: {} Output: {} IS: {}", input_video, output_video, input_sub);
    subtitle = new class subtitle(input_sub, RW::READ);
    video = new class video(input_video, output_video, subtitle);

    int ret = video->write_subtitle_file();
}

void backend::decodeVideoFile(const QString &outputSubtitle, const QString &inputVideo) {
    output_sub = outputSubtitle.toStdString();
    replace(output_sub, "file://", "");

    video->subtitle_file->set_path(output_sub);
    int ret = video->read_subtitle_file();

    emit this->subtitleFileWritten();
}

bool backend::doesVideoContainSteg(const QString &videoPath)
{
    input_video = videoPath.toStdString();
    replace(input_video, "file://", "");

    video = new class video(input_video, nullptr);

    return video->has_steg_file();
}

QString backend::getOutputSubtitle() {
    auto * str = this->video->subtitle_file->get_path().c_str();
    auto *qs = new QString(str);
    qDebug(qs->toLatin1());
    return *qs;
}

void backend::playVideoWithSubs() {
    QProcess qProcess;

    if(vlc) {
        string subtitleOption = "--sub-file=" + output_sub;
        qProcess.startDetached(vlcPath, QStringList() << subtitleOption.c_str() << "--video-on-top" << "--video-title-show" << input_video.c_str());
        qDebug(qProcess.readAllStandardOutput());
    } else {
        string subtitles = "subtitles=" + output_sub;
        QString command("ffplay");
        qProcess.startDetached(command, QStringList() << "-vf" << subtitles.c_str() << "-i" << input_video.c_str());

        qDebug(qProcess.readAllStandardOutput());
    }

}

void backend::playVideo(const QString &videoPath) {
    QProcess qProcess;

    if(vlc) {
        qProcess.startDetached(vlcPath, QStringList() << "--video-on-top" << "--video-title-show" << videoPath);
        qDebug(qProcess.readAllStandardOutput());
    } else {
        QString command("ffplay");
        qProcess.startDetached(command, QStringList() << "-i" << videoPath);

        qDebug(qProcess.readAllStandardOutput());
    }

}

bool backend::fileExists(QString path) {
    QFileInfo check_file(path);
    return check_file.exists() && check_file.isFile();
}
