import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

StackScene {
    id: subtitleView

    TextArea {
        id: textArea
        x: 95
        y: 37
        width: 511
        height: 358
        font.family: "Arial"
        readOnly: true
    }

    Button {
        x: 201
        y: 418
        width: buttonW
        height: buttonH
        text: "Play video with subtitles"
        onClicked: {
            backend.playVideoWithSubs()
        }
    }

    function readTextFile(){
           var xhr = new XMLHttpRequest;
           var fileUrl = "file://" + backend.getOutputSubtitle()
           console.log("File url: " + fileUrl)
           xhr.open("GET", fileUrl);
           xhr.onreadystatechange = function () {
               if(xhr.readyState === XMLHttpRequest.DONE){
                   var response = xhr.responseText;
                   textArea.text = response
               }
           }
           xhr.send();
       }


    Component.onCompleted: {
        readTextFile()
    }
}
