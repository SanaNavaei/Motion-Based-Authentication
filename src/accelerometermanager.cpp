#include "accelerometermanager.h"

AccelerometerManager::AccelerometerManager(qreal epsilon, qint64 timeout, qint64 finishRecordingTimeout, QObject *parent)
    : QObject{parent}
{
    m_epsilon = epsilon;
    m_timeout = timeout;
    m_finishRecordingTimeout = finishRecordingTimeout;
    m_firstCall = true;
    m_isCalibrated = false;

    m_lastTimeStamp = 0;
    m_lastAccelarationTimeStamp_X = 0;
    m_lastAccelarationTimeStamp_Y = 0;
    m_lastAccelarationTimeStamp_Z = 0;
    m_lastRealAccelerationTimeStamp = 0;

    m_x = 0;
    m_y = 0;
    m_z = 0;

    m_xAcceleration = 0;
    m_yAcceleration = 0;
    m_zAcceleration = 0;

    m_xVelocity = 0;
    m_yVelocity = 0;
    m_zVelocity = 0;

    m_xPositionByAcceleration = 0;
    m_yPositionByAcceleration = 0;
    m_zPositionByAcceleration = 0;

    m_xPositionByVelocity = 0;
    m_yPositionByVelocity = 0;
    m_zPositionByVelocity = 0;

    m_xAccelerationBiasArray = QVector<qreal>();
    m_yAccelerationBiasArray = QVector<qreal>();
    m_zAccelerationBiasArray = QVector<qreal>();

    m_xAccelerationBias = 0;
    m_yAccelerationBias = 0;
    m_zAccelerationBias = 0;
}

bool AccelerometerManager::timedOut() {
    return (m_lastTimeStamp - m_lastAccelarationTimeStamp_X > m_timeout * 10) && 
           (m_lastTimeStamp - m_lastAccelarationTimeStamp_Y > m_timeout * 10) &&
           (m_lastTimeStamp - m_lastAccelarationTimeStamp_Z > m_timeout * 10);
}

bool AccelerometerManager::xTimedOut() {
    return m_lastTimeStamp - m_lastAccelarationTimeStamp_X > m_timeout;
}

bool AccelerometerManager::yTimedOut() {
    return m_lastTimeStamp - m_lastAccelarationTimeStamp_Y > m_timeout;
}

bool AccelerometerManager::zTimedOut() {
    return m_lastTimeStamp - m_lastAccelarationTimeStamp_Z > m_timeout;
}

bool AccelerometerManager::recordingFinished() {
    if (m_lastTimeStamp - m_lastRealAccelerationTimeStamp > m_finishRecordingTimeout) {
        return true;
    }
    return false;
}

void AccelerometerManager::addBiasEntry(qreal x, qreal y, qreal z) {
    m_xAccelerationBiasArray.append(x);
    m_yAccelerationBiasArray.append(y);
    m_zAccelerationBiasArray.append(z);
    if (m_xAccelerationBiasArray.size() == 20) {
        m_xAccelerationBias = std::accumulate(m_xAccelerationBiasArray.begin(), m_xAccelerationBiasArray.end(), 0.0) / m_xAccelerationBiasArray.size();
        m_yAccelerationBias = std::accumulate(m_yAccelerationBiasArray.begin(), m_yAccelerationBiasArray.end(), 0.0) / m_yAccelerationBiasArray.size();
        m_zAccelerationBias = std::accumulate(m_zAccelerationBiasArray.begin(), m_zAccelerationBiasArray.end(), 0.0) / m_zAccelerationBiasArray.size();
        m_isCalibrated = true;
    }
}

void AccelerometerManager::processNewData(qreal x, qreal y, qreal z, qint64 timestamp) {
    qint64 timeSinceLast = timestamp - m_lastTimeStamp;
    m_lastTimeStamp = timestamp;

    if (m_firstCall) {
        m_firstCall = false;
        m_lastAccelarationTimeStamp_X = timestamp;
        m_lastAccelarationTimeStamp_Y = timestamp;
        m_lastAccelarationTimeStamp_Z = timestamp;
        if (m_lastRealAccelerationTimeStamp == 0) {
            m_lastRealAccelerationTimeStamp = timestamp;
        }
        return;
    }

    x -= m_xAccelerationBias;
    y -= m_yAccelerationBias;
    z -= m_zAccelerationBias;

    m_xAcceleration = qAbs(x) < m_epsilon ? 0 : x;
    m_yAcceleration = qAbs(y) < m_epsilon ? 0 : y;
    m_zAcceleration = qAbs(z) < m_epsilon ? 0 : z;

    qreal timeSinceLastInSeconds = static_cast<qreal>(timeSinceLast) / 1000000.0;

    m_xVelocity += static_cast<qreal>(m_xAcceleration) * timeSinceLastInSeconds; 
    m_yVelocity += static_cast<qreal>(m_yAcceleration) * timeSinceLastInSeconds;
    m_zVelocity += static_cast<qreal>(m_zAcceleration) * timeSinceLastInSeconds;

    m_xPositionByVelocity += m_xVelocity * timeSinceLastInSeconds;
    m_yPositionByVelocity += m_yVelocity * timeSinceLastInSeconds;
    m_zPositionByVelocity += m_zVelocity * timeSinceLastInSeconds;

    m_xPositionByAcceleration += 0.5 * m_xAcceleration * timeSinceLastInSeconds * timeSinceLastInSeconds + m_xVelocity * timeSinceLastInSeconds;
    m_yPositionByAcceleration += 0.5 * m_yAcceleration * timeSinceLastInSeconds * timeSinceLastInSeconds + m_yVelocity * timeSinceLastInSeconds;
    m_zPositionByAcceleration += 0.5 * m_zAcceleration * timeSinceLastInSeconds * timeSinceLastInSeconds + m_zVelocity * timeSinceLastInSeconds;

    if (m_xAcceleration != 0 || m_yAcceleration != 0) {
        qDebug() << "X Acceleration: " << m_xPositionByAcceleration << " Y Acceleration: " << m_yPositionByAcceleration;
    }

    if (m_xAcceleration != 0) {
        m_lastAccelarationTimeStamp_X = timestamp;
        m_lastRealAccelerationTimeStamp = timestamp;
    } else if (xTimedOut()) {
        m_xVelocity = 0;
    }

    if (m_yAcceleration != 0) {
        m_lastAccelarationTimeStamp_Y = timestamp;
        m_lastRealAccelerationTimeStamp = timestamp;
    } else if (yTimedOut()) {
        m_yVelocity = 0;
    }

    if (m_zAcceleration != 0) {
        m_lastAccelarationTimeStamp_Z = timestamp;
        m_lastRealAccelerationTimeStamp = timestamp;
    } else if (zTimedOut()) {
        m_zVelocity = 0;
    }
}

void AccelerometerManager::reset() {
    m_firstCall = true;
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_xAcceleration = 0;
    m_yAcceleration = 0;
    m_zAcceleration = 0;
    m_xVelocity = 0;
    m_yVelocity = 0;
    m_zVelocity = 0;
    m_xPositionByAcceleration = 0;
    m_yPositionByAcceleration = 0;
    m_zPositionByAcceleration = 0;
    m_xPositionByVelocity = 0;
    m_yPositionByVelocity = 0;
    m_zPositionByVelocity = 0;
}

void AccelerometerManager::resetOnBackButton() {
    reset();
    m_lastTimeStamp = 0;
    m_lastAccelarationTimeStamp_X = 0;
    m_lastAccelarationTimeStamp_Y = 0;
    m_lastAccelarationTimeStamp_Z = 0;
    m_lastRealAccelerationTimeStamp = 0;
}
