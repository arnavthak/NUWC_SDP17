//written by Ben

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: root
    anchors.fill: parent

    property var chipConfig: ({})
    property string selectedFileName: ""
    property string result: ""
    property url selectedFile: fileDialog.selectedFile
    property double currentTime: Date.now()

    function addRecentFile(filePath) {
        console.log("Adding recent file...")

        let name = filePath.split("/").pop()

        let entry = {
            "name": name,
            "date": Date.now(),
            "path": filePath
        }

        // Remove duplicate if already exists
        for (let i = 0; i < recentFilesModel.count; i++) {
            if (recentFilesModel.get(i).path === filePath) {
                recentFilesModel.remove(i)
                break
            }
        }

        // Insert at top
        recentFilesModel.insert(0, entry)

        // Limit list size
        if (recentFilesModel.count > 10) {
            recentFilesModel.remove(recentFilesModel.count - 1)
        }

        saveRecentFiles()
    }

    function saveRecentFiles() {
        console.log("Saving recent files...")

        let files = []

        for (let i = 0; i < recentFilesModel.count; i++) {
            let f = recentFilesModel.get(i)

            // Create clean object (NOT QML object)
            files.push({
                name: f.name,
                date: f.date,
                path: f.path
            })
        }

        recentFilesManager.saveRecentFiles(files)
    }

    function formatDate(timestamp, now) {
        let diff = Math.floor((now - timestamp) / 1000)
        console.log("diff " + diff + "\n");
        console.log("now " + now + "\n");
        console.log("timestamp " + timestamp + "\n");

        if (diff < 60)
            return "Just now"

        if (diff < 3600) {
            let m = Math.floor(diff / 60)
            return m + (m === 1 ? " min ago" : " mins ago")
        }

        if (diff < 86400) {
            let h = Math.floor(diff / 3600)
            return h + (h === 1 ? " hr ago" : " hrs ago")
        }

        if (diff < 604800) {
            let d = Math.floor(diff / 86400)
            return d + (d === 1 ? " day ago" : " days ago")
        }

        let date = new Date(timestamp)
        return Qt.formatDateTime(date, "MMM d, yyyy")
    }

    Connections {
        target: yamlProcessor

        function onYamlLoaded(filePath) {
            addRecentFile(filePath.toString())
        }
    }

    ListModel {
        id: recentFilesModel
    }

    Component.onCompleted: {
        let files = recentFilesManager.loadRecentFiles()
        console.log("Loaded files:", JSON.stringify(files))

        for (let i = 0; i < files.length; i++) {
            let f = files[i]

            // Skip bad entries
            if (!f || !f.name || !f.path)
                continue

            // Ensure it's a plain JS object
            recentFilesModel.append({
                name: f.name,
                date: f.date,
                path: f.path
            })
        }
    }

    Timer {
        interval: 60000
        running: true
        repeat: true
        onTriggered: currentTime = Date.now()
    }

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
                                    text: selectedFileName.length > 0 ? selectedFileName : ""
                                    placeholderText: "No file selected"
                                    readOnly: true
                                }

                                Button {
                                    text: "Browse"
                                    onClicked: {
                                        fileDialog.open();
                                    }
                                }

                                FileDialog {
                                    id: fileDialog
                                    nameFilters: ["YAML Files (*.yaml *.yml)", "All Files (*)"]
                                    currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
                                }

                                Connections {
                                    target: fileDialog
                                    onAccepted: {
                                        selectedFileName = fileDialog.selectedFile.toString().split("/").pop()
                                    }
                                }
                            }
                        }

                        // Action buttons (these were getting clipped before)
                        RowLayout {
                            spacing: 12
                            Layout.topMargin: 8

                            Button {
                                text: "Load Configuration"
                                onClicked: {
                                    chipConfig = yamlProcessor.loadYaml(fileDialog.selectedFile);
                                    console.log(JSON.stringify(chipConfig, null, 2));

                                    //result = serialComms.sendTestStream("PWRALL");
                                    //console.log(result);
                                }
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
                                model: recentFilesModel

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

                                        onClicked: {
                                            // Set the selected file (this is what your Load button uses)
                                            fileDialog.selectedFile = path

                                            // Update the text field display
                                            selectedFileName = path.split("/").pop()
                                        }

                                        onEntered: parent.hovered = true
                                        onExited: parent.hovered = false
                                    }

                                    Column {
                                        anchors.fill: parent
                                        anchors.margins: 8
                                        spacing: 2

                                        Text {
                                            text: name
                                            font.pixelSize: 14
                                            color: "#111827"
                                            elide: Text.ElideRight
                                        }
                                        Text {
                                            text: formatDate(date, currentTime)
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
                                    Label {
                                        text: chipConfig.chipInfo && chipConfig.chipInfo["Chip Number"]
                                              ? chipConfig.chipInfo["Chip Number"]
                                              : "Not loaded"
                                        color: "#111827"
                                    }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Logic Type"; font.pixelSize: 12; color: "#6b7280" }
                                    Label {
                                        text: chipConfig.chipInfo && chipConfig.chipInfo["Logic Type"]
                                              ? chipConfig.chipInfo["Logic Type"]
                                              : "Not loaded"
                                        color: "#111827"
                                    }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Number of Inputs"; font.pixelSize: 12; color: "#6b7280" }
                                    Label {
                                        text: chipConfig.chipInfo && chipConfig.chipInfo["Number of Inputs"]
                                              ? chipConfig.chipInfo["Number of Inputs"]
                                              : "-"
                                        color: "#111827"
                                    }
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Label { text: "Pin Count"; font.pixelSize: 12; color: "#6b7280" }
                                    Label {
                                        text: chipConfig.chipInfo && chipConfig.chipInfo["Pin Count"]
                                              ? chipConfig.chipInfo["Pin Count"]
                                              : "-"
                                        color: "#111827"
                                    }
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
                                    text: chipConfig.chipInfo && chipConfig.chipInfo["Description"]
                                          ? chipConfig.chipInfo["Description"]
                                          : "-"
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
