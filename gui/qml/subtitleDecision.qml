import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

StackScene {
    id: subtiteDecision

    Button {
        id: yes
        x: 201
        y: 228
        width: buttonW
        height: buttonH
        text: "Yes"
        onClicked: {
            stack.push("qrc:/qml/encodeView.qml")
        }
    }

    Button {
        id: no
        x: 201
        y: 309
        width: buttonW
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
        y: 74
        width: 201
        height: 53
        text: qsTr("Do you have a subtitle file?")
        font.family: "Tahoma"
        font.pointSize: 34
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
