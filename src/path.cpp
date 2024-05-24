#include "path.h"

Path::Path(double x, double y, double angle, QObject *parent)
    : QObject{parent}
{
    m_x = x;
    m_y = y;
    m_angle = angle;
}
