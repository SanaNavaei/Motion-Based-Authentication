pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    visible: true
    required property int fontSize

    Connections {
        target: verifier
        onFinishedCalibrating: {
            console.log("Finished calibrating")
            calibratingFinished.visible = true
            calibratingFinished.text = "Calibration Finished"
            calibrationFinishedPopup.open()
            timer.running = true
        }
    }

    Connections {
        target: verifier
        onCalibratingGyroscope: {
            console.log("Calibrating Gyroscope")
            calibratingFinished.visible = true
            calibratingFinished.text = "Calibrating Gyroscope"
        }
    }

    Connections {
        target: verifier
        onCalibratingAccelerometer: {
            console.log("Calibrating Accelerometer")
            calibratingFinished.visible = true
            calibratingFinished.text = "Calibrating Accelerometer"
        }
    }

    Popup {
        id: calibrationFinishedPopup
        x: (root.width - width) / 2
        y: (layout.height - height) / 2
        width: 300
        height: 75
        closePolicy: Popup.CloseOnPressOutside
        focus: true
        modal: true

        background: Rectangle {
            color: "#4CAF50"  // Green color
            border.color: "#388E3C"  // Darker green for the border
            border.width: 10
            radius: 37
        }

        contentItem: Item {
            width: parent.width
            height: parent.height

            Text {
                anchors.centerIn: parent
                text: "Calibration Finished"
                font.pixelSize: 20
                color: "white"
            }
        }
    }

    Timer {
        id: timer
        interval: 3000
        repeat: false
        onTriggered: {
            calibrationFinishedPopup.close()
        }
    }

    ColumnLayout {
        id: layout

        anchors.fill: parent
        spacing: 10

        Image {
            id: image

            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: 340
            Layout.preferredWidth: 340
            fillMode: Image.PreserveAspectFit
            source: "images/gyro2.png"
        }

        Text {
            id: calibratingFinished
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
            text: "Calibration Finished"
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
