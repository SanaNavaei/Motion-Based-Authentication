#ifndef VERIFIER_H
#define VERIFIER_H

#include <QObject>
#include <QVector>
#include <QQmlEngine>
#include <QString>
#include <string>
#include "accelerometermanager.h"
#include "gyroscopemanager.h"
#include "path.h"

class Verifier : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isCalibrated READ isCalibrated NOTIFY finishedCalibrating)
    Q_PROPERTY(bool originalPatternExists READ originalPatternExists NOTIFY finishedRecordingPattern)

public:
    Verifier(GyroscopeManager* gyroscopeManager, AccelerometerManager* m_accelerometerManager, QObject *parent = nullptr);
    Q_INVOKABLE void onRecordNewPatternPageClicked();
    Q_INVOKABLE void onCalibrationPageClicked();
    Q_INVOKABLE void onAuthenticatePatternPageClicked();
    Q_INVOKABLE void onBackButtonClicked();
    Q_INVOKABLE bool isCalibrated();
    Q_INVOKABLE bool originalPatternExists();
    void processSensorData();
    bool hasMoved();
    void reset();
    bool isAuthenticated();
    void showPattern(QVector<Path*> pattern);

public slots:
    void addVerifiedPatternSegment(qreal x, qreal y, qreal angle);
    void addAuthenticationPatternSegment(qreal x, qreal y, qreal angle);
    void onGyroscopeDataReceived(qreal x, qreal y, qreal z, qint64 timestamp);
    void onAccelerometerDataReceived(qreal x, qreal y, qreal z, qint64 timestamp);

private:
    bool m_verified;
    std::string m_page;
    std::string m_state;
    int m_finishRecordingTimeout;
    GyroscopeManager* m_gyroscopeManager;
    AccelerometerManager* m_accelerometerManager;
    qreal m_errorBias_x;
    qreal m_errorBias_y;

    QVector<Path*> m_verifiedPattern;
    QVector<Path*> m_currentPattern;

signals:
    void finishedRecordingPattern();
    void finishedCalibrating();
    void calibratingAccelerometer();
    void calibratingGyroscope();
    void recordPatternStateChanged(const QString &state);
    void finishedAuthentication(bool isAuthenticated);

};

#endif // VERIFIER_H
