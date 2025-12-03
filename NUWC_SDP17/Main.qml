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
            id: fileSelector
            objectName: "fileSelector"
            text: qsTr("Choose YAML File...")
            onClicked: {
                //serialComms.linkTest("COM4", "COM7"); //enter your specific OS specific path names here
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
                yamlProcessor.readChipConfiguration(fileDialog.selectedFile);
            }
        }
    }
}
