#ifndef IMAGES_H
#define IMAGES_H

#include <QMap>
#include <QImage>
#include <QString>

class Images
{
public:
    Images();
    void load();
    QImage& get(const QString& imageName);
    bool isLoaded();

private:
    QMap<QString, QImage> images;
    bool loadedFlag;
};

extern Images pictures;

#endif // IMAGES_H
