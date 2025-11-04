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

    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        onAccepted: image.source = selectedFile
    }
}
