#include "images.h"

Images pictures = Images();

Images::Images() :
    loadedFlag(false)
{
}

void Images::load()
{
    if (loadedFlag)
    {
        return;
    }
    images.insert("field", QImage(":/pictures/background.png"));
    images.insert("dot", QImage(":/pictures/dot.png"));
    images.insert("full", QImage(":/pictures/full.png"));
    images.insert("half", QImage(":/pictures/half.png"));
    images.insert("redhalf", QImage(":/pictures/redhalf.png"));
    images.insert("redfull", QImage(":/pictures/redfull.png"));
    loadedFlag = true;
}

QImage &Images::get(const QString &imageName)
{
    QMap<QString, QImage>::iterator iter = images.find(imageName);
    if (iter == images.end())
    {
        throw "No such picture";
    }
    return iter.value();
}

bool Images::isLoaded()
{
    return loadedFlag;
}

