#include "customaccelerometerfilter.h"

CustomAccelerometerFilter::CustomAccelerometerFilter(QObject *parent) 
    : QObject(parent) 
{
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_timestamp = 0;
}

void CustomAccelerometerFilter::setVerifier(Verifier* verifier) {
    m_verifier = verifier;
}

bool CustomAccelerometerFilter::filter(QAccelerometerReading *reading) {
    qreal x = reading->x();
    qreal y = reading->y();
    qreal z = reading->z();
    qint64 timestamp = reading->timestamp();
    if (x != m_x || y != m_y || z != m_z) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_timestamp = reading->timestamp();
        // emit accelerometerDataChanged(x, y, z);
        m_verifier->onAccelerometerDataReceived(x, y, z, timestamp);
        return true;
    }
    return false;
}
