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
        onAccepted: {
            var filePath = selectedFile;

            var chipNum = yamlProcessor.readChipInfo(filePath, "Chip Number");
            chipNumberLabel.text = "Chip Number: " + chipNum;

        }
    }

    Column {
        Text {
            id: chipNumberLabel
            text: "Chip Number: N/A";
            font.pixelSize: 16
            color: "black";
        }

        Button {
            text: "Submit"
            onClicked: {
                yamlProcessor.yamlLoaded(fileDialog.selectedFile);
            }
        }
    }
}
