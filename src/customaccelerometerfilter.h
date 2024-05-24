#ifndef CUSTOMACCELEROMETERFILTER_H
#define CUSTOMACCELEROMETERFILTER_H

#include <QObject>
#include <QtSensors>
#include "verifier.h"

class CustomAccelerometerFilter : public QObject, public QAccelerometerFilter 
{
    Q_OBJECT
    
public:
    explicit CustomAccelerometerFilter(QObject *parent = nullptr);
    void setVerifier(Verifier* verifier);
    bool filter(QAccelerometerReading *reading) override;

private:
    Verifier* m_verifier;
    qreal m_x;
    qreal m_y;
    qreal m_z;
    qint64 m_timestamp;
    
signals:
    void accelerometerDataChanged(qreal x, qreal y, qreal z);
};

#endif // CUSTOMACCELEROMETERFILTER_H
