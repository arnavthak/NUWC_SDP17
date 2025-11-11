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
            onClicked: fileDialog.open()
        }
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["YAML Files (*.yaml *.yml)", "All Files (*)"]
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        onAccepted: {
            var filePath = selectedFile;
            var field = "Chip Number";
            var chipNum = yamlProcessor.readChipInfo(filePath, field);
            chipNumberLabel.text = "Chip Number: " + chipNum;
        }
    }

    Text {
        id: chipNumberLabel
        text: "Chip Number: N/A";
        font.pixelSize: 16
        color: "white";
    }
}
