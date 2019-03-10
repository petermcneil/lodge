import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

Rectangle {
    id: encodeView
    color: backgroundC
    width: w
    height: h

    BackButton{ width: 54; height: 30}

    FileLoader {
        id: input_video
        x: 70
        y: 175
        buttonText: "Input video file"
    }

    FileLoader {
        id: output_video
        x: 70
        y: 251
        buttonText: "Output video file"
    }

    FileLoader {
        id: output_subtitle
        x: 70
        y: 333
        buttonText: "Load subtitle file"
    }

    Image {
        id: image
        x: 270
        y: 46
        width: 100
        height: 100
        fillMode: Image.PreserveAspectFit
        source: "../extras/logos/logo_transparent.png"
    }

    Button {
        x: 150
        y: 399
        width: buttonW
        height: buttonH
        text: "Write subtitle file"
        onClicked: {
            backend.encodeVideoFile(output_subtitle.fileToSave, input_video.fileToSave, output_video.fileToSave)
        }
    }
}
