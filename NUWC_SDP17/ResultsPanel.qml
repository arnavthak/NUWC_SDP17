// written by Ben

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: root
    anchors.fill: parent

    // ============================================================
    // DATA / FUNCTIONALITY
    // ============================================================
    property var results: ({})
    property var rawPinConfigs: ({})
    property var pinValues: ([])
    property var chipInfo: ({})
    property string selectedFormat: "json"

    property bool isSimulation

    property int totalCount: {
        if (!results)
            return 0
        return Object.keys(results).length
    }

    property int passCount: {
        var count = 0
        if (!results)
            return 0

        for (var key in results) {
            if (results[key] && results[key]["Result"] === "PASS")
                count++
        }
        return count
    }

    property int failCount: {
        var count = 0
        if (!results)
            return 0

        for (var key in results) {
            if (results[key] && results[key]["Result"] === "FAIL")
                count++
        }
        return count
    }

    property real successRate: {
        if (!results || totalCount === 0)
            return 0
        return passCount / totalCount
    }

    property real successPercentage: successRate * 100

    function buildExportData() {
        return {
            results: results,
            pinValues: pinValues,
            rawPinConfigs: rawPinConfigs
        }
    }

    function truthInputsForRow(rowIndex) {
        var map = pinValues[rowIndex]
        if (!map)
            return "-"

        var keys = Object.keys(map)
        keys.sort(function(a, b) { return parseInt(a) - parseInt(b) })

        var str = ""
        for (var i = 0; i < keys.length; i++) {
            var k = keys[i]
            str += k + ":" + map[k]
            if (i < keys.length - 1)
                str += ", "
        }
        return str
    }

    function summaryTitle() {
        if (totalCount === 0)
            return "No test summary available"
        if (failCount === 0)
            return "All tests passed"
        if (passCount === 0)
            return "All tests failed"
        return "Test run completed"
    }

    function summaryBody() {
        if (totalCount === 0)
            return "Run a test to generate a readable summary here. This section can later show pass/fail highlights, warnings, and a short explanation of what happened."

        var body = totalCount + " test"
        body += totalCount === 1 ? " was" : "s were"
        body += " executed. "
        body += passCount + " passed and " + failCount + " failed. "
        body += "Success rate: " + successPercentage.toFixed(1) + "%."

        if (failCount > 0)
            body += " Review the Truth Table and Test Results tabs for mismatches and failure details."

        return body
    }

    function statusText() {
        if (totalCount === 0)
            return "No active result set loaded"
        return "Loaded " + totalCount + " result" + (totalCount === 1 ? "" : "s")
    }

    function statusBadgeText() {
        if (totalCount === 0)
            return "Idle"
        if (failCount > 0)
            return "Issues"
        return "Ready"
    }

    function statusDotColor() {
        if (totalCount === 0)
            return "#94a3b8"
        if (failCount > 0)
            return "#dc2626"
        return "#16a34a"
    }

    // ============================================================
    // CONSISTENT TABLE COLUMN WIDTHS
    // ============================================================
    readonly property real tableSpacing: 12

    readonly property real truthAvailableWidth: Math.max(0, scrollView.availableWidth - 28)
    readonly property real truthColumnWidth: Math.max(90, (truthAvailableWidth - (tableSpacing * 4)) / 5)

    readonly property real pinAvailableWidth: Math.max(0, scrollView.availableWidth - 28)
    readonly property real pinCol1Width: Math.max(100, pinAvailableWidth * 0.16)
    readonly property real pinCol2Width: Math.max(120, pinAvailableWidth * 0.22)
    readonly property real pinCol3Width: Math.max(100, pinAvailableWidth * 0.16)
    readonly property real pinCol4Width: Math.max(180, pinAvailableWidth - pinCol1Width - pinCol2Width - pinCol3Width - (tableSpacing * 3))

    readonly property real testAvailableWidth: Math.max(0, scrollView.availableWidth - 28)
    readonly property real testCol1Width: Math.max(150, testAvailableWidth * 0.24)
    readonly property real testCol2Width: Math.max(100, testAvailableWidth * 0.14)
    readonly property real testCol3Width: Math.max(110, testAvailableWidth * 0.14)
    readonly property real testCol4Width: Math.max(220, testAvailableWidth - testCol1Width - testCol2Width - testCol3Width - (tableSpacing * 3))

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

            // ============================================================
            // HEADER CARD
            // ============================================================
            Frame {
                Layout.fillWidth: true
                padding: 18

                background: Rectangle {
                    color: "#eef6ff"
                    radius: 14
                    border.color: "#c7defc"
                }

                contentItem: ColumnLayout {
                    spacing: 6

                    Label {
                        text: "Test Results & Reports"
                        font.pixelSize: 22
                        font.bold: true
                        color: "#0f172a"
                    }

                    Label {
                        text: "Review test outcomes, truth tables, pin mappings, and generate simple reports."
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                        color: "#475569"
                        font.pixelSize: 13
                    }
                }
            }

            // ============================================================
            // SUMMARY CARDS
            // ============================================================
            GridLayout {
                Layout.fillWidth: true
                columns: width < 900 ? 2 : 4
                columnSpacing: 16
                rowSpacing: 16

                Repeater {
                    model: [
                        { "title": "Total Tests",  "value": totalCount.toString(), "accent": "#dbeafe", "textColor": "#2563eb", "iconText": "T" },
                        { "title": "Passed",       "value": passCount.toString(), "accent": "#dcfce7", "textColor": "#16a34a", "iconText": "P" },
                        { "title": "Failed",       "value": failCount.toString(), "accent": "#fee2e2", "textColor": "#dc2626", "iconText": "F" },
                        { "title": "Success Rate", "value": (totalCount === 0 ? "0%" : successPercentage.toFixed(1) + "%"), "accent": "#f3e8ff", "textColor": "#9333ea", "iconText": "%" }
                    ]

                    delegate: Frame {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 112
                        padding: 0

                        background: Rectangle {
                            color: "#ffffff"
                            radius: 14
                            border.color: "#e2e8f0"
                        }

                        contentItem: Item {
                            anchors.fill: parent

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 16
                                spacing: 14

                                Rectangle {
                                    Layout.preferredWidth: 46
                                    Layout.preferredHeight: 46
                                    radius: 12
                                    color: modelData["accent"]

                                    Label {
                                        anchors.centerIn: parent
                                        text: modelData["iconText"]
                                        font.pixelSize: 18
                                        font.bold: true
                                        color: modelData["textColor"]
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 6

                                    Label {
                                        text: modelData["title"]
                                        color: "#64748b"
                                        font.pixelSize: 12
                                    }

                                    Label {
                                        text: modelData["value"]
                                        color: "#0f172a"
                                        font.pixelSize: 24
                                        font.bold: true
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // ============================================================
            // MAIN RESULTS CARD
            // ============================================================
            Frame {
                Layout.fillWidth: true
                padding: 18

                background: Rectangle {
                    color: "#ffffff"
                    radius: 16
                    border.color: "#e2e8f0"
                }

                contentItem: ColumnLayout {
                    spacing: 18

                    // ----------------------------------------------------
                    // TITLE + ACTION BUTTONS
                    // ----------------------------------------------------
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 16

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Label {
                                text: "Detailed Results"
                                font.pixelSize: 19
                                font.bold: true
                                color: "#0f172a"
                            }

                            Label {
                                text: "View truth tables, pin information, individual test results, and export a simple report."
                                color: "#64748b"
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                                font.pixelSize: 13
                            }
                        }

                        RowLayout {
                            spacing: 10

                            FileDialog {
                                id: saveDialog
                                title: "Save File"
                                fileMode: FileDialog.SaveFile

                                onAccepted: {
                                    if (typeof exporter !== "undefined" && exporter) {
                                        exporter.exportData(buildExportData(), selectedFile, selectedFormat)
                                    } else {
                                        pdfStatusText.text = "Exporter is not available in QML."
                                        pdfStatusBar.visible = true
                                    }
                                }
                            }

                            Button {
                                text: "Export CSV"
                                enabled: totalCount > 0

                                onClicked: {
                                    selectedFormat = "csv"
                                    saveDialog.nameFilters = ["CSV Files (*.csv)"]
                                    saveDialog.open()
                                }

                                background: Rectangle {
                                    radius: 10
                                    color: parent.down ? "#e2e8f0" : "#f8fafc"
                                    border.color: "#cbd5e1"
                                }

                                contentItem: Label {
                                    text: parent.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: parent.enabled ? "#334155" : "#94a3b8"
                                    font.pixelSize: 13
                                }
                            }

                            Button {
                                text: "Export JSON"
                                enabled: totalCount > 0

                                onClicked: {
                                    selectedFormat = "json"
                                    saveDialog.nameFilters = ["JSON Files (*.json)"]
                                    saveDialog.open()
                                }

                                background: Rectangle {
                                    radius: 10
                                    color: parent.down ? "#e2e8f0" : "#f8fafc"
                                    border.color: "#cbd5e1"
                                }

                                contentItem: Label {
                                    text: parent.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: parent.enabled ? "#334155" : "#94a3b8"
                                    font.pixelSize: 13
                                }
                            }

                            Button {
                                text: "Export PDF Report"
                                onClicked: savePdfDialog.open()

                                background: Rectangle {
                                    radius: 10
                                    color: parent.down ? "#1d4ed8" : "#2563eb"
                                    border.color: "#2563eb"
                                }

                                contentItem: Label {
                                    text: parent.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: "#ffffff"
                                    font.pixelSize: 13
                                    font.bold: true
                                }
                            }
                        }
                    }

                    // ----------------------------------------------------
                    // STATUS STRIP
                    // ----------------------------------------------------
                    Rectangle {
                        Layout.fillWidth: true
                        radius: 12
                        color: "#f8fafc"
                        border.color: "#e2e8f0"
                        implicitHeight: 54

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 14
                            spacing: 12

                            Rectangle {
                                width: 10
                                height: 10
                                radius: 5
                                color: root.statusDotColor()
                            }

                            Label {
                                text: root.statusText()
                                color: "#334155"
                                font.pixelSize: 13
                                font.bold: true
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Rectangle {
                                radius: 999
                                color: "#e2e8f0"
                                implicitWidth: 82
                                implicitHeight: 28

                                Label {
                                    anchors.centerIn: parent
                                    text: root.statusBadgeText()
                                    color: "#475569"
                                    font.pixelSize: 12
                                    font.bold: true
                                }
                            }
                        }
                    }

                    // ----------------------------------------------------
                    // TAB BAR
                    // ----------------------------------------------------
                    Rectangle {
                        Layout.fillWidth: true
                        radius: 12
                        color: "#f8fafc"
                        border.color: "#e2e8f0"
                        implicitHeight: 52

                        TabBar {
                            id: resultsTabBar
                            anchors.fill: parent
                            anchors.margins: 6
                            spacing: 6
                            background: Item {}

                            TabButton {
                                text: "Truth Table"
                                implicitHeight: 38

                                background: Rectangle {
                                    radius: 10
                                    color: parent.checked ? "#ffffff" : "transparent"
                                    border.color: parent.checked ? "#cbd5e1" : "transparent"
                                }

                                contentItem: Label {
                                    text: parent.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: parent.checked ? "#0f172a" : "#64748b"
                                    font.pixelSize: 13
                                    font.bold: parent.checked
                                }
                            }

                            TabButton {
                                text: "Pin Mapping"
                                implicitHeight: 38

                                background: Rectangle {
                                    radius: 10
                                    color: parent.checked ? "#ffffff" : "transparent"
                                    border.color: parent.checked ? "#cbd5e1" : "transparent"
                                }

                                contentItem: Label {
                                    text: parent.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: parent.checked ? "#0f172a" : "#64748b"
                                    font.pixelSize: 13
                                    font.bold: parent.checked
                                }
                            }

                            TabButton {
                                text: "Test Results"
                                implicitHeight: 38

                                background: Rectangle {
                                    radius: 10
                                    color: parent.checked ? "#ffffff" : "transparent"
                                    border.color: parent.checked ? "#cbd5e1" : "transparent"
                                }

                                contentItem: Label {
                                    text: parent.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: parent.checked ? "#0f172a" : "#64748b"
                                    font.pixelSize: 13
                                    font.bold: parent.checked
                                }
                            }
                        }
                    }

                    // ----------------------------------------------------
                    // TAB CONTENT
                    // ----------------------------------------------------
                    StackLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 320
                        currentIndex: resultsTabBar.currentIndex

                        // =================================================
                        // TRUTH TABLE TAB
                        // =================================================
                        Item {
                            Rectangle {
                                anchors.fill: parent
                                radius: 14
                                color: "#ffffff"
                                border.color: "#e2e8f0"
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 14
                                spacing: 12

                                Rectangle {
                                    Layout.fillWidth: true
                                    implicitHeight: 48
                                    radius: 10
                                    color: "#f8fafc"
                                    border.color: "#e2e8f0"

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.margins: 12
                                        spacing: root.tableSpacing

                                        Item {
                                            Layout.preferredWidth: root.truthColumnWidth
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Inputs"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.truthColumnWidth
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Output"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.truthColumnWidth
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Expected"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.truthColumnWidth
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Status"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        /*Item {
                                            Layout.preferredWidth: root.truthColumnWidth
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Test Name"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }*/
                                    }
                                }

                                Loader {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    active: totalCount > 0 && pinValues.length > 0

                                    sourceComponent: ListView {
                                        anchors.fill: parent
                                        clip: true
                                        spacing: 6
                                        model: Object.keys(root.results)

                                        delegate: Rectangle {
                                            width: ListView.view ? ListView.view.width : parent.width
                                            height: 52
                                            radius: 10
                                            color: index % 2 === 0 ? "#ffffff" : "#f8fafc"
                                            border.color: "#e2e8f0"

                                            RowLayout {
                                                anchors.fill: parent
                                                anchors.margins: 12
                                                spacing: root.tableSpacing

                                                Item {
                                                    Layout.preferredWidth: root.truthColumnWidth
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.truthInputsForRow(index)
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        wrapMode: Text.WordWrap
                                                        elide: Text.ElideRight
                                                        color: "#0f172a"
                                                        font.pixelSize: 12
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.truthColumnWidth
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.results[modelData] && root.results[modelData]["Response"] ? root.results[modelData]["Response"] : "-"
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        color: "#0f172a"
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.truthColumnWidth
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.results[modelData] && root.results[modelData]["Expected"] ? root.results[modelData]["Expected"] : "-"
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        color: "#0f172a"
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.truthColumnWidth
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.results[modelData] && root.results[modelData]["Result"] ? root.results[modelData]["Result"] : "-"
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        color: root.results[modelData] && root.results[modelData]["Result"] === "PASS" ? "#16a34a" : "#dc2626"
                                                        font.bold: true
                                                    }
                                                }

                                                /*Item {
                                                    Layout.preferredWidth: root.truthColumnWidth
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: modelData
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        elide: Text.ElideRight
                                                        color: "#334155"
                                                        font.pixelSize: 12
                                                    }
                                                }*/
                                            }
                                        }
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    visible: !(totalCount > 0 && pinValues.length > 0)

                                    Column {
                                        anchors.centerIn: parent
                                        spacing: 10
                                        width: Math.min(parent.width - 40, 420)

                                        Rectangle {
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            width: 56
                                            height: 56
                                            radius: 16
                                            color: "#e0f2fe"

                                            Label {
                                                anchors.centerIn: parent
                                                text: "TT"
                                                color: "#0284c7"
                                                font.bold: true
                                                font.pixelSize: 16
                                            }
                                        }

                                        Label {
                                            width: parent.width
                                            text: "No truth table data available"
                                            horizontalAlignment: Text.AlignHCenter
                                            font.pixelSize: 16
                                            font.bold: true
                                            color: "#0f172a"
                                        }

                                        Label {
                                            width: parent.width
                                            text: "Run a test to populate this table with inputs, outputs, expected values, and pass/fail status."
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignHCenter
                                            color: "#64748b"
                                            font.pixelSize: 13
                                        }
                                    }
                                }
                            }
                        }

                        // =================================================
                        // PIN MAPPING TAB
                        // =================================================
                        Item {
                            Rectangle {
                                anchors.fill: parent
                                radius: 14
                                color: "#ffffff"
                                border.color: "#e2e8f0"
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 14
                                spacing: 12

                                Rectangle {
                                    Layout.fillWidth: true
                                    implicitHeight: 48
                                    radius: 10
                                    color: "#f8fafc"
                                    border.color: "#e2e8f0"

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.margins: 12
                                        spacing: root.tableSpacing

                                        Item {
                                            Layout.preferredWidth: root.pinCol1Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Pin Number"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.pinCol2Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Function"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.pinCol3Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Alias"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.pinCol4Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Description"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }
                                    }
                                }

                                Loader {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    active: Object.keys(root.rawPinConfigs).length > 0

                                    sourceComponent: ListView {
                                        anchors.fill: parent
                                        clip: true
                                        spacing: 6
                                        model: Object.keys(root.rawPinConfigs).sort(function(a, b) { return parseInt(a) - parseInt(b) })

                                        delegate: Rectangle {
                                            width: ListView.view ? ListView.view.width : parent.width
                                            height: 52
                                            radius: 10
                                            color: index % 2 === 0 ? "#ffffff" : "#f8fafc"
                                            border.color: "#e2e8f0"

                                            RowLayout {
                                                anchors.fill: parent
                                                anchors.margins: 12
                                                spacing: root.tableSpacing

                                                Item {
                                                    Layout.preferredWidth: root.pinCol1Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: modelData
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        color: "#0f172a"
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.pinCol2Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.rawPinConfigs[modelData] && root.rawPinConfigs[modelData][0] ? root.rawPinConfigs[modelData][0] : "-"
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        elide: Text.ElideRight
                                                        color: "#0f172a"
                                                        font.pixelSize: 12
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.pinCol3Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.rawPinConfigs[modelData] && root.rawPinConfigs[modelData][1] ? root.rawPinConfigs[modelData][1] : "-"
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        elide: Text.ElideRight
                                                        color: "#0f172a"
                                                        font.pixelSize: 12
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.pinCol4Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.rawPinConfigs[modelData] && root.rawPinConfigs[modelData][2] ? root.rawPinConfigs[modelData][2] : "-"
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        wrapMode: Text.WordWrap
                                                        elide: Text.ElideRight
                                                        color: "#0f172a"
                                                        font.pixelSize: 12
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    visible: Object.keys(root.rawPinConfigs).length === 0

                                    Column {
                                        anchors.centerIn: parent
                                        spacing: 10
                                        width: Math.min(parent.width - 40, 430)

                                        Rectangle {
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            width: 56
                                            height: 56
                                            radius: 16
                                            color: "#ede9fe"

                                            Label {
                                                anchors.centerIn: parent
                                                text: "PM"
                                                color: "#7c3aed"
                                                font.bold: true
                                                font.pixelSize: 16
                                            }
                                        }

                                        Label {
                                            width: parent.width
                                            text: "No pin mapping data available"
                                            horizontalAlignment: Text.AlignHCenter
                                            font.pixelSize: 16
                                            font.bold: true
                                            color: "#0f172a"
                                        }

                                        Label {
                                            width: parent.width
                                            text: "Load a circuit configuration to display pin names, functions, aliases, and descriptions."
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignHCenter
                                            color: "#64748b"
                                            font.pixelSize: 13
                                        }
                                    }
                                }
                            }
                        }

                        // =================================================
                        // TEST RESULTS TAB
                        // =================================================
                        Item {
                            Rectangle {
                                anchors.fill: parent
                                radius: 14
                                color: "#ffffff"
                                border.color: "#e2e8f0"
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 14
                                spacing: 12

                                Rectangle {
                                    Layout.fillWidth: true
                                    implicitHeight: 48
                                    radius: 10
                                    color: "#f8fafc"
                                    border.color: "#e2e8f0"

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.margins: 12
                                        spacing: root.tableSpacing

                                        Item {
                                            Layout.preferredWidth: root.testCol1Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Test Name"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.testCol2Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Status"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.testCol3Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Duration"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }

                                        Item {
                                            Layout.preferredWidth: root.testCol4Width
                                            Layout.fillHeight: true

                                            Label {
                                                anchors.fill: parent
                                                text: "Message"
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                font.bold: true
                                                color: "#334155"
                                            }
                                        }
                                    }
                                }

                                Loader {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    active: totalCount > 0

                                    sourceComponent: ListView {
                                        anchors.fill: parent
                                        clip: true
                                        spacing: 6
                                        model: Object.keys(root.results)

                                        delegate: Rectangle {
                                            width: ListView.view ? ListView.view.width : parent.width
                                            height: 52
                                            radius: 10
                                            color: index % 2 === 0 ? "#ffffff" : "#f8fafc"
                                            border.color: "#e2e8f0"

                                            RowLayout {
                                                anchors.fill: parent
                                                anchors.margins: 12
                                                spacing: root.tableSpacing

                                                Item {
                                                    Layout.preferredWidth: root.testCol1Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: modelData
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        elide: Text.ElideRight
                                                        color: "#0f172a"
                                                        font.pixelSize: 12
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.testCol2Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: root.results[modelData] && root.results[modelData]["Result"] ? root.results[modelData]["Result"] : "-"
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        color: root.results[modelData] && root.results[modelData]["Result"] === "PASS" ? "#16a34a" : "#dc2626"
                                                        font.bold: true
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.testCol3Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: {
                                                            if (!root.results[modelData])
                                                                return "-"
                                                            var duration = root.results[modelData]["Duration_ms"]
                                                            return duration !== undefined ? duration + " ms" : "-"
                                                        }
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        color: "#0f172a"
                                                    }
                                                }

                                                Item {
                                                    Layout.preferredWidth: root.testCol4Width
                                                    Layout.fillHeight: true

                                                    Label {
                                                        anchors.fill: parent
                                                        text: {
                                                            if (!root.results[modelData])
                                                                return "-"
                                                            if (root.results[modelData]["Description"])
                                                                return root.results[modelData]["Description"]
                                                            return "-"
                                                        }
                                                        horizontalAlignment: Text.AlignHCenter
                                                        verticalAlignment: Text.AlignVCenter
                                                        wrapMode: Text.WordWrap
                                                        elide: Text.ElideRight
                                                        color: "#0f172a"
                                                        font.pixelSize: 12
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                Item {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    visible: totalCount === 0

                                    Column {
                                        anchors.centerIn: parent
                                        spacing: 10
                                        width: Math.min(parent.width - 40, 430)

                                        Rectangle {
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            width: 56
                                            height: 56
                                            radius: 16
                                            color: "#dcfce7"

                                            Label {
                                                anchors.centerIn: parent
                                                text: "TR"
                                                color: "#16a34a"
                                                font.bold: true
                                                font.pixelSize: 16
                                            }
                                        }

                                        Label {
                                            width: parent.width
                                            text: "No test results available"
                                            horizontalAlignment: Text.AlignHCenter
                                            font.pixelSize: 16
                                            font.bold: true
                                            color: "#0f172a"
                                        }

                                        Label {
                                            width: parent.width
                                            text: "Execute a test run to view named results, duration information, messages, and status indicators."
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignHCenter
                                            color: "#64748b"
                                            font.pixelSize: 13
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

    // ============================================================
    // PDF SAVE DIALOG
    // ============================================================
    FileDialog {
        id: savePdfDialog
        title: "Save PDF Report"
        fileMode: FileDialog.SaveFile
        nameFilters: ["PDF files (*.pdf)"]
        defaultSuffix: "pdf"

        onAccepted: {
            var selectedPath = selectedFile.toString()

            if (selectedPath.startsWith("file://")) {
                selectedPath = selectedPath.replace("file://", "")

                if (!selectedPath.startsWith("/")) {
                    selectedPath = "/" + selectedPath
                }

                selectedPath = decodeURIComponent(selectedPath)
            }

            console.log("Saving PDF to:", selectedPath)

            if (typeof pdfGenerator !== "undefined" && pdfGenerator) {

                // =====================================================
                // BUILD PIN CONFIG STRUCTURE
                // =====================================================
                var pinConfigsList = []

                var pinKeys = Object.keys(root.rawPinConfigs)
                pinKeys.sort(function(a, b) { return parseInt(a) - parseInt(b) })

                for (var i = 0; i < pinKeys.length; i++) {
                    var pin = pinKeys[i]
                    var cfg = root.rawPinConfigs[pin]

                    pinConfigsList.push({
                        pinName: pin,
                        direction: cfg && cfg[0] ? cfg[0] : "-",
                        defaultValue: cfg && cfg[1] ? cfg[1] : "-"
                    })
                }

                // =====================================================
                // BUILD TEST RESULT STRUCTURE
                // =====================================================
                var testsList = []

                var testKeys = Object.keys(root.results)

                for (var i = 0; i < testKeys.length; i++) {
                    var key = testKeys[i]
                    var result = root.results[key]

                    // Build input pin map
                    var inputMap = {}
                    if (root.pinValues[i]) {
                        var pinMap = root.pinValues[i]
                        var pins = Object.keys(pinMap)

                        for (var j = 0; j < pins.length; j++) {
                            inputMap[pins[j]] = String(pinMap[pins[j]])
                        }
                    }

                    testsList.push({
                        testName: key,
                        inputPins: inputMap,
                        responseBytes: result && result["Response"] ? result["Response"] : "-",
                        expectedBytes: result && result["Expected"] ? result["Expected"] : "-",
                        passed: result && result["Result"] === "PASS",
                        durationMs: result && result["Duration_ms"] !== undefined ? result["Duration_ms"] : 0
                    })
                }

                // =====================================================
                // CALL UPDATED C++ FUNCTION
                // =====================================================
                var ok = pdfGenerator.generateBasicReport(
                    selectedPath,
                    "74ls" + chipInfo["Chip Number"],
                    isSimulation ? "Simulation" : "Hardware",
                    totalCount,
                    passCount,
                    failCount,
                    pinConfigsList,
                    testsList,
                    [summaryBody()]
                )

                if (ok) {
                    pdfStatusText.text = "PDF report generated successfully."
                    pdfStatusBar.visible = true
                } else {
                    pdfStatusText.text = "Failed to generate PDF report."
                    pdfStatusBar.visible = true
                }

            } else {
                pdfStatusText.text = "pdfGenerator is not available in QML."
                pdfStatusBar.visible = true
            }
        }
    }

    // ============================================================
    // FLOATING STATUS BAR
    // ============================================================
    Rectangle {
        id: pdfStatusBar
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        radius: 12
        color: "#0f172a"
        border.color: "#1e293b"
        implicitWidth: Math.max(260, pdfStatusText.implicitWidth + 40)
        implicitHeight: 48
        z: 100

        Label {
            id: pdfStatusText
            anchors.centerIn: parent
            color: "#ffffff"
            font.pixelSize: 13
            text: ""
        }

        MouseArea {
            anchors.fill: parent
            onClicked: pdfStatusBar.visible = false
        }

        Timer {
            interval: 3500
            running: pdfStatusBar.visible
            repeat: false
            onTriggered: pdfStatusBar.visible = false
        }
    }

    Connections {
        target: typeof pdfGenerator !== "undefined" ? pdfGenerator : null

        function onReportGenerated(filePath) {
            pdfStatusText.text = "Saved PDF: " + filePath
            pdfStatusBar.visible = true
        }

        function onReportFailed(reason) {
            pdfStatusText.text = "Error: " + reason
            pdfStatusBar.visible = true
        }
    }
}