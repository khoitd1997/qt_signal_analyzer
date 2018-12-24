import QtQuick 2.1
import QtQuick.Controls 2.5
import QtQuick 2.12

import "../../CustomComponents"
import "../../CustomStyle"

Item {
    CollapsibleSection {
            id: mathHeader
            width: parent.width
            anchors.topMargin: 10
            displayText: "Math"
            onIsClicked: {
                isOn ? tabUp2.running = true : tabDown2.running = true
            }
    }

    Rectangle {
        PropertyAnimation { id: tabDown2;
                            easing.type: Easing.Linear	;
                            target: mathSection;
                            property: "height";
                            from: 0;
                            to: mathSection.childrenRect.height + 10;
                            duration: 150 
        }

        PropertyAnimation { id: tabUp2;
                            easing.type: Easing.Linear;
                            target: mathSection;
                            property: "height";
                            from: mathSection.childrenRect.height + 10;
                            to: 0;
                            duration: 150 
        }
        
        id: mathSection
        width: mathHeader.width
        visible: false
        anchors.top: mathHeader.bottom
        color: "#3A3A3A"
    }
}
