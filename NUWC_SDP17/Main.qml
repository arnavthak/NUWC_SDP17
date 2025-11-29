// Code Author: Arnav Thakrar

import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    width: 1280
    height: 960
    visible: true
    title: qsTr("IC Test Application")

    header: ToolBar {
        Button {
            text: qsTr("Choose YAML File...")
            onClicked: {
                //serialComms.linkTest("/dev/ttys006", "/dev/ttys008");
                fileDialog.open();
            }
        }
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["YAML Files (*.yaml *.yml)", "All Files (*)"]
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
    }

    Column {
        Button {
            text: "Submit"
            onClicked: {
                yamlProcessor.yamlLoaded(fileDialog.selectedFile);
            }
        }
    }
}
