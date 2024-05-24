#ifndef PATH_H
#define PATH_H

#include <QObject>
#include <QQmlEngine>

class Path : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    Path(double x, double y, double angle, QObject *parent = nullptr);
    double m_x;
    double m_y;
    double m_angle;

signals:
};

#endif // PATH_H
