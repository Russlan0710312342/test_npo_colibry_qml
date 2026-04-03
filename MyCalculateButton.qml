import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: calculateButtonRoot

    signal clickedSignal()
    property alias text: buttonText.text   // текст кнопки
    property color color: "white"          // цвет кнопки
    width: 60
    height: 60   // квадрат → круг

    // Тень (через задний Rectangle)
    Rectangle {
        anchors.fill: parent
        color: "#40000000"    // тёмная полупрозрачная тень
        radius: height / 2
        anchors.topMargin: 2
        anchors.bottomMargin: -2
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        z: -1
    }

    // Фон кнопки
    Rectangle {
        anchors.fill: parent
        color: calculateButtonRoot.color
        radius: height / 2
        antialiasing: true

        // События нажатия
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
        }

        // Текст по центру
        Text {
            id: buttonText
            anchors.centerIn: parent
            text: "1"
            color: "#024873"
            font.pixelSize: 28
            font.bold: true
        }
    }
}

