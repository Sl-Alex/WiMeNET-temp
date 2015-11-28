import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 500
    height: 197
    clip: true

    property alias connectBtn: connectBtn
    property alias disconnectBtn: disconnectBtn
    property alias onBtn: onBtn
    property alias offBtn: offBtn
    property alias statusBtn: statusBtn
    property alias wiBtn: wi_btn
    property alias packetTxt: packet_text

    property alias connstatusEdit: connstatusEdit
    property alias ledvalueEdit: ledvalueEdit

    GroupBox {
        id: groupBox1
        x: 8
        y: 8
        width: 150
        height: 182
        title: qsTr("Connection")

        ColumnLayout {
            id: columnLayout1
            anchors.fill: parent

            Button {
                id: connectBtn
                y: 8
                height: 23
                text: qsTr("Connect")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            WiNET_device_button {
                id: wi_btn
                y: 40
                height: 50
                text: qsTr("Kitchen climate sensor")
                binded: true
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            Button {
                id: disconnectBtn
                y: 58
                text: qsTr("Disconnect")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            RowLayout {
                id: rowLayout1
                y: 197
                height: 100
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0

                Label {
                    id: label1
                    height: 13
                    text: qsTr("Status")
                    verticalAlignment: Text.AlignVCenter
                }

                TextField {
                    id: connstatusEdit
                    text: "Disconnected"
                    readOnly: true
                    anchors.left: label1.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    anchors.leftMargin: 10
                    placeholderText: qsTr("")
                }
            }
        }
    }

    GroupBox {
        id: groupBox2
        y: 8
        width: 107
        height: 182
        anchors.left: groupBox1.right
        anchors.leftMargin: 0
        title: qsTr("LED control")

        ColumnLayout {
            id: columnLayout2
            anchors.fill: parent

            Button {
                id: onBtn
                y: 38
                text: qsTr("On")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            Button {
                id: offBtn
                y: 58
                text: qsTr("Off")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            Button {
                id: statusBtn
                y: 142
                text: qsTr("Get status")
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
            }

            RowLayout {
                id: rowLayout2
                height: 100
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0

                Label {
                    id: label2
                    text: qsTr("Value")
                }

                TextField {
                    id: ledvalueEdit
                    text: "0"
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.top: parent.top
                    anchors.leftMargin: 10
                    readOnly: true
                    anchors.left: label2.right
                    anchors.bottom: parent.bottom
                }
            }

        }

    }

    Text {
        id: packet_text
        x: 271
        y: 8
        width: 221
        height: 119
        text: "00 01 02 03 04 05 06 07
10 11 12 13 14 15 16 17
20 21 22 23 24 25 26 27
30 31 32 33 34 35 36 37
40 41 42 43 44 45 46 47
50 51 52 53 54 55 56 57
60 61 62 63 64 65 66 67
70 71 72 73 74 75 76 77"
        font.bold: false
        font.family: "Courier"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 11
    }

    Button {
        id: getLastPacketBtn
        x: 271
        y: 133
        width: 221
        height: 23
        text: qsTr("Get last packet")
    }
}
