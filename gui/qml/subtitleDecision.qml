import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

StackScene {
    id: subtitleDecision

    Button {
        id: yes
        x: 250
        y: 245
        width: 200
        height: buttonH
        text: "Yes"
        clip: false
        activeFocusOnPress: false
        onClicked: {
            stack.push("qrc:/qml/encodeView.qml")
        }
    }

    Image {
        id: image
        x: 300
        y: 27
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "qrc:/resources/logo_transparent.png"
    }

    Button {
        id: no
        x: 250
        y: 324
        width: 200
        height: buttonH
        text: "No"
        onClicked: {
            stack.push("qrc:/qml/videoTranscription.qml")
        }
    }

    MessageDialog {
        id: message_dialog
        title: "May I have your attention please"
    }

    Text {
        id: question
        x: 250
        y: 155
        width: 201
        height: 53
        text: qsTr("Do you have a subtitle file?")
        font.pointSize: 24
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
