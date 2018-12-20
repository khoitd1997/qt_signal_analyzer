import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick 2.12

Drawer {
    id: sideBar

    modal: false
    interactive: false
    position: 0
    visible: true

    signal tabChanged(int newTabIndex)

    Rectangle {
        color:"#181818"
        anchors.fill: parent

        Column {
            width: parent.width
            height: parent.height
            spacing: 22
            topPadding: 10
            leftPadding: 6
            Repeater {
                id: iconList
                width: parent.width
                height: parent.height

                // color code #D1D1D1
                model: ["qrc:live-icon.png", "qrc:analytics.png", "qrc:microchip.png"]
                focus: true

                delegate: Component {
                    Image {
                        id: sideBarIcon
                        width: 40
                        height: 40
                        source: modelData
                        opacity: index == 0 ? 0.8 : 0.3

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                tabChanged(index)
                                changeCurrTab(index)
                            }
                        }
                    }
                }
            }
        }
    }
    
    function changeCurrTab(iconIndex) {
        for(var i = 0; i < iconList.count; i++){
            iconList.itemAt(i).opacity = 0.3
        }
        iconList.itemAt(iconIndex).opacity = 0.8
    }
}
