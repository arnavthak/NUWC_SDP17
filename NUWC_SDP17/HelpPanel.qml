// written by Ben

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent

    ScrollView {
        id: scrollView
        anchors.fill: parent
        clip: true
        contentWidth: availableWidth

        ColumnLayout {
            id: mainLayout
            width: scrollView.availableWidth
            spacing: 24
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 24

            // ===================== HEADER =====================
            Frame {
                Layout.fillWidth: true
                padding: 16

                background: Rectangle {
                    color: "#e8f1ff"
                    radius: 8
                    border.color: "#bfdbfe"
                }

                contentItem: ColumnLayout {
                    spacing: 6

                    Label {
                        text: "Help & Documentation"
                        font.pixelSize: 20
                        font.bold: true
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "Learn how to use the IC Test Bench application - from setup to analysis"
                        color: "#4b5563"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }
            }

            // ===================== QUICK START =====================
            Frame {
                Layout.fillWidth: true
                padding: 16

                contentItem: ColumnLayout {
                    spacing: 16
                    Layout.fillWidth: true

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Label {
                            text: "Quick Start: Testing Workflow"
                            font.pixelSize: 18
                            font.bold: true
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                        Label {
                            text: "Follow these steps to test your circuit configuration"
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        spacing: 12
                        Layout.fillWidth: true

                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            color: "#f9fafb"
                            border.color: "#e5e7eb"
                            implicitHeight: step1Row.implicitHeight + 24

                            RowLayout {
                                id: step1Row
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                Rectangle {
                                    width: 40
                                    height: 40
                                    radius: 8
                                    color: "#dbeafe"
                                    Layout.alignment: Qt.AlignTop

                                    Label {
                                        anchors.centerIn: parent
                                        text: "1"
                                        color: "#1d4ed8"
                                        font.bold: true
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 4

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 8

                                        Label {
                                            text: "Step 1"
                                            padding: 4
                                            background: Rectangle {
                                                radius: 999
                                                color: "white"
                                                border.color: "#e5e7eb"
                                            }
                                        }

                                        Label {
                                            text: "Load Configuration"
                                            color: "#111827"
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                    }

                                    Label {
                                        text: "Navigate to the Configuration Panel and select your circuit configuration file (.yaml)."
                                        color: "#4b5563"
                                        font.pixelSize: 12
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            color: "#f9fafb"
                            border.color: "#e5e7eb"
                            implicitHeight: step2Row.implicitHeight + 24

                            RowLayout {
                                id: step2Row
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                Rectangle {
                                    width: 40
                                    height: 40
                                    radius: 8
                                    color: "#dcfce7"
                                    Layout.alignment: Qt.AlignTop

                                    Label {
                                        anchors.centerIn: parent
                                        text: "2"
                                        color: "#16a34a"
                                        font.bold: true
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 4

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 8

                                        Label {
                                            text: "Step 2"
                                            padding: 4
                                            background: Rectangle {
                                                radius: 999
                                                color: "white"
                                                border.color: "#e5e7eb"
                                            }
                                        }

                                        Label {
                                            text: "Execute Tests"
                                            color: "#111827"
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                    }

                                    Label {
                                        text: "Go to the Test Control Panel, choose between Simulation or Hardware mode, and click \"Start Test\" to run your test script."
                                        color: "#4b5563"
                                        font.pixelSize: 12
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            color: "#f5f3ff"
                            border.color: "#e5e7eb"
                            implicitHeight: step3Row.implicitHeight + 24

                            RowLayout {
                                id: step3Row
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                Rectangle {
                                    width: 40
                                    height: 40
                                    radius: 8
                                    color: "#ede9fe"
                                    Layout.alignment: Qt.AlignTop

                                    Label {
                                        anchors.centerIn: parent
                                        text: "3"
                                        color: "#7c3aed"
                                        font.bold: true
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 4

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 8

                                        Label {
                                            text: "Step 3"
                                            padding: 4
                                            background: Rectangle {
                                                radius: 999
                                                color: "white"
                                                border.color: "#e5e7eb"
                                            }
                                        }

                                        Label {
                                            text: "Review Results"
                                            color: "#111827"
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                    }

                                    Label {
                                        text: "View your test results, truth tables, and pin mappings in the Results Panel. Export data as needed for further analysis."
                                        color: "#4b5563"
                                        font.pixelSize: 12
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // ===================== FAQ =====================
            Frame {
                Layout.fillWidth: true
                padding: 16

                contentItem: ColumnLayout {
                    spacing: 16
                    Layout.fillWidth: true

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Label {
                            text: "Frequently Asked Questions"
                            font.pixelSize: 18
                            font.bold: true
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                        Label {
                            text: "Common questions about file types, configuration, and testing"
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        spacing: 8
                        Layout.fillWidth: true

                        Rectangle {
                            id: faq1
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false
                            implicitHeight: faq1Col.implicitHeight + 24

                            ColumnLayout {
                                id: faq1Col
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                Item {
                                    Layout.fillWidth: true
                                    implicitHeight: faq1Row.implicitHeight

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: faq1.expanded = !faq1.expanded
                                    }

                                    RowLayout {
                                        id: faq1Row
                                        anchors.fill: parent
                                        spacing: 8

                                        Label {
                                            text: "What file types are supported?"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                        Label {
                                            text: faq1.expanded ? "▲" : "▼"
                                            color: "#9ca3af"
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: faq1.expanded
                                    spacing: 4

                                    Label {
                                        text: "Configuration Files: YAML (.yaml) files containing circuit parameters, chip specifications, pin counts, logic type definitions, the sequence of test operations, input patterns, and expected outputs."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: faq2
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false
                            implicitHeight: faq2Col.implicitHeight + 24

                            ColumnLayout {
                                id: faq2Col
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                Item {
                                    Layout.fillWidth: true
                                    implicitHeight: faq2Row.implicitHeight

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: faq2.expanded = !faq2.expanded
                                    }

                                    RowLayout {
                                        id: faq2Row
                                        anchors.fill: parent
                                        spacing: 8

                                        Label {
                                            text: "What's the difference between Simulation and Hardware mode?"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                        Label {
                                            text: faq2.expanded ? "▲" : "▼"
                                            color: "#9ca3af"
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: faq2.expanded
                                    spacing: 4

                                    Label {
                                        text: "Simulation Mode: Tests your circuit design using software simulation. Always has the actual output match the expected output. Used for testing the application independent of the hardware."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: "Hardware Mode: Tests with connected physical hardware. Requires proper hardware connections and IC chip for testing."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: faq3
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false
                            implicitHeight: faq3Col.implicitHeight + 24

                            ColumnLayout {
                                id: faq3Col
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                Item {
                                    Layout.fillWidth: true
                                    implicitHeight: faq3Row.implicitHeight

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: faq3.expanded = !faq3.expanded
                                    }

                                    RowLayout {
                                        id: faq3Row
                                        anchors.fill: parent
                                        spacing: 8

                                        Label {
                                            text: "How do I configure circuit parameters?"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                        Label {
                                            text: faq3.expanded ? "▲" : "▼"
                                            color: "#9ca3af"
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: faq3.expanded
                                    spacing: 4

                                    Label {
                                        text: "Circuit parameters are defined in your configuration file (chip number, logic type, pin count, etc.). You must edit the file directly."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: faq4
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false
                            implicitHeight: faq4Col.implicitHeight + 24

                            ColumnLayout {
                                id: faq4Col
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                Item {
                                    Layout.fillWidth: true
                                    implicitHeight: faq4Row.implicitHeight

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: faq4.expanded = !faq4.expanded
                                    }

                                    RowLayout {
                                        id: faq4Row
                                        anchors.fill: parent
                                        spacing: 8

                                        Label {
                                            text: "How do I interpret truth table results?"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                        Label {
                                            text: faq4.expanded ? "▲" : "▼"
                                            color: "#9ca3af"
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: faq4.expanded
                                    spacing: 4

                                    Label {
                                        text: "Compare Output vs Expected. PASS means they match; FAIL means mismatch. The Inputs tab shows the values that all the input pins were set to."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: faq5
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false
                            implicitHeight: faq5Col.implicitHeight + 24

                            ColumnLayout {
                                id: faq5Col
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                Item {
                                    Layout.fillWidth: true
                                    implicitHeight: faq5Row.implicitHeight

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: faq5.expanded = !faq5.expanded
                                    }

                                    RowLayout {
                                        id: faq5Row
                                        anchors.fill: parent
                                        spacing: 8

                                        Label {
                                            text: "Can I export my test results?"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                        }
                                        Label {
                                            text: faq5.expanded ? "▲" : "▼"
                                            color: "#9ca3af"
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: faq5.expanded
                                    spacing: 4

                                    Label {
                                        text: "Yes — use the Results Panel export options (CSV/JSON). Exported data typically includes truth tables, pin mappings, summaries, and status messages."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }

                                    Label {
                                        text: "You can also export a PDF report that includes all the test results in a more readable format in the Result Panel."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // ===================== QUICK REFERENCE =====================
            Frame {
                Layout.fillWidth: true
                padding: 16

                contentItem: ColumnLayout {
                    spacing: 16
                    Layout.fillWidth: true

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Label {
                            text: "Quick Reference"
                            font.pixelSize: 18
                            font.bold: true
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        Label {
                            text: "Key terminology and concepts"
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 24
                        rowSpacing: 12

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            Label {
                                text: "Configuration Terms"
                                font.pixelSize: 14
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label { text: "Pin Mapping"; font.pixelSize: 12; color: "#6b7280" }
                            Label {
                                text: "Assignment of physical pins on the chip to aliases that can represent one or more physical pins."
                                font.pixelSize: 12
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Item { implicitHeight: 6 }

                            Label { text: "Logic Type"; font.pixelSize: 12; color: "#6b7280" }
                            Label {
                                text: "The type of logical operation the IC performs (set to N/A if IC does not perform a logical operation)."
                                font.pixelSize: 12
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            Label {
                                text: "Testing Terms"
                                font.pixelSize: 14
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label { text: "Truth Table"; font.pixelSize: 12; color: "#6b7280" }
                            Label {
                                text: "Complete list of input and output combinations for an IC."
                                font.pixelSize: 12
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Item { implicitHeight: 6 }

                            Label { text: "Test"; font.pixelSize: 12; color: "#6b7280" }
                            Label {
                                text: "A mapping of binary values to each pin on the IC including the values for the input pins to be set to and the expected values for the output pins."
                                font.pixelSize: 12
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }

            // ===================== ERROR CODES =====================
            Frame {
                Layout.fillWidth: true
                padding: 16

                contentItem: ColumnLayout {
                    spacing: 16
                    Layout.fillWidth: true

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Label {
                            text: "Error Codes"
                            font.pixelSize: 18
                            font.bold: true
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        Label {
                            text: "Information for interpreting hardware self-test errors."
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        radius: 8
                        color: "#f9fafb"
                        border.color: "#e5e7eb"
                        implicitHeight: errorIntro.implicitHeight + 24

                        ColumnLayout {
                            id: errorIntro
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8

                            Label {
                                text: "The hardware module may generate error codes when issues occur with it. This section explains those error codes."
                                color: "#4b5563"
                                font.pixelSize: 12
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "Error Code Format:"
                                font.bold: true
                                color: "#111827"
                                font.pixelSize: 12
                                Layout.fillWidth: true
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                radius: 6
                                color: "white"
                                border.color: "#e5e7eb"
                                implicitHeight: formatLabel.implicitHeight + 16

                                Label {
                                    id: formatLabel
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    text: "# #"
                                    font.family: "monospace"
                                    font.pixelSize: 12
                                    color: "#111827"
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap
                                }
                            }

                            Label {
                                text: "Each error is displayed as a one or two-digit numeric code from 0 to 13."
                                color: "#4b5563"
                                font.pixelSize: 12
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 24
                        rowSpacing: 8

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Error Codes"
                                font.pixelSize: 14
                                font.bold: true
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "0 = No Errors (All Tests Passed)​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "1 = Memory Check Failed​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "2 = Clock Check Failed​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "3 = 3.3V Rail Failure​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "4 = 5V (VBUS) Rail Failure​​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "5-6 = Chip Power Pin Failures​​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "7-11 = Chip Ground Pin Failures​​​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "12 = ADC communication Failure​​​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "13 = Loopback Test Failure​​​​"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }

            // ===================== FULL DOCUMENTATION =====================
            Frame {
                Layout.fillWidth: true
                padding: 16

                contentItem: ColumnLayout {
                    spacing: 16
                    Layout.fillWidth: true

                    ColumnLayout {
                        spacing: 4
                        Layout.fillWidth: true

                        Label {
                            text: "Full Documentation"
                            font.pixelSize: 18
                            font.bold: true
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }

                        Label {
                            text: "Access comprehensive guides, API references, and advanced tutorials"
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Button {
                            Layout.fillWidth: true
                            text: "User Guide"
                            onClicked: Qt.openUrlExternally("https://drive.google.com/file/d/1K6kCfqXsqGyKT_TuXDRhznH97W_paCgV/view?usp=sharing")
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "Configuration File Guide"
                            onClicked: Qt.openUrlExternally("https://drive.google.com/file/d/1LgW0SZKzwybiLIX7XmWH--5rLOyz9nct/view?usp=sharing")
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "Hardware Setup Instructions"
                            onClicked: Qt.openUrlExternally("https://www.google.com")
                        }

                        Button {
                            Layout.fillWidth: true
                            text: "API Reference"
                            onClicked: Qt.openUrlExternally("https://drive.google.com/file/d/1z6-LVaGpmzkCh-8YR19zF7n3OQbKFKjl/view?usp=sharing")
                        }
                    }
                }
            }
        }
    }
}