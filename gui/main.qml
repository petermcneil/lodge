import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Window {
    property int h: 500
    property int w: 700
    property int buttonW: 299
    property int buttonH: 44
    property string buttonC: "#372c2c"

    visible: true
    minimumWidth: w
    maximumWidth: w
    maximumHeight: h
    minimumHeight: h
    width: w
    height: h
    color: "#cbc2c2"
    title: qsTr("Lodge")

    Image {
        id: image
        x: 175
        y: 59
        width: 350
        height: 138
        fillMode: Image.PreserveAspectFit
        source: "images/logo_transparent.png"
    }

    Button {
        id: encode_button
        x: 201
        y: 248
        width: buttonW
        height: buttonH
        text: "<font color='#ffffff'>" + "Write subtitle" + "</font>"

        style: ButtonStyle {
            background: Rectangle {
                color: buttonC
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
            }

        }

        onClicked: console.log("Pressed write subtitle")
    }

    Button {
        id: decode_button
        x: 201
        y: 374
        width: buttonW
        height: buttonH
        text: "<font color='#ffffff'>" + "Read subtitle" + "</font>"

        style: ButtonStyle {
            background: Rectangle {
                color: buttonC
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
            }
        }

        onClicked: console.log("Pressed read subtitle")
    }
}
