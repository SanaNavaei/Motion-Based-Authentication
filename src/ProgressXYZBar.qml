// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0

    required property int fontSize
    property alias xAccText: xAcc.text
    property alias xAccValue: xAcc.value
    property alias xVelText: xVel.text
    property alias xVelValue: xVel.value
    property alias xPosText: xPos.text
    property alias xPosValue: xPos.value

    property alias yAccText: yAcc.text
    property alias yAccValue: yAcc.value
    property alias yVelText: yVel.text
    property alias yVelValue: yVel.value
    property alias yPosText: yPos.text
    property alias yPosValue: yPos.value

    // property alias zAccText: zAcc.text
    // property alias zAccValue: zAcc.value
    // property alias zVelText: zVel.text
    // property alias zVelValue: zVel.value
    // property alias zPosText: zPos.text
    // property alias zPosValue: zPos.value
   

    component NamedProgressBar: ColumnLayout {
        property alias text: axes.text
        property alias value: bar.value
        Text {
            id: axes
            font.pixelSize: root.fontSize
            Layout.fillWidth: true
        }
        ProgressBar {
            id: bar
            Layout.fillWidth: true
        }
    }

    NamedProgressBar {
        id: xAcc
    }

    NamedProgressBar {
        id: xVel
    }

    NamedProgressBar {
        id: xPos
    }

    NamedProgressBar {
        id: yAcc
    }

    NamedProgressBar {
        id: yVel
    }

    NamedProgressBar {
        id: yPos
    }

    // NamedProgressBar {
    //     id: zAcc
    // }

    // NamedProgressBar {
    //     id: zVel
    // }

    // NamedProgressBar {
    //     id: zPos
    // }
}
