/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

Rectangle {
    id: wi_dev

    property bool binded: false
    property int signal_strength: 75
    property alias text: button_Text.text

    // Both left and right buttons are acceptable
    signal bindingClicked(int button)
    signal clicked(int button)

    border.width: 3
    border.color: "#38a900"

    width: 383
    height: 67
    radius: 5
    color: "#ffff99"

    antialiasing: true

    MouseArea {
        id: mainMouseArea
        smooth: true
        antialiasing: false
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor


        // Events
        onClicked: parent.clicked(mouse.button)
        onPressedChanged: {}
    }

    Column {
        id: status_Column
        x: 47
        y: 6
        width: childrenRect.width
        transformOrigin: Item.Center
        anchors.left: parent.left
        anchors.leftMargin: 7
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 7
        anchors.top: parent.top
        anchors.topMargin: 7
        spacing:2

        Rectangle {
            id: row1
            height: parent.height/2
            width: childrenRect.width + 10
            color: "transparent"
            radius:3
            border.width: 1
            border.color: "transparent"
            opacity:0.7

            Image {
                id: rssi_Image
                //opacity: 1
                //anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                anchors.left: parent.left
                anchors.leftMargin: 2
                sourceSize.height: 15
                sourceSize.width: 0
                fillMode: Image.PreserveAspectFit
                source: {
                    //"pics/rssi_100.svg"
                    if (signal_strength > 87)
                        "pics/rssi_100.svg"
                    else if (signal_strength > 62)
                        "pics/rssi_75.svg"
                    else if (signal_strength > 37)
                        "pics/rssi_50.svg"
                    else if (signal_strength > 12)
                        "pics/rssi_25.svg"
                    else
                        "pics/rssi_0.svg"
                }
            }

            Text {
                id: rssi_Text
                text: {
                    if (wi_dev.signal_strength > 0)
                        wi_dev.signal_strength + "%"
                    else
                        "Err"
                }
                anchors.left: rssi_Image.right
                anchors.leftMargin: 2
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                font.pixelSize: 12
                //anchors.verticalCenter: parent.verticalCenter
                style: Text.Normal
                color: "#808080"
                styleColor: "#808080"
            }
        }

        Rectangle {
            id: row2
            height: parent.height/2
            width: childrenRect.width + 4
            color: "transparent"
            radius:3
            border.width: 1
            border.color: "transparent"
            opacity:0.7


            Image {
                id: bind_Image
                opacity: 1

                fillMode: Image.PreserveAspectFit
                source: {
                    if (wi_dev.binded)
                        "pics/binding_ok.svg"
                    else
                        "pics/binding_no.svg"
                }
                //anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                anchors.left: parent.left
                anchors.leftMargin: 2
                sourceSize.height: 0
                sourceSize.width: 15
            }

            Text {
                id: bind_Text
                color: "#808080"
                text: {
                    if (wi_dev.binded)
                        "OK"
                    else
                        "NO"
                }
                style: Text.Normal
                //anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                anchors.left: bind_Image.right
                anchors.leftMargin: 2
                styleColor: "#808080"
                font.pixelSize: 12
            }
            MouseArea {
                id: mouseArea2
                smooth: true
                antialiasing: false
                anchors.left: bind_Image.left
                anchors.right: bind_Text.right
                anchors.top: bind_Text.top
                anchors.bottom: bind_Text.bottom
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor


                // Events
                onClicked: {
                    wi_dev.bindingClicked(mouse.button)
                }
                onPressedChanged: {}
            }
        }
    }

    Text {
        id: button_Text
        y: 6
        height: 18
        anchors.verticalCenter: parent.verticalCenter
        style: Text.Normal
        color: "#000000"
        text: "#parent.description#"
        font.pixelSize: 12
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        verticalAlignment: Text.AlignVCenter
        anchors.right: parent.right
        anchors.rightMargin: 7
        anchors.left: status_Column.right
        anchors.leftMargin: 0
        horizontalAlignment: Text.AlignRight
        styleColor: "#808080"
    }

    states:  [
        State {
            // Mouse is over the "bind/unbind" subbutton
            name: "bind-over"; when: mouseArea2.containsMouse
            PropertyChanges { target: bind_Text; text: {if (wi_dev.binded) qsTr("unbind"); else qsTr("bind");}}
            PropertyChanges { target: row2; border.color: "#808080"; opacity:1}
        },

        State {
            name: "mouse-over"; when: mainMouseArea.containsMouse && mainMouseArea.pressedButtons == Qt.NoButton
            PropertyChanges { target: wi_dev; border.color: "#80d000"}
        },

        State {
            name: "mouse-down-left"; when: mainMouseArea.containsMouse && mainMouseArea.pressedButtons != Qt.NoButton;
            PropertyChanges { target: button_Text; color: "green"}
            PropertyChanges { target: wi_dev; border.color: "#FF9933"}
            PropertyChanges { target: wi_dev; color: "#FFFF66"}

            //PropertyChanges { target: wi_dev; opacity: 0.5; }
        }
    ]

    transitions: Transition {
        //NumberAnimation { properties: "opacity"; easing.type: Easing.InOutQuad; duration: 50}
        //ColorAnimation {properties: "color"; easing.type: Easing.InOutQuad; duration: 50}
    }

    Keys.onSpacePressed: parent.clicked()
}
