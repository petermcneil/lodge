import QtQuick 6.4
import QtQuick.Controls 6.4
import Qt.labs.platform 1.1
import QtQuick.Layouts 6.4
import QtQuick.Window 6.4

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
