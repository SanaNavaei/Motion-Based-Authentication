// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtSensors

Item {
    id: root
    visible: false

    required property int fontSize
    required property int imageSize

    function resetRotations() : void
    {
        imageZRotation.angle = 0
    }

    Connections {
        target: verifier
        onFinishedRecordingPattern: {
            console.log("Finished Recording")
            recordingAccelerometerText.visible = false
            recordingGyroscopeText.visible = false
            recordingFinishedPopup.open()
            timer.running = true
            recordingPatternStatusText.visible = true
            recordingPatternFinishedAnimation.visible = true
        }
    }

    Popup {
        id: recordingFinishedPopup
        modal: true
        width: 300
        height: 75
        x: (layout.width - width) / 2
        y: (layout.height - height) / 2
        closePolicy: Popup.CloseOnPressOutside
        focus: true
        
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
                text: "Recording Finished"
                font.pixelSize: 20
                color: "white"
            }
        }
    }

    Timer {
        id: timer
        interval: 3000
        running: false
        repeat: false
        onTriggered: {
            recordingFinishedPopup.close()
        }
    }

    Connections {
        target: verifier
        onRecordPatternStateChanged: function(state){
            if (state === "recordingAccelerometer") {
                recordingAccelerometerText.visible = true
                recordingGyroscopeText.visible = false
            }
            else if (state === "recordingGyroscope") {
                recordingAccelerometerText.visible = false
                recordingGyroscopeText.visible = true
            }
        }
    }
        

    Gyroscope {
        id: gyroscope

        property real lastTimeStamp: 0
        property real z: 0

        active: true
        dataRate: 25

        onReadingChanged: {
            z = (reading as GyroscopeReading).z
            let firstCall = false
            if (lastTimeStamp == 0) {
                firstCall = true
            }
            let timeSinceLast = reading.timestamp - lastTimeStamp
            lastTimeStamp = reading.timestamp
            if (firstCall === true)
                return
            let normalizedZ = z * (timeSinceLast / 1000000)
            imageZRotation.angle += normalizedZ
        }
    }

    ColumnLayout {
        id: layout

        anchors.fill: parent
        spacing: 10

        Image {
            id: image

            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: root.imageSize
            Layout.preferredWidth: root.imageSize
            fillMode: Image.PreserveAspectFit
            source: "images/qt_logo.png"

            transform: [
                Translate {
                    id: imageTranslation

                    x: 0
                    y: 0
                },
                Rotation {
                    id: imageZRotation
                    angle: gyroscope.zAngle
                    axis.x: 0
                    axis.y: 0
                    axis.z: 1
                    origin.x: layout.width / 2
                    origin.y: layout.height / 2
                }
            ]
        }

        // property alias text: axes.text
        // property alias value: angle.value
        // Text {
        //     id: axes
        //     font.pixelSize: root.fontSize
        //     Layout.fillWidth: true
        //     text: "Angle of rotation: " + imageZRotation.angle.toFixed(2)
        // }

        AnimatedImage {
            id: recordingPatternFinishedAnimation
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: 100
            Layout.preferredWidth: 100
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 50
            sourceSize.height: 50
            source: "images/Successful.gif"
            visible: false
        }

        Text {
            id: recordingAccelerometerText
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
            text: "Recording Accelerometer Data"
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: recordingGyroscopeText
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
            text: "Recording Gyroscope Data"
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: recordingPatternStatusText
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
            text: "Recording Pattern Finished"
            visible: false
            color: "green"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Button {
            highlighted: true
            font.pixelSize: root.defaultFontSize
            font.letterSpacing: 1.5
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            onClicked: root.resetRotations()
            text: "Reset Rotation"
        }
    }
}
