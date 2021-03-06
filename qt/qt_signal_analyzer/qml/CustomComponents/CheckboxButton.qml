import QtQuick 2.0
import QtQuick.Controls 2.4
import "../CustomStyle"

Item {
    id: control
    property string text: "Checkbox: "
    property bool isChecked: checkBox.checked
    property int numberID: 0

    property color buttonTextColor: UIStyle.buttonTextColor
    property color buttonBorderColor: UIStyle.buttonBorderColor

    signal checkChanged(bool checkStatus)
    signal nameChanged(string newName)

    implicitWidth: checkBox.implicitWidth
    implicitHeight: checkBox.implicitHeight

    CheckBox {
        implicitWidth: 120
        implicitHeight: 60
        id: checkBox
        text: control.text
        checked: false

        indicator: Rectangle {
            implicitWidth: 20
            implicitHeight: 20
            x: checkBox.leftPadding
            y: parent.height / 2 - height / 2
            radius: 4
            color: UIStyle.buttonBgHovered
            border.color: UIStyle.buttonBgUnhovered

            Rectangle {
                anchors.fill: parent
                radius: 4
                color: checkBox.checked ? control.buttonTextColor : UIStyle.buttonBgHovered
                visible: checkBox.checked
            }
        }

        background: Rectangle {
                radius: 4
                border.color:  control.buttonBorderColor
                border.width: checkBox.visualFocus ? 2 : 1
                color: checkBox.hovered ? UIStyle.buttonBgHovered : UIStyle.buttonBgUnhovered
        }

        contentItem: TextField {
            text: control.text
            opacity: enabled ? 1.0 : 0.3
            padding: 0

            anchors.top: checkBox.top
            anchors.left: checkBox.indicator.right
            anchors.right: checkBox.right
            anchors.bottom: checkBox.bottom
            cursorVisible: true
            selectByMouse: true
            verticalAlignment: Text.AlignVCenter

            topPadding: checkBox.height / 5
            leftPadding: checkBox.spacing
            rightPadding: checkBox.spacing
            font.weight: Font.DemiBold
            color: control.buttonTextColor

            background: Rectangle {
                x: checkBox.indicator.width + 5
                color: checkBox.hovered ? UIStyle.buttonBgHovered : UIStyle.buttonBgUnhovered
                border.color: "transparent"
            }

            onAccepted: {
                nameChanged(text)
            }
        }

        onClicked: {
            checkChanged(checked)
        }
    }
}
