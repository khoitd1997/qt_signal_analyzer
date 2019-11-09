import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick 2.12

Menu {
    delegate: MenuItem {
        id: menuObject

        contentItem: Text {
            leftPadding: 0
            rightPadding: menuObject.arrow.width
            text: menuObject.text
            font: menuObject.font
            opacity: enabled ? 1.0 : 0.3
            color: menuObject.highlighted ? "#ffffff" : "#BEBCB5"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }
}
