//written by Ben

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent

    property url selectedFile
    property var results: ({})
    property bool isSimulation: true

    property int passCount: {
        var count = 0;
        for (var key in results) {
            if (results[key].Result === "PASS")
                count++;
        }
        return count;
    }

    property int failCount: {
        var count = 0;
        for (var key in results) {
            if (results[key].Result === "FAIL")
                count++;
        }
        return count;
    }

    property int progress: 0

    ListModel {
        id: consoleModel
    }

    function addMessage(text, color) {
        consoleModel.append({
            "text": text,
            "color": color
        })
    }

    Connections {
        target: testController

        function onLogMessage(text, color) {
            addMessage(text, color)
        }

        function onResultsReady(resultsMap) {
            results = resultsMap
            progress = 100
        }

        function onClearConsole() {
            consoleModel.clear()
        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            id: mainLayout
            width: scrollView.availableWidth
            anchors.margins: 24
            spacing: 24

            // HEADER CARD
            Frame {
                id: headerFrame
                Layout.fillWidth: true
                padding: 16
                implicitHeight: headerCol.implicitHeight + padding * 2

                background: Rectangle {
                    color: "#e8f1ff"
                    radius: 8
                    border.color: "#bfdbfe"
                }

                ColumnLayout {
                    id: headerCol
                    anchors.fill: parent
                    spacing: 6

                    Label {
                        text: "Test Execution & Control"
                        font.pixelSize: 20
                        font.bold: true
                    }

                    Label {
                        text: "Run tests on your circuit in simulation or hardware mode and monitor execution in real-time"
                        color: "#4b5563"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }
            }

            // MAIN ROW: LEFT (TEST MODE) + RIGHT (PROGRESS / CONSOLE)
            RowLayout {
                id: cardRow
                Layout.fillWidth: true
                spacing: 24

                // we’ll compute a shared height once children know their implicitHeight
                property real sharedHeight:
                    Math.max(leftCol.implicitHeight, rightCol.implicitHeight) + 32

                // LEFT CARD
                Frame {
                    id: leftFrame
                    Layout.fillWidth: true
                    padding: 16
                    implicitHeight: cardRow.sharedHeight

                    background: Rectangle {
                        color: "#ffffff"
                        radius: 12
                        border.color: "#d1d5db"
                    }

                    ColumnLayout {
                        id: leftCol
                        anchors.fill: parent
                        spacing: 16

                        ColumnLayout {
                            spacing: 4
                            Label {
                                text: "Test Mode"
                                font.pixelSize: 18
                                font.bold: true
                            }
                            Label {
                                text: "Choose how to execute your tests"
                                color: "#6b7280"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }

                        ButtonGroup { id: modeGroup }

                        // Simulation mode card
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "#f9fafb"
                            implicitHeight: simRow.implicitHeight + 24

                            RowLayout {
                                id: simRow
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                RadioButton {
                                    id: simulationRadio
                                    Layout.alignment: Qt.AlignTop
                                    checked: true
                                    ButtonGroup.group: modeGroup
                                    onCheckedChanged: {
                                        isSimulation = checked;
                                    }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label {
                                        text: "Simulation Mode"
                                        font.bold: true
                                    }
                                    Label {
                                        text: "Test circuit behavior using software simulation"
                                        font.pixelSize: 12
                                        color: "#6b7280"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        // Hardware mode card
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "#f9fafb"
                            implicitHeight: hwRow.implicitHeight + 24

                            RowLayout {
                                id: hwRow
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                RadioButton {
                                    id: hardwareRadio
                                    Layout.alignment: Qt.AlignTop
                                    ButtonGroup.group: modeGroup
                                    onCheckedChanged: {
                                        isSimulation = !checked;
                                    }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label {
                                        text: "Hardware Mode"
                                        font.bold: true
                                    }
                                    Label {
                                        text: "Test with connected physical hardware"
                                        font.pixelSize: 12
                                        color: "#6b7280"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        // Start / pause / stop buttons
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: "#e5e7eb"
                            }

                            Button {
                                text: "Start Test"
                                Layout.fillWidth: true
                                onClicked: {
                                    //results = testController.runTests(selectedFile, isSimulation);
                                    //console.log(JSON.stringify(results, null, 2));
                                    if (!isSimulation && !serialComms.isMCUConnected()) {
                                        testController.clearConsole();
                                        testController.logMessage("[Error] Invalid Pin Count in chip configuration", "#ef4444");
                                        return;
                                    }
                                    testController.startTests(selectedFile, isSimulation);
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                Button {
                                    text: "Pause"
                                    Layout.fillWidth: true
                                    onClicked: {
                                        testController.pauseTests()
                                    }
                                }

                                Button {
                                    text: "Stop"
                                    Layout.fillWidth: true
                                    onClicked: {
                                        testController.stopTests()
                                    }
                                }
                            }
                        }

                        // Instructions link
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: "#e5e7eb"
                            }

                            Button {
                                text: "Test setup instructions"
                                flat: true
                                Layout.alignment: Qt.AlignLeft
                                ToolTip.visible: hovered
                                ToolTip.text: "1. Load configuration file\n2. Select test script\n3. Choose test mode\n4. Verify hardware connections (if applicable)"
                            }
                        }
                    }
                }

                // RIGHT CARD
                Frame {
                    id: rightFrame
                    Layout.fillWidth: true
                    padding: 16
                    implicitHeight: cardRow.sharedHeight

                    background: Rectangle {
                        color: "#ffffff"
                        radius: 12
                        border.color: "#d1d5db"
                    }

                    ColumnLayout {
                        id: rightCol
                        anchors.fill: parent
                        spacing: 16

                        // Title + Idle pill
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2
                                Label {
                                    text: "Test Progress & Output"
                                    font.pixelSize: 18
                                    font.bold: true
                                }
                                Label {
                                    text: "Real-time execution status and log messages"
                                    color: "#6b7280"
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                }
                            }

                            Label {
                                text: "Idle"
                                padding: 6
                                background: Rectangle {
                                    radius: 999
                                    color: "#f3f4f6"
                                    border.color: "#d1d5db"
                                }
                            }
                        }

                        // Progress bar
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            RowLayout {
                                Layout.fillWidth: true
                                Label { text: "Test Progress"; color: "#4b5563" }
                                Label {
                                    text: "0 / 0 steps"
                                    Layout.alignment: Qt.AlignRight
                                    color: "#111827"
                                }
                            }

                            ProgressBar {
                                from: 0
                                to: 100
                                value: progress
                                Layout.fillWidth: true
                            }
                        }

                        // Mini stats bar
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            color: "#f9fafb"
                            border.color: "#e5e7eb"
                            implicitHeight: statsGrid.implicitHeight + 24

                            GridLayout {
                                id: statsGrid
                                anchors.fill: parent
                                anchors.margins: 12
                                columns: 3
                                columnSpacing: 24
                                rowSpacing: 8

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Passed"; font.pixelSize: 11; color: "#6b7280" }
                                    Label { text: passCount; color: "#16a34a" }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Failed"; font.pixelSize: 11; color: "#6b7280" }
                                    Label { text: failCount; color: "#dc2626" }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Warnings"; font.pixelSize: 11; color: "#6b7280" }
                                    Label { text: "0"; color: "#ca8a04" }
                                }
                            }
                        }

                        // Console
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Console Output"
                                font.bold: true
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 260
                                radius: 8
                                color: "#111827"
                                border.color: "#1f2937"

                                ScrollView {
                                    id: consoleScroll
                                    anchors.fill: parent
                                    clip: true
                                    contentWidth: availableWidth

                                    Column {
                                        width: parent.width
                                        spacing: 2
                                        anchors.margins: 8

                                        Repeater {
                                            model: consoleModel

                                            delegate: Text {
                                                text: model.text
                                                color: model.color
                                                font.pixelSize: 12
                                                font.family: "monospace"
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
