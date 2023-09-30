#pragma once

#include <QBuffer>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QPixmap>
#include <sys/wait.h>
#include <unistd.h>

bool RunPipe(const std::string &cmd, QPixmap &pixmap);

bool copyPixmap(QImage image);

bool savePixmap(QPixmap pixmap);
