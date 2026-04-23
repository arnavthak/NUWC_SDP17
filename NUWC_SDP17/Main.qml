//written by Ben

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 1200
    height: 800
    title: "Circuit GUI Prototype - Redesign Wireframes"

    Rectangle {
        anchors.fill: parent
        color: "#f9fafb"   // light gray background

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 16

            // Header
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    text: "Circuit GUI Prototype - Redesign Wireframes"
                    font.pixelSize: 22
                    font.bold: true
                    color: "#111827"   // gray-900
                }

                Label {
                    text: "Improved interface addressing feedback on clarity, integration, and contextual help"
                    color: "#4b5563"   // gray-600
                    wrapMode: Text.WordWrap
                }
            }

            // Tabs
            TabBar {
                id: tabBar
                Layout.fillWidth: true

                TabButton { text: "Configuration Panel" }
                TabButton { text: "Test Control Panel" }
                TabButton { text: "Results Panel" }
                TabButton { text: "Help Panel" }
            }

            // Content
            StackLayout {
                id: stackLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: tabBar.currentIndex

                ConfigurationPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: configPanel
                }

                TestControlPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    selectedFile: configPanel.selectedFile
                    totalTests: configPanel.chipConfig["tests"].length
                    id: testPanel
                }

                ResultsPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    results: testPanel.results
                    rawPinConfigs: configPanel.chipConfig["rawPinConfigs"]
                    pinValues: configPanel.chipConfig["pinValues"]
                    isSimulation: testPanel.isSimulation
                    chipInfo: configPanel.chipConfig["chipInfo"]
                }

                HelpPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
