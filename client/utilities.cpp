#include "utilities.h"

QPoint getMyFieldCoord(const QPoint &position)
{
    QPoint result(-1, -1);

    if(position.x() < MYFIELD_X || position.x() > (MYFIELD_X + FIELD_WIDTH) ||
       position.y() < MYFIELD_Y || position.y() > (MYFIELD_Y + FIELD_HEIGHT))
    {
        return result;
    }
    result.setX(static_cast<int>(1.0 * (position.x() - MYFIELD_X) / (0.1 * FIELD_WIDTH)));
    result.setY(static_cast<int>(1.0 * (position.y() - MYFIELD_Y) / (0.1 * FIELD_HEIGHT)));
    return result;
}

QPoint getEnemyFieldCoord(const QPoint &position)
{
    QPoint result(-1, -1);

    if(position.x() < ENEMYFIELD_X || position.x() > (ENEMYFIELD_X + FIELD_WIDTH) ||
       position.y() < ENEMYFIELD_Y || position.y() > (ENEMYFIELD_Y + FIELD_HEIGHT))
    {
        return result;
    }
    result.setX(static_cast<int>(1.0 * (position.x() - ENEMYFIELD_X) / (0.1 * FIELD_WIDTH)));
    result.setY(static_cast<int>(1.0 * (position.y() - ENEMYFIELD_Y) / (0.1 * FIELD_HEIGHT)));
    return result;
}

void playHitSound()
{
    QSound audio(":/sounds/hit.mp3");
    audio.play();
}

void playMissSound()
{
    QSound audio(":/sounds/miss.mp3");
    audio.play();
}
