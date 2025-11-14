import QtQuick 6.4
import QtQuick.Controls 6.4
import QtQuick.Layouts 6.4

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
                x: 0
                y: 3
                width: 65
                height: 27
                font.pointSize: 20
                text: {
                    if (fileToSave !== null && fileToSave !== "") {
                        return fileToSave.replace("file://", "")
                    } else {
                        return initialText
                    }
                }
                verticalAlignment: Text.AlignVCenter

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

    CustomFileDialog {
        id: file_dialog
        nameFilters: filters
        onAccepted:  {
            fileToSave = file_dialog.selectedFile
            file_loader_root.changed(file_dialog.selectedFile)
        }
    }
}
