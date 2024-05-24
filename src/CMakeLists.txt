# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(motionbasedauthenticator LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/sensors/motionbasedauthenticator")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Svg Sensors)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(motionbasedauthenticator
    main.cpp
)

set_target_properties(motionbasedauthenticator PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if(ANDROID)
    set_property(TARGET motionbasedauthenticator PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif()

if(APPLE AND IOS)
    set_property(TARGET motionbasedauthenticator PROPERTY
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )
else()
    # default search path for the config file is "qrc:/"
    qt_add_resources(motionbasedauthenticator "resources"
        FILES qtquickcontrols2.conf)
endif()

qt_add_qml_module(motionbasedauthenticator
    URI MotionBasedAuthenticator
    VERSION 1.0
    QML_FILES
        "Main.qml"
        "RecordNewPattern.qml"
        "ProgressXYZBar.qml"
    RESOURCES
        "images/compass.svg"
        "images/magnet.svg"
        "images/qt_logo.png"
        "images/auth_logo.png"
    SOURCES
        sensorsupport.h
        SOURCES path.h path.cpp
        SOURCES verifier.h verifier.cpp
        SOURCES customaccelerometerfilter.h customaccelerometerfilter.cpp
        SOURCES customgyroscopefilter.h customgyroscopefilter.cpp
        SOURCES gyroscopemanager.h gyroscopemanager.cpp
        SOURCES accelerometermanager.h accelerometermanager.cpp
        QML_FILES Calibrate.qml
        QML_FILES AuthenticatePattern.qml
        RESOURCES images/auth_logo.png images/AuthenticationInProcess.gif images/AuthenticationInProcessRaw.gif images/compass.svg images/Failed.gif images/FailedRaw1.gif images/FailedRaw2.gif images/gyro1.png images/gyro2.png images/gyro2Raw.jpg images/loading.gif images/magnet.svg images/qt_logo.png images/Successful.gif images/SuccessfulRaw.gif
)

target_link_libraries(motionbasedauthenticator
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::Svg
        Qt::Sensors
)


install(TARGETS motionbasedauthenticator
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
