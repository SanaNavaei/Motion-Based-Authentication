#ifndef GYROSCOPEMANAGER_H
#define GYROSCOPEMANAGER_H

#include <QObject>
#include <QVector>

class GyroscopeManager : public QObject
{
    Q_OBJECT

public:
    GyroscopeManager(qreal epsilon = 0.1, qint64 timeout = 2000000, qint64 finishRecordingTimeout = 10000000, qreal threshold = 45, QObject *parent = nullptr);
    bool timedOut();
    bool recordingFinished();
    void addBiasEntry(qreal x, qreal y, qreal z);
    void roundAngle();
    void reset();
    void resetOnBackButton();
    void processNewData(qreal x, qreal y, qreal z, qint64 timestamp);
    
    qreal m_epsilon;
    int m_timeout;
    int m_finishRecordingTimeout;
    qreal m_threshold;
    bool m_firstCall;
    bool m_isCalibrated;

    qint64 m_lastTimeStamp;
    qint64 m_lastVelocityTimeStamp_Z;
    qint64 m_lastRealVelocityTimeStamp;

    qreal m_x;
    qreal m_y;
    qreal m_z;
    qreal m_angle;

    QVector<qreal> m_ZGyroscopeBiasArray;
    qreal m_zGyroscopeBias;

private:
    
signals:

};

#endif // GYROSCOPEMANAGER_H
