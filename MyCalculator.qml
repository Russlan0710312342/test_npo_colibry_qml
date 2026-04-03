import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 360
    height: 630

    property int buttonSize: 60
    property int spacing: 12

    // Верхний статус-бар
    Image {
        id: timeLabel
        source: "qrc:/images/icons/Status_bar.svg"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 360
        height: 24
        z: inputFrame.z + 1
    }

    Rectangle {
        id: inputFrame
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 360
        height: 156
        color: "#04BFAD"
        radius: 25
        antialiasing: true

        // Text: расчетная строка
        Text {
            id: calculationLine
            anchors.top: parent.top
            anchors.topMargin: 38
            anchors.right: parent.right
            anchors.rightMargin: 20
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            text: "380 + 240"
            font.family: "Open Sans"
            font.bold: true
            font.pixelSize: 24
            color: "#FFFFFF"
            lineHeight: 30
            lineHeightMode: Text.FixedHeight
        }

        // Text: результат
        Text {
            id: resultLine
            anchors.top: calculationLine.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 20
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            text: "620"
            font.family: "Open Sans"
            font.bold: true
            font.pixelSize: 50
            color: "#FFFFFF"
            lineHeight: 50
            lineHeightMode: Text.FixedHeight
        }
    }

    // Grid для кнопок
    Grid {
        id: buttonGrid
        anchors.top: inputFrame.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 4
        rowSpacing: 25
        columnSpacing: 25
        anchors.right: parent.right
        anchors.left: parent.left
        width: parent.width
        height: parent.height - (inputFrame.height + 20)
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.bottomMargin: 40
        // #F7E425 желтый
        Repeater {
            model: [
                {text: "()",    color: "#0889A6"},
                {text: "+/-",   color: "#0889A6"},
                {text: "%",     color: "#0889A6"},
                {text: "÷",     color: "#0889A6"},
                {text: "7",     color: "white"},
                {text: "8",     color: "white"},
                {text: "9",     color: "white"},
                {text: "x",     color: "#0889A6"},
                {text: "4",     color: "white"},
                {text: "5",     color: "white"},
                {text: "6",     color: "white"},
                {text: "-",     color: "#0889A6"},
                {text: "1",     color: "white"},
                {text: "2",     color: "white"},
                {text: "3",     color: "white"},
                {text: "+",     color: "#0889A6"},
                {text: "C",     color: "#F25E5E"},
                {text: "0",     color: "white"},
                {text: ".",     color: "white"},
                {text: "=",     color: "#0889A6"}
            ]

            delegate: Item {
                width: buttonSize
                height: buttonSize

                Rectangle {
                    anchors.fill: parent
                    color: modelData.color
                    radius: width / 2
                    antialiasing: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: console.log("Button clicked:", modelData.text)
                        cursorShape: Qt.PointingHandCursor
                    }

                    Text {
                        anchors.centerIn: parent
                        text: modelData.text
                        color: modelData.color === "white" ? "#04BFAD" : "white"
                        font.pixelSize: 20
                        font.bold: true
                    }
                }
            }
        }
    }
}