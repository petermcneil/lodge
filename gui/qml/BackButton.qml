import QtQuick 6.4
import QtQuick.Controls 6.4


Button {
    id: back
    x: 2
    y: 2
    height: 30
    width: 40
    text: "Back"

    onClicked: stack.pop()
}

