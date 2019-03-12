import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Item {
    id: file_loader_root
    height: 30
    width: 500
    property string initialText: "No file found"
    property string fileToSave: ""
    property string buttonText: "Load file"
    property var filters : ["All files (*)"]

    signal changed(string path)

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: file_loader_btn.left
        anchors.bottom: parent.bottom

        ScrollView {
            anchors.fill: parent
            contentWidth: file_loader_text.contentWidth
            clip: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.horizontal.interactive: false

            Text {
                id: file_loader_text
                font.pointSize: 20
                text: {
                    if (fileToSave !== null && fileToSave !== "") {
                        return fileToSave.replace("file://", "")
                    } else {
                        return initialText
                    }
                }

            }
        }

    }

    Button {
        id: file_loader_btn
        width: 130
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: buttonText
        onClicked: file_dialog.open()
    }

    FileDialog {
        id: file_dialog
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted:  {
            fileToSave = file_dialog.fileUrl
            file_loader_root.changed(file_dialog.fileUrl)
        }

        nameFilters: filters
    }
}
