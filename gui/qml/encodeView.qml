import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

StackScene {
    id: encodeView

    property string inputV
    property string inputS

    Component.onCompleted: {
        console.log("Loading")
        if(inputS !== "" && inputV !== "") {
            input_video.fileToSave = inputV
            input_video.changed(inputV)
            input_subtitle.fileToSave = inputS
            input_subtitle.changed(inputS)
            console.log("Finished")
        }
    }


    FileLoader {
        id: input_video
        x: 100
        y: 173
        buttonText: "Input video file"
        filters: ["All files (*)"]
        onChanged: {
            output_video.fileToSave = path.replace(".mp4", "_lodge.mp4")
        }
    }

    FileLoader {
        id: output_video
        x: 100
        y: 251
        buttonText: "Output video file"
        filters: ["All files (*)"]
    }

    FileLoader {
        id: input_subtitle
        x: 100
        y: 333
        buttonText: "Load subtitle file"
        filters: ["All files (*)"]
    }

    Image {
        id: image
        x: 270
        y: 46
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "qrc:/resources/logo_transparent.png"
    }

    Button {
        x: 171
        y: 399
        width: buttonW
        height: buttonH
        text: "Write subtitle file"
        onClicked: {
            var input_s = input_subtitle.fileToSave
            var input_v = input_video.fileToSave
            var output_v = output_video.fileToSave

            if(input_s !== "" && input_v !== "" && output_v !== "") {
                backend.encodeVideoFile(input_subtitle.fileToSave, input_video.fileToSave, output_video.fileToSave)
                message_dialog.text = "Subtitle file sucessfully lodged!"
                message_dialog.open()
            } else {
                message_dialog.text = "File paths are empty, add some to continue!"
                message_dialog.open()
            }

        }
    }


    MessageDialog {
        id: message_dialog
        title: "May I have your attention please"
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
