import QtQuick 2.0
import QtQuick.Controls 1.4


    Button {
        id: back
        x: 2
        y: 2
        height: 30
        width: 40
        text: "Back"

        onClicked: stack.pop()
    }

