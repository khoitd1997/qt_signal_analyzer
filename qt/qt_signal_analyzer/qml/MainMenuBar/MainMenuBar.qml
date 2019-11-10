import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick 2.12
import QtQuick.Dialogs 1.3

import Qt.analyzer.signalSourceDetector 1.0
import Qt.analyzer.dataSource 1.0

MenuBar {
    signal graphImageRequested(string graphImageUrl)
    signal signalSourceChanged(string newSignalSource)
    property string currentSignalSource : "Simulation"
    id: menuBar

    height: 25
    FileDialog {
        visible: false
        id: graphSaveDialog
        title: "Please choose a file"
        selectExisting: false
        folder: shortcuts.home
        nameFilters: [ "Image files (*.jpg *.png)"]
        onAccepted: {
            graphImageRequested(graphSaveDialog.fileUrls.toString())
        }
    }

    MainMenuObject {
        title: qsTr("File")

        Action {
            text: qsTr("Save Graph Picture")
            onTriggered: {
                graphSaveDialog.visible = true
            }
        }

        MenuSeparator {
            leftPadding: 10
            rightPadding: 10
        }
    }

    MainMenuObject {
        title: qsTr("Edit")

        MainMenuObject {
            id: signalSourceMenu
            title: qsTr("Signal Sources")

            Repeater {
                model: SignalSourceDetector.sources
                MenuItem {
                    text: modelData
                    onTriggered: {
                        if(currentSignalSource !== modelData) {
                            currentSignalSource = modelData
                            signalSourceChanged(currentSignalSource)
                        }
                    }
                }
            }

            onAboutToShow: {
                SignalSourceDetector.updateSources()
            }
        }

        MenuSeparator {
            leftPadding: 10
            rightPadding: 10
        }
    }


    Menu { title: qsTr("Help") }

    delegate: MenuBarItem {
        id: menuBarItem
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        contentItem: Text {
            text: menuBarItem.text
            font.pointSize: 12
            opacity: enabled ? 1.0 : 0.3
            color: "#BEBCB5"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            opacity: enabled ? 1 : 0.3
            color: menuBarItem.highlighted ? "#666561" : "transparent"
        }
    }

    Rectangle {
        implicitWidth: 40
        implicitHeight: menuBar.height
        color: "#ffffff"
        Rectangle {
            color: "#21be2b"
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }

    Component.onCompleted: {
        DataSource.setGuiSource(menuBar)
    }
}
