import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import com.ics.demo 1.0


ApplicationWindow {
    id: testwnd
    title: qsTr("STM32 USB Bulk")
    width: 500
    height: 217
    visible: true

/*    Connections {
        target: drvusb
        onSendTime:
        {
            console.log("Text Changed: " + time);
            testwnd.title = time;
        }
    }
*/
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm {
        id : mainform
        anchors.fill: parent
        connectBtn.onClicked:
        {
            connstatusEdit.text = "Connecting..."
            transceiver.connect();
        }
        offBtn.onClicked:
        {
            var ret = transceiver.send(0);
            if (ret > 0)
            {
                connstatusEdit.text = "LED is OFF"
            }
            else
            {
                connstatusEdit.text = "Send failure"
            }
        }
        onBtn.onClicked:
        {
            var ret = transceiver.send(1);
            if (ret > 0)
            {
                connstatusEdit.text = "LED is ON"
            }
            else
            {
                connstatusEdit.text = "Send failure"
            }
        }
        disconnectBtn.onClicked:
        {
            transceiver.disconnect();
            //messageDialog.show(qsTr("Button 3 pressed"))
        }
        statusBtn.onClicked:
        {
            var ret = transceiver.read();
            //ledvalueEdit.text = "test"
            ledvalueEdit.text = ret;
        }
        wiBtn.onClicked:
        {
            if (button === Qt.LeftButton)
            {
                wiBtn.signal_strength+=5
                if (wiBtn.signal_strength>100)
                    wiBtn.signal_strength = 100
                connstatusEdit.text = "LEFT"
            }
            else if (button === Qt.RightButton)
            {
                wiBtn.signal_strength-=5
                if (wiBtn.signal_strength<0)
                    wiBtn.signal_strength = 0
                connstatusEdit.text = "RIGHT"
            }
            else
            {
                connstatusEdit.text = "UNKNOWN"
            }
        }
        wiBtn.onBindingClicked:
        {
            wiBtn.binded = ! wiBtn.binded
            if (button === Qt.LeftButton)
            {
                connstatusEdit.text = "LEFT2"
            }
            else if (button === Qt.RightButton)
            {
                connstatusEdit.text = "RIGHT2"
            }
            else
            {
                connstatusEdit.text = "UNKNOWN2"
            }
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    Transceiver {
        id: transceiver
        onStatusChanged: {
            if (transceiver.status) {
                mainform.connstatusEdit.text = "Connected"
            } else {
                mainform.connstatusEdit.text = "Disconnected"
            }
        }
        onPacketReceived: {
            mainform.packetTxt.text = txt;
        }
    }

    Item {
        Timer {
            interval: 500; running: false; repeat: true
            onTriggered: transceiver.connect();
        }

        Text { id: time }
    }
}
