import QtQuick 6.4
import QtQuick.Controls 6.4
import QtQuick.Layouts 6.4
import Qt.labs.folderlistmodel 2.15
import QtCore

Dialog {
    id: fileDialog
    title: "Choose a file"
    modal: true
    width: 600
    height: 400
    standardButtons: Dialog.Ok | Dialog.Cancel
    
    property url selectedFile
    property var nameFilters: ["*"]
    property url currentFolder: StandardPaths.standardLocations(StandardPaths.HomeLocation)[0]
    
    signal accepted()
    
    onOpened: {
        folderModel.folder = currentFolder
    }
    
    contentItem: ColumnLayout {
        TextField {
            id: pathField
            Layout.fillWidth: true
            text: folderModel.folder
            readOnly: true
        }
        
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            
            model: FolderListModel {
                id: folderModel
                showDirs: true
                showDotAndDotDot: true
                nameFilters: fileDialog.nameFilters
            }
            
            delegate: ItemDelegate {
                width: ListView.view.width
                text: fileName
                highlighted: ListView.isCurrentItem
                
                onClicked: {
                    if (fileIsDir) {
                        folderModel.folder = fileURL
                    } else {
                        ListView.view.currentIndex = index
                        fileDialog.selectedFile = fileURL
                    }
                }
                
                onDoubleClicked: {
                    if (!fileIsDir) {
                        fileDialog.selectedFile = fileURL
                        fileDialog.accept()
                    }
                }
            }
        }
    }
}
