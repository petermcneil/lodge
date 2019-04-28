import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import lodge.backend 1.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: root
    property int h: 500
    property int w: 700
    property int buttonW: 299
    property int buttonH: 44
    property string buttonC: "#372c2c"
    property string backgroundC: "#cbc2c2"

    visible: true
    minimumWidth: w
    maximumWidth: w
    maximumHeight: h
    minimumHeight: h
    width: w
    height: h
    color: "#000000"
    title: qsTr("Lodge")

    Backend {
        id: backend
    }


    Component {
        id: rootView

        Rectangle {
            height: h
            width: w
            color: backgroundC

            Image {
                id: root_logo
                x: 175
                y: 59
                width: 350
                height: 138
                fillMode: Image.PreserveAspectFit
                source: "qrc:/resources/logo_transparent.png"
            }

            Button {
                id: encode_button
                x: 201
                y: 248
                width: buttonW
                height: buttonH
                text: "<font color='#000000'>" + "Write subtitle" + "</font>"

                onClicked: {
                    stack.push("qrc:/qml/subtitleDecision.qml")
                }

            }

            Button {
                id: decode_button
                x: 201
                y: 374
                width: buttonW
                height: buttonH
                text: "<font color='#000000'>" + "Read subtitle" + "</font>"

                onClicked: {
                    stack.push("qrc:/qml/decodeView.qml")
                }
            }
        }

    }


    StackView {
        id: stack
        initialItem: rootView
        Layout.fillHeight: true
        Layout.fillWidth: true

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 300
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 300
            }
        }
    }
}
