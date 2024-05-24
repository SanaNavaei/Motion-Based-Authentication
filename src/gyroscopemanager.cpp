#include "gyroscopemanager.h"

GyroscopeManager::GyroscopeManager(qreal epsilon, qint64 timeout, qint64 finishRecordingTimeout, qreal threshold, QObject *parent)
    : QObject{parent}
{
    m_epsilon = epsilon;
    m_timeout = timeout;
    m_finishRecordingTimeout = finishRecordingTimeout;
    m_threshold = threshold;
    m_firstCall = true;
    m_lastTimeStamp = 0;
    m_lastVelocityTimeStamp_Z = 0;
    m_lastRealVelocityTimeStamp = 0;
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_angle = 0;
}

bool GyroscopeManager::timedOut() {
    return m_lastTimeStamp - m_lastVelocityTimeStamp_Z > m_timeout;
}

bool GyroscopeManager::recordingFinished() {
    return m_lastTimeStamp - m_lastRealVelocityTimeStamp > m_finishRecordingTimeout;
}

void GyroscopeManager::addBiasEntry(qreal x, qreal y, qreal z) {
    m_ZGyroscopeBiasArray.append(z);
    if (m_ZGyroscopeBiasArray.size() == 20) {
        m_zGyroscopeBias = std::accumulate(m_ZGyroscopeBiasArray.begin(), m_ZGyroscopeBiasArray.end(), 0.0) / m_ZGyroscopeBiasArray.size();
        m_isCalibrated = true;
    }
}

void GyroscopeManager::roundAngle() {
    m_angle = ((int(m_angle) % 360) + 360) % 360;
    if (qAbs(m_angle - 0) <= m_threshold) {
        m_angle = 0;
    }
    else if (qAbs(m_angle - 90) <= m_threshold) {
        m_angle = 90;
    }
    else if (qAbs(m_angle - 180) <= m_threshold) {
        m_angle = 180;
    }
    else if (qAbs(m_angle - 270) <= m_threshold) {
        m_angle = 270;
    }
    else if (qAbs(m_angle - 360) <= m_threshold) {
        m_angle = 0;
    }
}

void GyroscopeManager::reset() {
    m_firstCall = true;
    m_lastVelocityTimeStamp_Z = 0;
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_angle = 0;
}

void GyroscopeManager::resetOnBackButton() {
    reset();
    m_lastTimeStamp = 0;
    m_lastRealVelocityTimeStamp = 0;
}

void GyroscopeManager::processNewData(qreal x, qreal y, qreal z, qint64 timestamp) {
    qint64 timeSinceLast = timestamp - m_lastTimeStamp;
    m_lastTimeStamp = timestamp;

    if (m_firstCall) {
        m_firstCall = false;
        m_lastVelocityTimeStamp_Z = timestamp;
        if (m_lastRealVelocityTimeStamp == 0) {
            m_lastRealVelocityTimeStamp = timestamp;
        }
        return;
    }

    z -= m_zGyroscopeBias;
    qreal normalizedZ = qAbs(z) < m_epsilon ? 0 : z;
    normalizedZ = normalizedZ * (static_cast<qreal>(timeSinceLast) / 1000000.0);
    m_angle += normalizedZ;
    if (normalizedZ != 0) {
        m_lastVelocityTimeStamp_Z = timestamp;
    }
}
