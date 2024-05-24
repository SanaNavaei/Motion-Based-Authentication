#ifndef ACCELEROMETERMANAGER_H
#define ACCELEROMETERMANAGER_H

#include <QObject>
#include <QVector>
#include <QDebug>

class AccelerometerManager : public QObject
{
    Q_OBJECT

public:
    AccelerometerManager(qreal epsilon = 0.1, qint64 timeout = 200000, qint64 finishRecordingTimeout = 10000000, QObject *parent = nullptr);
    bool timedOut();
    bool xTimedOut();
    bool yTimedOut();
    bool zTimedOut();
    bool recordingFinished();
    void addBiasEntry(qreal x, qreal y, qreal z);
    void processNewData(qreal x, qreal y, qreal z, qint64 timestamp);
    void reset();
    void resetOnBackButton();

    qreal m_epsilon;
    qint64 m_timeout;
    qint64 m_finishRecordingTimeout;
    bool m_firstCall;
    bool m_isCalibrated;

    qint64 m_lastTimeStamp;
    qint64 m_lastAccelarationTimeStamp_X;
    qint64 m_lastAccelarationTimeStamp_Y;
    qint64 m_lastAccelarationTimeStamp_Z;
    qint64 m_lastRealAccelerationTimeStamp;

    qreal m_x;
    qreal m_y;
    qreal m_z;

    qreal m_xAcceleration;
    qreal m_yAcceleration;
    qreal m_zAcceleration;

    qreal m_xVelocity;
    qreal m_yVelocity;
    qreal m_zVelocity;

    qreal m_xPositionByAcceleration;
    qreal m_yPositionByAcceleration;
    qreal m_zPositionByAcceleration;

    qreal m_xPositionByVelocity;
    qreal m_yPositionByVelocity;
    qreal m_zPositionByVelocity;

    QVector<qreal> m_xAccelerationBiasArray;
    QVector<qreal> m_yAccelerationBiasArray;
    QVector<qreal> m_zAccelerationBiasArray;

    qreal m_xAccelerationBias;
    qreal m_yAccelerationBias;
    qreal m_zAccelerationBias;

private:

signals:

};

#endif // ACCELEROMETERMANAGER_H
