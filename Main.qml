import QtQuick

Window {
    id: root
    width: 360
    height: 600
    visible: true
    title: qsTr("Calculator")
    minimumWidth: 360
    maximumWidth: 360
    minimumHeight: 400
    maximumHeight: 800
    color: "#024873"

    MyCalculator {
        id: calculator
        anchors.fill: parent
    }
}
