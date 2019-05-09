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
                subtitlePath.text = subtitleP + ".srt"
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
        width: 411
        visible: videoPlaying
        font.family: "Tahoma"
        font.pixelSize: 14
    }

    Text {
        id: subtitlePath
        visible: videoPlaying
        x: 225
        y: 17
        width: 251
        height: 15
        font.pixelSize: 12
    }

    Button {
        x: 201
        y: 431
        visible: videoPlaying
        width: buttonW
        height: buttonH
        text: "Save subtitle file"
        onClicked: {
            var subtitle = subtitlePath.text
            if(subtitle !== "") {
                saveFile(subtitle, subtitleText.text)
                stack.push("qrc:/qml/encodeView.qml")
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
