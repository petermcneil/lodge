import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import lodge.backend 1.0

StackScene {
    id: decodeView

    FileLoader {
        id: input_video
        x: 100
        y: 184
        buttonText: "Input video file"
        filters: ["All files (*)"]
    }

    FileLoader {
        id: output_subtitle
        x: 100
        y: 257
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
        x: 301
        y: 53
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "qrc:/resources/logo_transparent.png"
    }

    Button {
        id: checkForFile
        x: 201
        y: 311
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
        x: 201
        y: 380
        width: buttonW
        height: buttonH
        text: "Read file"
        visible: false

        onVisibleChanged: {
            checkForFile.visible = false
        }

        onClicked : {
            backend.decodeVideoFile(output_subtitle.fileToSave, input_video.fileToSave)
            stack.push("qrc:/qml/subtitleView.qml")
        }
    }

    MessageDialog {
        id: message_dialog
        title: "May I have your attention please"
    }

}
