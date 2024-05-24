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
        onFinishedAuthentication: {
            console.log("Finished Authentication")
            recordingGyroscopeText.visible = false
            recordingAccelerometerText.visible = false
            if (isAuthenticated) {
                authenticationSuccessfulPopup.open()
                timer.running = true
                authenticationText.text = "Authentication Successful"
                authenticationText.color = "green"
                successfullAuthenticationAnimation.visible = true
                loadingAnimation.visible = false
            } else {
                authenticationFailedPopup.open()
                timer.running = true
                authenticationText.text = "Authentication Failed"
                authenticationText.color = "red"
                failedAuthenticationAnimation.visible = true
                loadingAnimation.visible = false
            }
            authenticationText.visible = true
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
    
    Popup {
        id: authenticationSuccessfulPopup
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
                text: "Authentication Successful"
                font.pixelSize: 20
                color: "white"
            }
        }
    }

    Popup {
        id: authenticationFailedPopup
        modal: true
        width: 300
        height: 75
        x: (layout.width - width) / 2
        y: (layout.height - height) / 2
        closePolicy: Popup.CloseOnPressOutside
        focus: true

        background: Rectangle {
            color: "#F44336"  // Red color
            border.color: "#D32F2F"  // Darker red for the border
            border.width: 10
            radius: 37
        }

        contentItem: Item {
            width: parent.width
            height: parent.height

            Text {
                anchors.centerIn: parent
                text: "Authentication Failed"
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
            authenticationSuccessfulPopup.close()
            authenticationFailedPopup.close()
        }
    }

    ColumnLayout {
        id: layout

        anchors.fill: parent
        spacing: 10

        // Image {
        //     id: image

        //     Layout.alignment: Qt.AlignCenter
        //     Layout.preferredHeight: root.imageSize
        //     Layout.preferredWidth: root.imageSize
        //     fillMode: Image.PreserveAspectFit
        //     source: "images/qt_logo.png"
        // }

        AnimatedImage {
            id: authencationInProcessAnimation
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: root.imageSize
            Layout.preferredWidth: root.imageSize
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 340
            sourceSize.height: 340
            source: "images/AuthenticationInProcess.gif"
        }

        AnimatedImage {
            id: loadingAnimation
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: root.imageSize
            Layout.preferredWidth: root.imageSize
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 100
            sourceSize.height: 100
            source: "images/loading.gif"
            visible: true
        }

        AnimatedImage {
            id: successfullAuthenticationAnimation
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: root.imageSize
            Layout.preferredWidth: root.imageSize
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 100
            sourceSize.height: 100
            source: "images/Successful.gif"
            visible: false
        }

        AnimatedImage {
            id: failedAuthenticationAnimation
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: root.imageSize
            Layout.preferredWidth: root.imageSize
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 100
            sourceSize.height: 100
            source: "images/Failed.gif"
            visible: false
        }

        Text {
            id: recordingAccelerometerText
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
            text: "Recording accelerometer data"
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: recordingGyroscopeText
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
            text: "Recording gyroscope data"
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: authenticationText
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
            text: ""
            visible: false
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}