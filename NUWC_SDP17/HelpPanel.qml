//written by Ben

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent

    ScrollView {
        anchors.fill: parent

        ColumnLayout {
            anchors.margins: 24
            spacing: 24
            width: parent.width

            // Header
            Frame {
                Layout.fillWidth: true

                background: Rectangle {
                    color: "#e8f1ff"
                    radius: 8
                    border.color: "#bfdbfe"
                }

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 6

                    Label {
                        text: "Help & Documentation"
                        font.pixelSize: 20
                        font.bold: true
                    }

                    Label {
                        text: "Learn how to use the Circuit GUI Prototype - from setup to analysis"
                        color: "#4b5563"
                        wrapMode: Text.WordWrap
                    }
                }
            }

            // Quick start
            Frame {
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    ColumnLayout {
                        spacing: 4
                        Label {
                            text: "Quick Start: Testing Workflow"
                            font.pixelSize: 18
                            font.bold: true
                        }
                        Label {
                            text: "Follow these steps to test your circuit configuration"
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        // Step 1
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            color: "#f9fafb"
                            border.color: "#e5e7eb"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                Rectangle {
                                    width: 40; height: 40; radius: 8
                                    color: "#dbeafe"
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
                                        Label { text: "Load Configuration"; color: "#111827" }
                                    }
                                    Label {
                                        text: "Navigate to the Configuration Panel and select your circuit configuration file (.xml or .json) and test script file (.txt or .script)."
                                        color: "#4b5563"
                                        font.pixelSize: 12
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }

                        // Step 2
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            color: "#f9fafb"
                            border.color: "#e5e7eb"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                Rectangle {
                                    width: 40; height: 40; radius: 8
                                    color: "#dcfce7"
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
                                        Label { text: "Execute Tests"; color: "#111827" }
                                    }
                                    Label {
                                        text: "Go to the Test Control Panel, choose between Simulation or Hardware mode, and click \"Start Test\" to run your test script."
                                        color: "#4b5563"
                                        font.pixelSize: 12
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }

                        // Step 3
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            color: "#f5f3ff"
                            border.color: "#e5e7eb"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12

                                Rectangle {
                                    width: 40; height: 40; radius: 8
                                    color: "#ede9fe"
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
                                        Label { text: "Review Results"; color: "#111827" }
                                    }
                                    Label {
                                        text: "View your test results, truth tables, and pin mappings in the Results Panel. Export data as needed for further analysis."
                                        color: "#4b5563"
                                        font.pixelSize: 12
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // FAQ (accordion-style)
            Frame {
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    ColumnLayout {
                        spacing: 4
                        Label {
                            text: "Frequently Asked Questions"
                            font.pixelSize: 18
                            font.bold: true
                        }
                        Label {
                            text: "Common questions about file types, configuration, and testing"
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        // Helper component-ish pattern: copy for each FAQ
                        // FAQ 1
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                MouseArea {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: questionRow1.implicitHeight
                                    onClicked: parentItem.expanded = !parentItem.expanded
                                }

                                RowLayout {
                                    id: questionRow1
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Label {
                                        text: "What file types are supported?"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: parentItem.expanded ? "▲" : "▼"
                                        color: "#9ca3af"
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: parentItem.expanded
                                    spacing: 4

                                    Label {
                                        text: "Configuration Files: XML (.xml) or JSON (.json) files containing circuit parameters, chip specifications, pin counts, and logic type definitions."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }
                                    Label {
                                        text: "Test Script Files: Plain text (.txt) or script (.script) files that define the sequence of test operations, input patterns, and expected outputs."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }

                        // FAQ 2
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                MouseArea {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: questionRow2.implicitHeight
                                    onClicked: parentItem.expanded = !parentItem.expanded
                                }

                                RowLayout {
                                    id: questionRow2
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Label {
                                        text: "What's the difference between Simulation and Hardware mode?"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: parentItem.expanded ? "▲" : "▼"
                                        color: "#9ca3af"
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: parentItem.expanded
                                    spacing: 4

                                    Label {
                                        text: "Simulation Mode: Tests your circuit design using software simulation. No physical hardware required. Ideal for rapid prototyping and testing logic before hardware implementation."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }
                                    Label {
                                        text: "Hardware Mode: Tests with connected physical hardware (FPGA, breadboard, or custom circuit). Requires proper hardware connections and drivers to be configured."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }

                        // FAQ 3
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                MouseArea {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: questionRow3.implicitHeight
                                    onClicked: parentItem.expanded = !parentItem.expanded
                                }

                                RowLayout {
                                    id: questionRow3
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Label {
                                        text: "How do I configure circuit parameters?"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: parentItem.expanded ? "▲" : "▼"
                                        color: "#9ca3af"
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: parentItem.expanded
                                    spacing: 4

                                    Label {
                                        text: "Circuit parameters are defined in your configuration file. Key parameters include:"
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }

                                    ColumnLayout {
                                        spacing: 2
                                        Label { text: "• Chip Number: Identifier for the IC being tested"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• Logic Type: Type of logic (TTL, CMOS, etc.)"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• Number of Inputs: How many input pins"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• Pin Count: Total number of pins on the chip"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• Description: Optional notes about the circuit"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                    }

                                    Label {
                                        text: "You can edit these directly in the configuration file or use the \"Edit Parameters\" button in the Configuration Panel."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }

                        // FAQ 4
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                MouseArea {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: questionRow4.implicitHeight
                                    onClicked: parentItem.expanded = !parentItem.expanded
                                }

                                RowLayout {
                                    id: questionRow4
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Label {
                                        text: "How do I interpret truth table results?"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: parentItem.expanded ? "▲" : "▼"
                                        color: "#9ca3af"
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: parentItem.expanded
                                    spacing: 4

                                    Label {
                                        text: "Truth tables display the relationship between inputs and outputs:"
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }

                                    ColumnLayout {
                                        spacing: 2
                                        Label { text: "• Input columns: Show the input values applied during testing"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• Output column: Shows the actual output received"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• Expected column: Shows what the output should be"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• Status: Indicates PASS (output matches expected) or FAIL (mismatch)"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                    }

                                    Label {
                                        text: "Failed tests may indicate wiring issues, faulty components, or incorrect configuration."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }

                        // FAQ 5
                        Rectangle {
                            Layout.fillWidth: true
                            radius: 8
                            border.color: "#e5e7eb"
                            color: "white"
                            property bool expanded: false

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                MouseArea {
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: questionRow5.implicitHeight
                                    onClicked: parentItem.expanded = !parentItem.expanded
                                }

                                RowLayout {
                                    id: questionRow5
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Label {
                                        text: "Can I export my test results?"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        text: parentItem.expanded ? "▲" : "▼"
                                        color: "#9ca3af"
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    visible: parentItem.expanded
                                    spacing: 4

                                    Label {
                                        text: "Yes! In the Results Panel, you can export test data in multiple formats:"
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }

                                    ColumnLayout {
                                        spacing: 2
                                        Label { text: "• CSV Export: For Excel, Google Sheets, etc."; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                        Label { text: "• JSON Export: For programmatic integration"; font.pixelSize: 12; color: "#4b5563"; wrapMode: Text.WordWrap }
                                    }

                                    Label {
                                        text: "Exported data includes truth tables, pin mappings, test summaries, and status messages."
                                        font.pixelSize: 12
                                        color: "#4b5563"
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Quick reference + documentation buttons
            Frame {
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 16

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            Label {
                                text: "Quick Reference"
                                font.pixelSize: 18
                                font.bold: true
                            }
                            Label {
                                text: "Key terminology and concepts"
                                color: "#6b7280"
                                wrapMode: Text.WordWrap
                            }
                        }
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 24
                        rowSpacing: 12

                        ColumnLayout {
                            spacing: 4
                            Label {
                                text: "Configuration Terms"
                                font.pixelSize: 14
                                color: "#111827"
                            }

                            ColumnLayout {
                                spacing: 4
                                Label { text: "Pin Mapping"; font.pixelSize: 12; color: "#6b7280" }
                                Label { text: "Assignment of functions to physical pins on the chip"; font.pixelSize: 12; color: "#111827"; wrapMode: Text.WordWrap }
                                Label { text: "Logic Type"; font.pixelSize: 12; color: "#6b7280"; topPadding: 8 }
                                Label { text: "The family of logic circuits (TTL, CMOS, etc.)"; font.pixelSize: 12; color: "#111827"; wrapMode: Text.WordWrap }
                            }
                        }

                        ColumnLayout {
                            spacing: 4
                            Label {
                                text: "Testing Terms"
                                font.pixelSize: 14
                                color: "#111827"
                            }

                            ColumnLayout {
                                spacing: 4
                                Label { text: "Truth Table"; font.pixelSize: 12; color: "#6b7280" }
                                Label { text: "Complete list of input/output combinations for a circuit"; font.pixelSize: 12; color: "#111827"; wrapMode: Text.WordWrap }
                                Label { text: "Test Script"; font.pixelSize: 12; color: "#6b7280"; topPadding: 8 }
                                Label { text: "Sequence of operations to verify circuit behavior"; font.pixelSize: 12; color: "#111827"; wrapMode: Text.WordWrap }
                            }
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Label {
                            text: "Full Documentation"
                            font.pixelSize: 18
                            font.bold: true
                        }
                        Label {
                            text: "Access comprehensive guides, API references, and advanced tutorials"
                            color: "#6b7280"
                            wrapMode: Text.WordWrap
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
