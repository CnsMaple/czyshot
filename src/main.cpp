#include "show/cappic.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::vector<unsigned char> imageData;
    char temp;
    while (std::cin.get(temp))
    {
        imageData.push_back(static_cast<unsigned char>(temp));
    }

    QImage image;
    QPixmap pixmap;
    image.loadFromData(imageData.data(), imageData.size());
    pixmap = QPixmap::fromImage(image);

    CapPic cappic1(&pixmap);
    cappic1.show();
    return app.exec();
}
