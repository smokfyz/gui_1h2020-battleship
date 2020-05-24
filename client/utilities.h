#ifndef UTILITIES_H
#define UTILITIES_H

#include <QSound>
#include <QUrl>
#include <QResource>
#include <QPoint>
#include <QDebug>
#include "definitions.h"

QPoint getMyFieldCoord(const QPoint& position);
QPoint getEnemyFieldCoord(const QPoint& position);

void playHitSound();
void playMissSound();

#endif // UTILITIES_H
