// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root

    readonly property int defaultFontSize: 22
    readonly property int imageSize: width / 2

    width: 420
    height: 760
    visible: true
    title: "Motion-Based Authentication"

    header : ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            ToolButton {
                id: back
                text: qsTr("Back")
                font.pixelSize: root.defaultFontSize - 4
                visible: stack.depth > 1
                onClicked: {
                    stack.pop();
                    heading.text = root.title;
                    verifier.onBackButtonClicked()
                }
                Layout.alignment: Qt.AlignLeft
            }
            Label {
                id: heading
                text: root.title
                font.pixelSize: root.defaultFontSize
                font.weight: Font.Medium
                verticalAlignment: Qt.AlignVCenter
                Layout.alignment: Qt.AlignCenter
                Layout.preferredHeight: 55
            }
            Item {
                visible: back.visible
                Layout.preferredWidth: back.width
            }
        }
    }

    // Accelerometer {
    //     id: accelerometer

    //     onPatternSectionRecorded: {
    //         // TODO
    //     }
    // }

    // Connections {
    //     target: accelerometerFilter
    //     onAccelerometerDataChanged: {
    //         console.log("Accelerometer data changed: ", x, y, z)
    //     }
    // }

    // Connections {
    //     target: gyroscopeFilter
    //     onGyroscopeDataChanged: {
    //         console.log("Gyroscope data changed: ", x, y, z)
    //     }
    // }

    StackView {
        id: stack

        // Pushes the object and forwards the properties
        function pusher(object : string) : void {
            // Trim the suffix and set it as new heading
            heading.text = object.split(".")[0]
            return stack.push(object, {
                fontSize: root.defaultFontSize,
                imageSize: root.imageSize
            })
        }

        anchors.fill: parent
        anchors.margins: width / 12

        initialItem: Item {
            ColumnLayout {
                id: initialItem

                // anchors.fill: parent
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                component CustomButton: Button {
                    highlighted: true
                    font.pixelSize: root.defaultFontSize
                    font.letterSpacing: 1.5
                    Layout.alignment: Qt.AlignCenter
                    // Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Image {
                    id: image

                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredHeight: root.imageSize
                    Layout.preferredWidth: root.imageSize
                    fillMode: Image.PreserveAspectFit
                    source: "images/auth_logo.png"

                }

                CustomButton {
                    text: "Calibrate"
                    onClicked: {
                        stack.pusher("Calibrate.qml")
                        verifier.onCalibrationPageClicked()
                    }
                    enabled: SensorSupport.hasAccelerometer() && SensorSupport.hasGyroscope()
                }

                CustomButton {
                    text: "Record New Pattern"
                    onClicked: {
                        stack.pusher("RecordNewPattern.qml")
                        verifier.onRecordNewPatternPageClicked()
                    }
                    enabled: SensorSupport.hasAccelerometer() && SensorSupport.hasGyroscope() && verifier.isCalibrated
                }
                
                CustomButton {
                    text: "Authenticate Pattern"
                    onClicked: {
                        stack.pusher("AuthenticatePattern.qml")
                        verifier.onAuthenticatePatternPageClicked()
                    }
                    enabled: SensorSupport.hasAccelerometer() && SensorSupport.hasGyroscope() && verifier.isCalibrated && verifier.originalPatternExists
                }
            }
        }
    }
}
