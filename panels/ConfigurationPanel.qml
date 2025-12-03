//written by Ben

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            id: mainLayout
            width: scrollView.availableWidth
            anchors.margins: 24
            spacing: 16

            // ============================================================
            // HEADER CARD
            // ============================================================
            Frame {
                id: headerFrame
                Layout.fillWidth: true
                padding: 16
                // Make frame tall enough for its content
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

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        Label {
                            text: "Configuration & File Selection"
                            font.bold: true
                            font.pixelSize: 20
                        }

                        ToolButton {
                            text: "?"
                            font.pixelSize: 12
                            ToolTip.visible: hovered
                            ToolTip.text: "Load your circuit configuration and test script files to begin. These files define your circuit parameters and testing procedures."
                        }
                    }

                    Label {
                        text: "Load configuration files and test scripts to set up your circuit testing environment"
                        color: "#4b5563"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }
            }

            // ============================================================
            // MIDDLE: FILE SELECTION + RECENT FILES
            // ============================================================
            RowLayout {
                Layout.fillWidth: true
                spacing: 24

                // LEFT CARD: File Selection
                Frame {
                    id: fileFrame
                    Layout.fillWidth: true
                    padding: 16
                    implicitHeight: Math.max(fileCol.implicitHeight, recentCol.implicitHeight) + padding * 2
                    ColumnLayout {
                        id: fileCol
                        anchors.fill: parent
                        spacing: 16

                        ColumnLayout {
                            spacing: 4
                            Label {
                                text: "File Selection"
                                font.bold: true
                                font.pixelSize: 18
                            }
                            Label {
                                text: "Select your configuration and test script files"
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }

                        // Configuration file
                        ColumnLayout {
                            spacing: 4

                            RowLayout {
                                Layout.fillWidth: true

                                Label {
                                    text: "Configuration File"
                                    font.bold: true
                                }

                                Item { Layout.fillWidth: true }

                                Label {
                                    text: ".xml, .json"
                                    color: "#9ca3af"
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                TextField {
                                    Layout.fillWidth: true
                                    placeholderText: "No file selected"
                                    readOnly: true
                                }

                                Button {
                                    text: "Browse"
                                }
                            }
                        }

                        // Test script file
                        ColumnLayout {
                            spacing: 4

                            RowLayout {
                                Layout.fillWidth: true

                                Label {
                                    text: "Test Script File"
                                    font.bold: true
                                }

                                Item { Layout.fillWidth: true }

                                Label {
                                    text: ".txt, .script"
                                    color: "#9ca3af"
                                }
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                TextField {
                                    Layout.fillWidth: true
                                    placeholderText: "No file selected"
                                    readOnly: true
                                }

                                Button {
                                    text: "Browse"
                                }
                            }
                        }

                        // Action buttons (these were getting clipped before)
                        RowLayout {
                            spacing: 12
                            Layout.topMargin: 8

                            Button {
                                text: "Load Configuration"
                            }

                            Button {
                                text: "Edit Parameters"
                                flat: true
                            }
                        }
                    }
                }

                // RIGHT CARD: Recent Files
                Frame {
                    id: recentFrame
                    Layout.preferredWidth: 320
                    Layout.alignment: Qt.AlignTop
                    padding: 16
                    implicitHeight: Math.max(fileCol.implicitHeight, recentCol.implicitHeight) + padding * 2

                    ColumnLayout {
                        id: recentCol
                        anchors.fill: parent
                        spacing: 12

                        ColumnLayout {
                            spacing: 4

                            RowLayout {
                                spacing: 8
                                Label {
                                    text: "Recent Files"
                                    font.bold: true
                                    font.pixelSize: 18
                                }
                            }

                            Label {
                                text: "Quick access to recently used configurations"
                                color: "#4b5563"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }

                        ColumnLayout {
                            spacing: 8

                            Repeater {
                                model: [
                                    { "name": "circuit_config_v3.xml", "date": "2 hours ago" },
                                    { "name": "test_script_001.txt", "date": "Yesterday" },
                                    { "name": "fpga_setup.json", "date": "3 days ago" }
                                ]

                                delegate: Rectangle {
                                    Layout.fillWidth: true
                                    implicitHeight: 52
                                    radius: 6
                                    color: hovered ? "#eff6ff" : "white"
                                    border.color: hovered ? "#60a5fa" : "#e5e7eb"
                                    border.width: 1

                                    property bool hovered: false

                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onEntered: parent.hovered = true
                                        onExited: parent.hovered = false
                                    }

                                    Column {
                                        anchors.fill: parent
                                        anchors.margins: 8
                                        spacing: 2

                                        Text {
                                            text: modelData["name"]
                                            font.pixelSize: 14
                                            color: "#111827"
                                            elide: Text.ElideRight
                                        }
                                        Text {
                                            text: modelData["date"]
                                            font.pixelSize: 11
                                            color: "#6b7280"
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // ============================================================
            // BOTTOM: CURRENTLY LOADED CONFIGURATION
            // ============================================================
            Frame {
                id: currentFrame
                Layout.fillWidth: true
                padding: 16
                implicitHeight: currentCol.implicitHeight + padding * 2

                ColumnLayout {
                    id: currentCol
                    anchors.fill: parent
                    spacing: 16

                    ColumnLayout {
                        spacing: 4
                        Label {
                            text: "Currently Loaded Configuration"
                            font.pixelSize: 18
                            font.bold: true
                        }
                        Label {
                            text: "Overview of loaded circuit parameters and test setup"
                            color: "#4b5563"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }

                    Rectangle {
                        id: currentCard
                        Layout.fillWidth: true
                        radius: 8
                        color: "#f9fafb"
                        border.color: "#e5e7eb"

                        implicitHeight: cardCol.implicitHeight + 32

                        ColumnLayout {
                            id: cardCol
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 16

                            // 4 columns of stats
                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 32

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Chip Number"; font.pixelSize: 12; color: "#6b7280" }
                                    Label { text: "Not loaded"; color: "#111827" }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Logic Type"; font.pixelSize: 12; color: "#6b7280" }
                                    Label { text: "Not loaded"; color: "#111827" }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Number of Inputs"; font.pixelSize: 12; color: "#6b7280" }
                                    Label { text: "-"; color: "#111827" }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Pin Count"; font.pixelSize: 12; color: "#6b7280" }
                                    Label { text: "-"; color: "#111827" }
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                height: 1
                                color: "#e5e7eb"
                            }

                            ColumnLayout {
                                spacing: 4
                                Label {
                                    text: "Description"
                                    font.pixelSize: 12
                                    color: "#6b7280"
                                }
                                Label {
                                    text: "Load a configuration file to view details"
                                    font.italic: true
                                    color: "#9ca3af"
                                    wrapMode: Text.WordWrap
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
