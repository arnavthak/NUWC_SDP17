import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        id: page
        width: parent.width
        height: parent.height
        color: "lightgray"
        anchors.centerIn: parent

        Text {
            id: helloText
            text: "Hello World!"
            y: 30
            anchors.horizontalCenter: page.horizontalCenter
            font.pointSize: 24
            font.bold: true
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
        }

        states: State {
            name: "down"
            when: mouseArea.pressed == true
            PropertyChanges {
                helloText {
                    y: 400
                    rotation: 180
                }
            }
        }

        transitions: Transition {
            from: ""
            to: "down"
            reversible: true
            NumberAnimation {
                properties: "y,rotation"
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }

        Grid {
            id: colorPicker
            x: 4
            anchors.bottom: page.bottom
            anchors.bottomMargin: 4
            rows: 2
            columns: 3
            spacing: 3

            Cell {
                cellColor: "red"
                onClicked: helloText.color = cellColor
            }
            Cell {
                cellColor: "green"
                onClicked: helloText.color = cellColor
            }
            Cell {
                cellColor: "blue"
                onClicked: helloText.color = cellColor
            }
            Cell {
                cellColor: "yellow"
                onClicked: helloText.color = cellColor
            }
            Cell {
                cellColor: "purple"
                onClicked: helloText.color = cellColor
            }
            Cell {
                cellColor: "black"
                onClicked: helloText.color = cellColor
            }
        }
    }
}
