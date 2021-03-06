import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

StackScene {
    id: videoTrans

    property bool videoPlaying: false


    FileLoader {
        id: fileLoader
        visible: !videoPlaying
        x: 101
        y: 173
    }

    Text {
        id: element
        visible: !videoPlaying
        x: 225
        y: 90
        text: qsTr("Choose a video to transcribe")
        horizontalAlignment: Text.AlignHCenter
        font.family: "Tahoma"
        font.pixelSize: 20
    }


    Button {
        x: 201
        y: 284
        visible: !videoPlaying
        width: buttonW
        height: buttonH
        text: "Play video"
        onClicked: {
            var video = fileLoader.fileToSave
            if(video !== "") {
                var subtitleP = video.split(".")[0]
                subtitlePath.fileToSave = subtitleP + ".srt"
                videoPlaying = true
                backend.playVideo(video)
            } else {
                message_dialog.text = "Video file path must not be empty"
                message_dialog.open()
            }
        }
    }

    MessageDialog {
        id: message_dialog
        title: "May I have your attention please"
    }


    TextArea {
        id: subtitleText
        style: TextAreaStyle {
            textColor: "#333"
            selectionColor: "steelblue"
            selectedTextColor: "#eee"
            backgroundColor: "#eee"
        }
        x: 145
        y: 52
        height: 353
        visible: videoPlaying
        width: 411
        font.family: "Tahoma"
        font.pixelSize: 14
    }


    FileLoader {
        id: subtitlePath
        x: 171
        y: 17
        width: 345
        height: 29
        visible: videoPlaying
    }

    Button {
        x: 201
        y: 431
        visible: videoPlaying
        width: buttonW
        height: buttonH
        text: "Save subtitle file"
        onClicked: {
            var subtitle = subtitlePath.fileToSave
            if(subtitle !== "") {
                saveFile(subtitle, subtitleText.text)
                var enc = Qt.createComponent("qrc:/qml/encodeView.qml")
                stack.push(enc, {inputV: fileLoader.fileToSave, inputS: subtitle})
            } else {
                message_dialog.text = "Subtitle file path must not be empty"
                message_dialog.open()
            }
        }
    }

    function saveFile(fileUrl, text) {
        var request = new XMLHttpRequest();
        request.open("PUT", fileUrl, false);
        request.send(text);
        return request.status;
    }
}
