/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 2.4
import QtQml 2.12
import QtQuick.Controls.Material 2.4


Item {
    id: button

    property string text: "Option: "
    property variant items: ["first"]
    property int currentSelection: 0
    signal selectionChanged(variant selection)
    signal clicked

    implicitWidth: comboBox.implicitWidth
    implicitHeight: comboBox.implicitHeight

    ComboBox {
        id: comboBox
        font.capitalization: Font.Capitalize
        font.weight: Font.DemiBold
        font.pointSize: 13
        hoverEnabled : true
        model: button.items
        transformOrigin: Popup.Bottom
        indicator.rotation: 90

        delegate: ItemDelegate {
                width: comboBox.width
                contentItem: Text {
                    text: modelData
                    color: "#ff8f00"
                    font: comboBox.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: comboBox.highlightedIndex === index
            }

        contentItem: Label {
            id: comboText
            leftPadding: 10
            rightPadding: comboBox.spacing
            text: button.text + comboBox.displayText
            font: comboBox.font
            color: "#ff8f00"
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

                indicator.x: comboText.implicitWidth

        background: Rectangle {
            radius: 4
            implicitWidth: 40
            implicitHeight: 40
            border.color:  "#c56000"
            border.width: comboBox.visualFocus ? 2 : 1
            color: comboBox.hovered ? "#616161" : "#424242"
        }

//        indicator: Canvas {
//            id: canvas

//            width: 12
//            height: 8
//            contextType: "2d"

//            onPaint: {
//                context.reset();
//                context.moveTo(0, 0);
//                context.lineTo(width, 0);
//                context.lineTo(width / 2, height);
//                context.closePath();
//                context.fillStyle = comboBox.pressed ? "#17a81a" : "#21be2b";
//                context.fill();
//            }
//        }

        popup: Popup {
            id: popUp
            y: comboBox.height - 1
            width: comboBox.width
            implicitHeight: contentItem.implicitHeight
            padding: 1

            enter: Transition {
                RotationAnimator { target: comboBox.indicator
                    from: 90;
                            to: 0;
                duration: 150}
            }

            exit: Transition {
                RotationAnimator { target: comboBox.indicator
                                    from: 0;
                                            to: 90;
                duration: 150}
            }

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: comboBox.popup.visible ? comboBox.delegateModel : null
                currentIndex: comboBox.highlightedIndex
                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                color: popUp.hovered ? "#616161" : "#424242"
                radius: 4
            }
        }

        onActivated: {
            currentSelection = index;
            selectionChanged(button.items[index]);
        }
    }
}
