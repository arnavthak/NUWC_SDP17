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
        contentWidth: availableWidth    // <<< critical so content fills viewport

        ColumnLayout {
            id: mainLayout
            width: scrollView.availableWidth   // <<< use full width
            anchors.margins: 24
            spacing: 24

            // ============================================================
            // HEADER CARD
            // ============================================================
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
                        text: "Test Results & Mappings"
                        font.pixelSize: 20
                        font.bold: true
                    }

                    Label {
                        text: "View test outcomes, truth tables, and circuit mappings generated from your tests"
                        color: "#4b5563"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }
            }

            // ============================================================
            // SUMMARY TILES (Total / Passed / Failed / Success Rate)
            // ============================================================
            GridLayout {
                Layout.fillWidth: true
                columns: 4
                columnSpacing: 16
                rowSpacing: 16

                Repeater {
                    model: [
                        { "label": "Total Tests", "value": "0" },
                        { "label": "Passed",      "value": "0" },
                        { "label": "Failed",      "value": "0" },
                        { "label": "Success Rate","value": "-" }
                    ]

                    delegate: Frame {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 96
                        padding: 12

                        background: Rectangle {
                            color: "#ffffff"
                            radius: 8
                            border.color: "#e5e7eb"
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 4

                            Label {
                                text: modelData["label"]
                                font.pixelSize: 12
                                color: "#6b7280"
                            }
                            Label {
                                text: modelData["value"]
                                font.pixelSize: 18
                                color: "#111827"
                            }
                        }
                    }
                }
            }

            // ============================================================
            // MAIN RESULTS CARD (Tabs + Table)
            // ============================================================
            Frame {
                id: mainResultsFrame
                Layout.fillWidth: true
                padding: 16
                implicitHeight: mainCol.implicitHeight + padding * 2

                background: Rectangle {
                    color: "#ffffff"
                    radius: 12
                    border.color: "#d1d5db"
                }

                ColumnLayout {
                    id: mainCol
                    anchors.fill: parent
                    spacing: 16

                    // Title + export buttons
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2

                            Label {
                                text: "Test Results & Truth Tables"
                                font.pixelSize: 18
                                font.bold: true
                            }
                            Label {
                                text: "Detailed view of test outcomes and generated mappings"
                                color: "#6b7280"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }
                        }

                        RowLayout {
                            spacing: 8
                            Button { text: "Export CSV"; enabled: false }
                            Button { text: "Export JSON"; enabled: false }
                        }
                    }

                    // Tabs
                    TabBar {
                        id: resultsTabBar
                        Layout.fillWidth: true

                        TabButton { text: "Truth Table" }
                        TabButton { text: "Pin Mapping" }
                        TabButton { text: "Test Results" }
                    }

                    StackLayout {
                        id: resultsStack
                        Layout.fillWidth: true
                        Layout.preferredHeight: 260
                        currentIndex: resultsTabBar.currentIndex

                        // ---------------- Truth Table tab ----------------
                        Item {
                            anchors.fill: parent

                            Frame {
                                anchors.fill: parent
                                padding: 12

                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 8

                                    // Header row: spread evenly across width
                                    RowLayout {
                                        id: truthHeaderRow
                                        Layout.fillWidth: true
                                        spacing: 0

                                        Label {
                                            text: "Input A"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Input B"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Output"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Expected"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Status"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }

                                    Rectangle {
                                        Layout.fillWidth: true
                                        radius: 4
                                        color: "#f9fafb"
                                        border.color: "#e5e7eb"
                                        implicitHeight: 80

                                        Label {
                                            anchors.centerIn: parent
                                            text: "No truth table data available. Run a test to generate results."
                                            color: "#9ca3af"
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }
                                }
                            }
                        }

                        // ---------------- Pin Mapping tab ----------------
                        Item {
                            anchors.fill: parent

                            Frame {
                                anchors.fill: parent
                                padding: 12

                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 8

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 0

                                        Label {
                                            text: "Pin Number"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Function"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Type"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Description"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }

                                    Rectangle {
                                        Layout.fillWidth: true
                                        radius: 4
                                        color: "#f9fafb"
                                        border.color: "#e5e7eb"
                                        implicitHeight: 80

                                        Label {
                                            anchors.centerIn: parent
                                            text: "No pin mapping data available. Load a configuration to view mappings."
                                            color: "#9ca3af"
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }
                                }
                            }
                        }

                        // ---------------- Test Results tab ----------------
                        Item {
                            anchors.fill: parent

                            Frame {
                                anchors.fill: parent
                                padding: 12

                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 8

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 0

                                        Label {
                                            text: "Test Name"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Status"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Duration"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                        Label {
                                            text: "Message"
                                            font.bold: true
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }

                                    Rectangle {
                                        Layout.fillWidth: true
                                        radius: 4
                                        color: "#f9fafb"
                                        border.color: "#e5e7eb"
                                        implicitHeight: 80

                                        Label {
                                            anchors.centerIn: parent
                                            text: "No test results available. Execute tests to view results."
                                            color: "#9ca3af"
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // ============================================================
            // TEST SUMMARY CARD
            // ============================================================
            Frame {
                id: summaryFrame
                Layout.fillWidth: true
                padding: 16
                implicitHeight: summaryCol.implicitHeight + padding * 2

                background: Rectangle {
                    color: "#ffffff"
                    radius: 12
                    border.color: "#d1d5db"
                }

                ColumnLayout {
                    id: summaryCol
                    anchors.fill: parent
                    spacing: 12

                    ColumnLayout {
                        spacing: 4
                        Label {
                            text: "Test Summary"
                            font.pixelSize: 18
                            font.bold: true
                        }
                        Label {
                            text: "Overview and status messages from the latest test run"
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

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 12

                            Rectangle {
                                width: 40
                                height: 40
                                radius: 999
                                color: "#e5e7eb"
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                Label {
                                    text: "No test summary available"
                                    color: "#111827"
                                }
                                Label {
                                    text: "Run a test to generate a summary report with detailed status messages, warnings, and recommendations."
                                    color: "#6b7280"
                                    wrapMode: Text.WordWrap
                                    font.pixelSize: 12
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
