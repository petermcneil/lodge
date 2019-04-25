import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import lodge.backend 1.0

Rectangle {
    id: decodeView
    color: backgroundC
    width: w
    height: h

    BackButton{ width: 54; height: 30 }

    FileLoader {
        id: input_video
        x: 70
        y: 175
        buttonText: "Input video file"
        filters: ["All files (*)"]
    }

    FileLoader {
        id: output_subtitle
        x: 70
        y: 225
        buttonText: "Output subtitle file"
        visible: false

        onVisibleChanged: {
            var sub = backend.getOutputSubtitle()
            this.fileToSave = sub
            this.changed(sub)
        }
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
        id: checkForFile
        x: 173
        y: 274
        width: buttonW
        height: buttonH
        text: "Check for subtitle file"
        onClicked: {
            var path = input_video.fileToSave
            if(path !== "") {
                var contains = backend.doesVideoContainSteg(path)
                if(contains) {
                    console.log("Decoding and output")
                    output_subtitle.visible = true
                    readFile.visible = true
                } else {
                    message_dialog.text = "Video file does not contain a subtitle file."
                    message_dialog.open()
                }
            } else {
                message_dialog.text = "Video file path is empty, add one!"
                message_dialog.open()
            }
        }
    }

    Button {
        id: readFile
        x: 173
        y: 323
        width: buttonW
        height: buttonH
        text: "Read file"
        visible: false

        onVisibleChanged: {
            checkForFile.visible = false
        }

        onClicked : {
            backend.decodeVideoFile(output_subtitle.fileToSave, input_video.fileToSave)
            stack.push("qrc:/SubtitleView.qml")
        }
    }

    MessageDialog {
        id: message_dialog
        title: "May I have your attention please"
    }
}
