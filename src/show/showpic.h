#pragma once

#include "util/util.h"
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QWheelEvent>
#include <QWidget>

class ShowPic : public QMainWindow
{
        Q_OBJECT

    public:
        ShowPic(QPixmap *inimage = nullptr, QRect *rect = nullptr)
        {
            setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            setWindowTitle("czyshot show");
            move(rect->x(), rect->y());

            if (inimage == nullptr)
            {
                qDebug() << "image is null"
                         << "\n";
            }
            image = *inimage;

            centralWidget = new QWidget(this);
            setCentralWidget(centralWidget);

            imageLabel = new QLabel(centralWidget);
            imageLabel->setPixmap(image);
            imageLabel->setScaledContents(true);
            imageLabel->setAlignment(Qt::AlignCenter);
            setFixedSize(image.size());

            imageLabel->setFocusPolicy(Qt::WheelFocus);
        }

    protected:
        void wheelEvent(QWheelEvent *event) override
        {
            int delta = event->angleDelta().y();

            qreal scaleFactor = (delta > 0) ? 1.1 : 0.9;
            QSize needSize = scaleFactor * imageLabel->size();
            if ((needSize.width() < 100 || needSize.height() < 100)
                || (needSize.width() > 4000 || needSize.height() > 4000))
            {
                return;
            }
            setFixedSize(needSize);
            imageLabel->resize(needSize);
        }

        void mouseDoubleClickEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton)
            {
                close();
            }
        }

        void keyPressEvent(QKeyEvent *event) override
        {
            if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
            {
                copyPixmap(this->image.toImage());
                /* } else { */
                /*   QWidget::keyPressEvent(event); */
            }
            else if (event->key() == Qt::Key_S && (event->modifiers() & Qt::ControlModifier))
            {
                savePixmap(this->image);
            }
            if (event->key() == Qt::Key_Escape)
            {
                close();
            }
        }

    private:
        QWidget *centralWidget;
        QLabel *imageLabel;

        QPixmap image;
};
