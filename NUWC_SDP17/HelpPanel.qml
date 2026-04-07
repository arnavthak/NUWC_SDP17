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
                        text: "Learn how to use the Circuit GUI Prototype - from setup to analysis"
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
                                        text: "Navigate to the Configuration Panel and select your circuit configuration file (.xml or .json) and test script file (.txt or .script)."
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
                                        text: "Configuration Files: XML (.xml) or JSON (.json) files containing circuit parameters, chip specifications, pin counts, and logic type definitions."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: "Test Script Files: Plain text (.txt) or script (.script) files that define the sequence of test operations, input patterns, and expected outputs."
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
                                        text: "Simulation Mode: Tests your circuit design using software simulation. No physical hardware required."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: "Hardware Mode: Tests with connected physical hardware. Requires proper hardware connections and drivers."
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
                                        text: "Circuit parameters are defined in your configuration file (chip number, logic type, pin count, etc.). You can edit the file directly or use the Configuration Panel controls (if implemented)."
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
                                        text: "Compare Output vs Expected. PASS means they match; FAIL means mismatch. Frequent FAILs can indicate wiring issues, incorrect configuration, or a faulty component."
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
                                        text: "Yes — use the Results Panel export options (CSV/JSON if implemented). Exported data typically includes truth tables, pin mappings, summaries, and status messages."
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
                                text: "Assignment of functions to physical pins on the chip"
                                font.pixelSize: 12
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Item { implicitHeight: 6 }

                            Label { text: "Logic Type"; font.pixelSize: 12; color: "#6b7280" }
                            Label {
                                text: "The family of logic circuits (TTL, CMOS, etc.)"
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
                                text: "Complete list of input/output combinations for a circuit"
                                font.pixelSize: 12
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Item { implicitHeight: 6 }

                            Label { text: "Test Script"; font.pixelSize: 12; color: "#6b7280" }
                            Label {
                                text: "Sequence of operations to verify circuit behavior"
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
                            text: "Reference information for system errors and future troubleshooting documentation"
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
                                text: "During testing, the system may generate error codes when issues occur with configuration files, test scripts, hardware connections, or internal execution logic. This section provides a place for those codes to be documented and explained."
                                color: "#4b5563"
                                font.pixelSize: 12
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "Current placeholder format:"
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
                                    text: "##"
                                    font.family: "monospace"
                                    font.pixelSize: 12
                                    color: "#111827"
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap
                                }
                            }

                            Label {
                                text: "Each error is displayed as a two-digit numeric code from 00 to 99."
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
                                text: "Suggested Groups"
                                font.pixelSize: 14
                                font.bold: true
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "00-19  Configuration-related errors"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "20-39  Test script and execution errors"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "40-59  Hardware connection or communication errors"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "60-99  Reserved for future system errors"
                                font.pixelSize: 12
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Planned Documentation Fields"
                                font.pixelSize: 14
                                font.bold: true
                                color: "#111827"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            Label {
                                text: "• 2-digit error code"
                                font.pixelSize: 12
                                color: "#4b5563"
                            }

                            Label {
                                text: "• Short description"
                                font.pixelSize: 12
                                color: "#4b5563"
                            }

                            Label {
                                text: "• Likely cause"
                                font.pixelSize: 12
                                color: "#4b5563"
                            }

                            Label {
                                text: "• Recommended user action"
                                font.pixelSize: 12
                                color: "#4b5563"
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        radius: 8
                        color: "white"
                        border.color: "#e5e7eb"
                        implicitHeight: errorTable.implicitHeight + 24

                        ColumnLayout {
                            id: errorTable
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 0

                            Rectangle {
                                Layout.fillWidth: true
                                color: "#f9fafb"
                                border.color: "#e5e7eb"
                                implicitHeight: headerRow.implicitHeight + 16

                                RowLayout {
                                    id: headerRow
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    spacing: 16

                                    Label {
                                        text: "Code"
                                        font.bold: true
                                        Layout.preferredWidth: 80
                                    }

                                    Label {
                                        text: "Description"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }

                                    Label {
                                        text: "Possible Cause"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                color: "white"
                                border.color: "#e5e7eb"
                                implicitHeight: row1.implicitHeight + 16

                                RowLayout {
                                    id: row1
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    spacing: 16

                                    Label {
                                        text: "01"
                                        font.family: "monospace"
                                        Layout.preferredWidth: 80
                                        wrapMode: Text.WordWrap
                                    }

                                    Label {
                                        text: "Configuration file failed to load"
                                        Layout.fillWidth: true
                                        wrapMode: Text.WordWrap
                                    }

                                    Label {
                                        text: "Missing file, invalid path, or unsupported format"
                                        Layout.fillWidth: true
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                color: "white"
                                border.color: "#e5e7eb"
                                implicitHeight: row2.implicitHeight + 16

                                RowLayout {
                                    id: row2
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    spacing: 16

                                    Label {
                                        text: "21"
                                        font.family: "monospace"
                                        Layout.preferredWidth: 80
                                        wrapMode: Text.WordWrap
                                    }

                                    Label {
                                        text: "Test script parsing error"
                                        Layout.fillWidth: true
                                        wrapMode: Text.WordWrap
                                    }

                                    Label {
                                        text: "Invalid syntax or unsupported command in test script"
                                        Layout.fillWidth: true
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                color: "#fafafa"
                                border.color: "#e5e7eb"
                                implicitHeight: placeholderRow.implicitHeight + 16

                                Label {
                                    id: placeholderRow
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    text: "Additional two-digit error codes will be documented here as they are defined."
                                    color: "#9ca3af"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    wrapMode: Text.WordWrap
                                }
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

                        Button { Layout.fillWidth: true; text: "Complete User Manual" }
                        Button { Layout.fillWidth: true; text: "Script Writing Guide" }
                        Button { Layout.fillWidth: true; text: "Hardware Setup Instructions" }
                        Button { Layout.fillWidth: true; text: "API Reference" }
                    }
                }
            }
        }
    }
}