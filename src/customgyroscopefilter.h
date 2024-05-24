#ifndef CUSTOMGYROSCOPEFILTER_H
#define CUSTOMGYROSCOPEFILTER_H

#include <QObject>
#include <QtSensors>
#include "verifier.h"

class CustomGyroscopeFilter : public QObject, public QGyroscopeFilter
{
    Q_OBJECT

public:
    explicit CustomGyroscopeFilter(QObject *parent = nullptr);
    void setVerifier(Verifier* verifier);
    bool filter(QGyroscopeReading *reading) override;

private:
    Verifier* m_verifier;
    qreal m_x;
    qreal m_y;
    qreal m_z;
    qint64 m_timestamp;

signals:
    void gyroscopeDataChanged(qreal x, qreal y, qreal z);
};

#endif // CUSTOMGYROSCOPEFILTER_H
