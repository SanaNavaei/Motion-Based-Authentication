// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAccelerometer>
#include "customaccelerometerfilter.h"
#include "customgyroscopefilter.h"
#include "accelerometermanager.h"
#include "gyroscopemanager.h"
#include "verifier.h"

const int ACCELEROMETER_DATARATE_HZ = 20;
const int GYROSCOPE_DATARATE_HZ = 20;
 

int main(int argc, char *argv[])
{
    QGuiApplication app(argc,argv);
    QGuiApplication::setOrganizationName("Cyber-Physical Systems");
    QGuiApplication::setApplicationName("Motion-Based Authentication");

    CustomAccelerometerFilter accelerometerFilter;
    CustomGyroscopeFilter gyroscopeFilter;

    QAccelerometer accelerometer;
    accelerometer.addFilter(&accelerometerFilter);
    accelerometer.setAccelerationMode(QAccelerometer::User);
    accelerometer.setSkipDuplicates(true);
    accelerometer.setDataRate(ACCELEROMETER_DATARATE_HZ);

    QGyroscope gyroscope;
    gyroscope.addFilter(&gyroscopeFilter);
    gyroscope.setSkipDuplicates(true);
    gyroscope.setDataRate(GYROSCOPE_DATARATE_HZ);

    AccelerometerManager accelerometerManager;
    GyroscopeManager gyroscopeManager;
    Verifier verifier = Verifier(&gyroscopeManager, &accelerometerManager);
    accelerometerFilter.setVerifier(&verifier);
    gyroscopeFilter.setVerifier(&verifier);

    accelerometer.start();
    gyroscope.start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("accelerometerFilter", &accelerometerFilter);
    engine.rootContext()->setContextProperty("gyroscopeFilter", &gyroscopeFilter);
    engine.rootContext()->setContextProperty("verifier", &verifier);
    engine.loadFromModule("MotionBasedAuthenticator", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
