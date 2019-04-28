import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2

StackScene {
    id: subtitleView

    TextArea {
        id: textArea
        x: 105
        y: 29
        width: 502
        height: 298
        readOnly: true
    }

    Button {
        x: 241
        y: 401
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
