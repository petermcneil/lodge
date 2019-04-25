import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Rectangle {
    id: encodeView
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
            output_video.fileToSave = path.replace(".mp4", "_lodge.mp4")
        }
    }

    FileLoader {
        id: output_video
        x: 70
        y: 251
        buttonText: "Output video file"
        filters: ["Video files (*.mp4, *.mkv, *.avi)", "All files (*)"]
    }

    FileLoader {
        id: input_subtitle
        x: 70
        y: 333
        buttonText: "Load subtitle file"
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
        text: "Write subtitle file"
        onClicked: {
            var input_s = input_subtitle.fileToSave
            var input_v = input_video.fileToSave
            var output_v = output_video.fileToSave

            if(input_s !== "" && input_v !== "" && output_v !== "") {
                backend.encodeVideoFile(input_subtitle.fileToSave, input_video.fileToSave, output_video.fileToSave)
            } else {
                message_dialog.text = "File paths are empty, add some to continue!"
                message_dialog.open()
            }

        }
    }

    MessageDialog {
        id: message_dialog
        title: "May I have your attention please"
    }
}
