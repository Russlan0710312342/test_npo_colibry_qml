import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 360
    height: 630

    property int buttonSize: 60
    property int spacing: 12
    property bool openVal: false
    property Rectangle lastOperationButton: null
    property Text lastOperationButtonText: null
    property string lastOperationOriginalColor: ""

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
            text: "0"
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
            text: "0"
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
            id: repeter
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
                    id: btnRect
                    anchors.fill: parent
                    color: modelData.color
                    radius: width / 2
                    antialiasing: true

                    Text {
                        id: btnText
                        anchors.centerIn: parent
                        text: modelData.text
                        color: modelData.color === "white" ? "#04BFAD" : "white"
                        font.pixelSize: 20
                        font.bold: true
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            let t = modelData.text

                            // нормализация
                            if (t === "()") {
                                t = openVal ? ")" : "("
                                openVal = !openVal
                                controller.handleButtonClick(t)
                            } else if (t === "÷") controller.handleButtonClick("/")
                            else controller.handleButtonClick(t)

                            // Сброс предыдущей выделенной кнопки
                            if (lastOperationButton) {
                                lastOperationButton.color = lastOperationOriginalColor
                                lastOperationButtonText.color = (lastOperationOriginalColor === "white") ? "#04BFAD" : "white"
                            }

                            // Подсветка текущей кнопки (только для операций)
                            if (isNaN(Number(t)) && t !== "C") {
                                lastOperationButton = btnRect
                                lastOperationButtonText = btnText
                                lastOperationOriginalColor = btnRect.color

                                btnRect.color = "#F7E425"
                                btnText.color = "#000000"
                            }
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: controller

        function onUpdateInput (str) {
            resultLine.text = str
        }

        function onUpdateFormula (str) {
            calculationLine.text = str
        }
    }

}