#pragma once

#include "showpic.h"
#include "util/util.h"
#include <QMainWindow>
#include <QPainter>
#include <QPixmap>

class CapPic : public QMainWindow
{
        Q_OBJECT

    public:
        CapPic(QPixmap *pixmap = nullptr)
        {
            setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            setWindowTitle("czyshot cap");
            showFullScreen();

            if (pixmap == nullptr)
            {
                qDebug() << "image is null"
                         << "\n";
            }
            image = *pixmap;
            setFixedSize(image.size());
        }

    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton)
            {
                unsetCursor();
                close();
            }
        }

        void keyPressEvent(QKeyEvent *event) override
        {
            if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            {
                isShowPic = true;
                unsetCursor();
                close();
            }
            else if (event->key() == Qt::Key_Escape)
            {
                unsetCursor();
                close();
            }
            else if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
            {
                isShowPic = false;
                unsetCursor();
                close();
                QPixmap pixmap = this->image.copy(QRect(m_startPT, m_endPT));
                copyPixmap(pixmap.toImage());
            }
            else if (event->key() == Qt::Key_S && (event->modifiers() & Qt::ControlModifier))
            {
                isShowPic = false;
                unsetCursor();
                close();
                QPixmap pixmap = this->image.copy(QRect(m_startPT, m_endPT));
                savePixmap(pixmap);
            }
            else if (event->key() == Qt::Key_A && (event->modifiers() & Qt::ControlModifier))
            {
                m_startPT = QPoint(0, 0);
                m_endPT = QPoint(this->image.toImage().size().width(),
                                 this->image.toImage().size().height());
                this->update();
            }
        }

        void paintEvent(QPaintEvent *event) override
        {
            QPainter p(this);

            QPalette palette;
            palette.setBrush(backgroundRole(), image);
            setPalette(palette);

            setCursor(Qt::CrossCursor);

            QPen pen;
            pen.setWidth(5);
            pen.setColor(QColor("#00B0AE"));
            pen.setStyle(Qt::SolidLine);
            p.setPen(pen);
            p.drawRect(QRect(m_startPT, m_endPT));

            QPainter painter(this);
            QBrush brush(QColor(128, 128, 128, 128));
            painter.setBrush(brush);
            painter.drawRect(QRect(0, 0, window()->width(), window()->height()));
            painter.eraseRect(QRect(m_startPT, m_endPT));
        }

        void mousePressEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton)
            {
                isFirstMove = true;
                isPressedWidget = true;
            }
            if (event->button() == Qt::RightButton)
            {
                unsetCursor();
                close();
            }
            if (event->button() == Qt::MiddleButton)
            {
                isShowPic = true;
                unsetCursor();
                close();
            }
        }

        void mouseMoveEvent(QMouseEvent *event) override
        {
            if (isFirstMove)
            {
                m_endPT = m_startPT = event->pos();
                isFirstMove = false;
            }
            else
            {
                m_endPT = event->pos();
            }
            this->update();
        }

        void mouseReleaseEvent(QMouseEvent *event) override
        {
            isPressedWidget = false;
            if (m_startPT.x() > m_endPT.x())
            {
                std::swap(m_startPT.rx(), m_endPT.rx());
            }
            if (m_startPT.y() > m_endPT.y())
            {
                std::swap(m_startPT.ry(), m_endPT.ry());
            }
        }

        void closeEvent(QCloseEvent *event) override
        {
            if (m_startPT == m_endPT || !isShowPic)
            {
                event->accept();
                return;
            }
            QRect rect(m_startPT, m_endPT);
            qDebug() << "RECT:" << rect.x() << " " << rect.y() << " " << rect.width() << " "
                     << rect.height() << "\n";
            QPixmap croppedPixmap = image.copy(rect);
            ShowPic *newWindow = new ShowPic(&croppedPixmap, &rect);
            newWindow->show();
            event->accept();
        }

    private:
        QWidget *centralWidget;
        QLabel *imageLabel;

        QPixmap image;
        bool isPressedWidget = false;
        QPoint m_startPT;
        QPoint m_endPT;

        bool isShowPic = false;
        bool isFirstMove = true;
};
