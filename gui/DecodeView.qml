import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Rectangle {
    id: decodeView
    color: backgroundC
    width: w
    height: h

    BackButton{ width: 54; height: 30}

    FileLoader {
        id: input_video
        x: 70
        y: 175
        buttonText: "Input video file"
        filters: ["Video files (*.mp4, *.mkv, *.avi)", "All files (*)"]
        onChanged: {
            var pieces = path.split(/[\s/]+/);
            var vid_file = pieces[pieces.length-1]
            output_subtitle.fileToSave = path.replace(".mp4", "_lodge.mp4")
        }
    }

    FileLoader {
        id: output_subtitle
        x: 70
        y: 251
        buttonText: "Output subtitle file"
        filters: ["Subtitle files (*.srt)", "All files (*)"]
    }

    Image {
        id: image
        x: 270
        y: 46
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "../extras/logos/logo_transparent.png"
    }

    Button {
        x: 150
        y: 399
        width: buttonW
        height: buttonH
        text: "Read subtitle file"
        onClicked: {
            if(backend.doesVideoContainSteg(input_video.fileToSave)) {
                backend.decodeVideoFile(output_subtitle.fileToSave, input_video.fileToSave)
            } else {
                message_dialog.text = "Video file does not contain a subtitle file."
                message_dialog.open()
            }
        }
    }

    MessageDialog {
        id: message_dialog
        title: "May I have your attention please"
    }
}
