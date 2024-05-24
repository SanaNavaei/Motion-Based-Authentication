#include "verifier.h"

Verifier::Verifier(GyroscopeManager* gyroscopeManager, AccelerometerManager* accelerometerManager, QObject *parent)
    : QObject{parent}
{
    m_verified = false;
    m_page = "main";
    m_state = "idle";
    m_finishRecordingTimeout = 5000000;
    m_gyroscopeManager = gyroscopeManager;
    m_accelerometerManager = accelerometerManager;
    m_verifiedPattern = QVector<Path*>();
    m_currentPattern = QVector<Path*>();
    m_errorBias_x = 0.5;
    m_errorBias_y = 0.5;
}

void Verifier::onRecordNewPatternPageClicked() 
{
    qDebug() << "New Pattern Page Button Clicked";
    m_page = "newPattern";
    m_state = "recordingAccelerometer";
    emit recordPatternStateChanged(QString::fromStdString(m_state));
    qDebug() << "Recording Accelerometer Now";
    m_verifiedPattern.clear();
    m_verified = false;
}

void Verifier::onCalibrationPageClicked()
{
    qDebug() << "Calibration Page Button Clicked";
    m_page = "calibration";
    m_state = "calibratingAccelerometer";
    m_gyroscopeManager->m_isCalibrated = false;
    m_gyroscopeManager->m_ZGyroscopeBiasArray.clear();
    m_accelerometerManager->m_isCalibrated = false;
    m_accelerometerManager->m_xAccelerationBiasArray.clear();
    m_accelerometerManager->m_yAccelerationBiasArray.clear();
    m_accelerometerManager->m_zAccelerationBiasArray.clear();
}

void Verifier::onAuthenticatePatternPageClicked()
{
    qDebug() << "Authenticate Pattern Page Button Clicked";
    m_page = "authenticatePattern";
    m_state = "recordingAccelerometer";
    emit recordPatternStateChanged(QString::fromStdString(m_state));
    qDebug() << "Recording Accelerometer Now";
    m_currentPattern.clear();
}

bool Verifier::isCalibrated()
{
    if (m_gyroscopeManager->m_isCalibrated && m_accelerometerManager->m_isCalibrated) {
        return true;
    }
    return false;
}

bool Verifier::originalPatternExists()
{
    if (m_verifiedPattern.size() > 0) {
        return true;
    }
    return false;
}

void Verifier::onBackButtonClicked()
{
    qDebug() << "Back Button Clicked";
    m_page = "main";
    m_state = "idle";
    reset();
}

bool Verifier::hasMoved()
{
    if (m_accelerometerManager->m_xPositionByAcceleration != 0 ||  m_accelerometerManager->m_yPositionByAcceleration != 0 || m_gyroscopeManager->m_angle != 0) {
        return true;
    }
    return false;
}

void Verifier::reset()
{
    m_gyroscopeManager->resetOnBackButton();
    m_accelerometerManager->resetOnBackButton();
}

void Verifier::onGyroscopeDataReceived(qreal x, qreal y, qreal z, qint64 timestamp)
{
    if (m_page == "calibration" && m_state == "calibratingGyroscope") {
        m_gyroscopeManager->m_lastTimeStamp = timestamp;
        if (!m_gyroscopeManager->m_isCalibrated) {
            m_gyroscopeManager->addBiasEntry(x, y, z);
            emit calibratingGyroscope();
        } else {
            m_state = "calibrated";
            emit finishedCalibrating();
            qDebug() << "Gyroscope is calibrated";
            qDebug() << "Z Gyroscope Bias: " << m_gyroscopeManager->m_zGyroscopeBias;
        }
    }
    else if (m_page == "newPattern" && m_state == "recordingGyroscope") {
        m_gyroscopeManager->processNewData(x, y, z, timestamp);
        if (m_gyroscopeManager->timedOut()) {
            m_gyroscopeManager->roundAngle();

            // TODO: Add the path section to the current pattern
            if (hasMoved()) {
                addVerifiedPatternSegment(m_accelerometerManager->m_xPositionByAcceleration, m_accelerometerManager->m_yPositionByAcceleration, m_gyroscopeManager->m_angle);
            }

            m_state = "recordingAccelerometer";
            emit recordPatternStateChanged(QString::fromStdString(m_state));
            qDebug() << "Accelerator: " << m_accelerometerManager->m_xPositionByAcceleration << " " << m_accelerometerManager->m_yPositionByAcceleration << " " << m_accelerometerManager->m_zPositionByAcceleration;
            qDebug() << "Gyroscope: " << m_gyroscopeManager->m_angle;
            qDebug() << "Recording Accelerometer Now";

            m_accelerometerManager->reset();
            m_gyroscopeManager->reset();

            if (m_gyroscopeManager->recordingFinished() && m_accelerometerManager->recordingFinished()) {
                // TODO: do something about finishing the pattern recording
                m_verified = true;
                emit finishedRecordingPattern();

                m_state = "idle";
                qDebug() << "Pattern recording finished";
                qDebug() << "State: " << m_state;
            }
        }
    }
    else if (m_page == "authenticatePattern" && m_state == "recordingGyroscope") {
        m_gyroscopeManager->processNewData(x, y, z, timestamp);
        if (m_gyroscopeManager->timedOut()) {
            m_gyroscopeManager->roundAngle();
            if (hasMoved()) {
                addAuthenticationPatternSegment(m_accelerometerManager->m_xPositionByAcceleration, m_accelerometerManager->m_yPositionByAcceleration, m_gyroscopeManager->m_angle);
            }
            m_state = "recordingAccelerometer";
            emit recordPatternStateChanged(QString::fromStdString(m_state));
            qDebug() << "Accelerator: " << m_accelerometerManager->m_xPositionByAcceleration << " " << m_accelerometerManager->m_yPositionByAcceleration << " " << m_accelerometerManager->m_zPositionByAcceleration;
            qDebug() << "Gyroscope: " << m_gyroscopeManager->m_angle;
            qDebug() << "Recording Accelerometer Now";

            m_accelerometerManager->reset();
            m_gyroscopeManager->reset();
            if (m_gyroscopeManager->recordingFinished() && m_accelerometerManager->recordingFinished()) {
                bool authenticationStatus = isAuthenticated();
                emit finishedAuthentication(authenticationStatus);
                m_state = "idle";
                qDebug() << "Pattern recording finished";
                qDebug() << "State: " << m_state;
                qDebug() << "Authenticated: " << authenticationStatus;
            }
        }
    }
}

void Verifier::onAccelerometerDataReceived(qreal x, qreal y, qreal z, qint64 timestamp)
{
    if (m_page == "calibration" && m_state == "calibratingAccelerometer") {
        m_accelerometerManager->m_lastTimeStamp = timestamp;
        if (!m_accelerometerManager->m_isCalibrated) {
            m_accelerometerManager->addBiasEntry(x, y, z);
            emit calibratingAccelerometer();
        } else {
            m_state = "calibratingGyroscope";
            qDebug() << "Accelerometer is calibrated";
            qDebug() << "X Acceleration Bias: " << m_accelerometerManager->m_xAccelerationBias;
            qDebug() << "Y Acceleration Bias: " << m_accelerometerManager->m_yAccelerationBias;
            qDebug() << "Z Acceleration Bias: " << m_accelerometerManager->m_zAccelerationBias;
        }
    }
    else if ((m_page == "newPattern" && m_state == "recordingAccelerometer")  || (m_page == "authenticatePattern" && m_state == "recordingAccelerometer")) {
        m_accelerometerManager->processNewData(x, y, z, timestamp);
        if (m_accelerometerManager->timedOut()) {
            m_state = "recordingGyroscope";
            emit recordPatternStateChanged(QString::fromStdString(m_state));
            if (m_accelerometerManager->m_xPositionByAcceleration != 0 || m_accelerometerManager->m_yPositionByAcceleration != 0 || m_accelerometerManager->m_zPositionByAcceleration !=0) {
                qDebug() << "X Position by Acceleration: " << m_accelerometerManager->m_xPositionByAcceleration;
                qDebug() << "Y Position by Acceleration: " << m_accelerometerManager->m_yPositionByAcceleration;
                qDebug() << "Z Position by Acceleration: " << m_accelerometerManager->m_zPositionByAcceleration;
            }
            qDebug() << "Recording Gyroscope Now";
        }
    }
}

void Verifier::addVerifiedPatternSegment(qreal x, qreal y, qreal angle)
{
    if (m_verified)
        return;

    Path* newPatternSegment = new Path{x, y, angle};
    m_verifiedPattern.append(newPatternSegment);
}

void Verifier::addAuthenticationPatternSegment(qreal x, qreal y, qreal angle)
{
    Path* newPatternSegment = new Path{x, y, angle};
    m_currentPattern.append(newPatternSegment);
}

bool Verifier::isAuthenticated()
{

    qDebug() << "Verified Pattern";
    showPattern(m_verifiedPattern);
    qDebug() << "============================================";
    qDebug() << "Current Pattern";
    showPattern(m_currentPattern);

    if (m_verifiedPattern.size() != m_currentPattern.size())
        return false;

    for (int i = 0; i < m_verifiedPattern.size(); i++) {
        if (qAbs(m_verifiedPattern[i]->m_x) < qAbs(m_verifiedPattern[i]->m_y)) {
            m_verifiedPattern[i]->m_x = 0;
        } else {
            m_verifiedPattern[i]->m_y = 0; 
        }
        if (qAbs(m_currentPattern[i]->m_x) < qAbs(m_currentPattern[i]->m_y)) {
            m_currentPattern[i]->m_x = 0;
        } else {
            m_currentPattern[i]->m_y = 0;
        }
        if ((qAbs(m_verifiedPattern[i]->m_x - m_currentPattern[i]->m_x) > (m_errorBias_x * m_verifiedPattern[i]->m_x)) ||
            (qAbs(m_verifiedPattern[i]->m_y - m_currentPattern[i]->m_y) > (m_errorBias_y * m_verifiedPattern[i]->m_y)) ||
            (m_verifiedPattern[i]->m_angle != m_currentPattern[i]->m_angle))  {
            return false;
        }
    }
    return true;
}

void Verifier::showPattern(QVector<Path*> pattern)
{
    qDebug() << "--------------------------------------------";
    for (int i = 0; i < pattern.size(); i++) {
        qDebug() << "Path " << i << ": ";
        qDebug() << "X: " << pattern[i]->m_x;
        qDebug() << "Y: " << pattern[i]->m_y;
        qDebug() << "Angle: " << pattern[i]->m_angle;
        qDebug() << "--------------------------------------------";
    }
}

